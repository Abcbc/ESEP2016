#include "src/controller/height_measurement/height_measurement.h"
#include "Src/controller/event_table.h"
#include "lib/dispatcher/Dispatcher.cpp"
#include "config.h"

#define UNDEFINED 1576
#define MAX_HEIGHT 2200
#define MIN_HEIGHT 3000
#define GROUND_HEIGHT 4000
#define INIT_HEIGHT 4076 // band 8
#define MAX_LENGTH 750
#define MIN_LENGTH 5
#define MIN_ARRAY_LENGTH 40
#define SUM_IC_WM 3
#define SMALLEST_ABS_ERROR 20000
#define PROTOTYPE_LENGTH 450
#define CUT_OFF_FRONT 12
#define SUM_PUKS 10
#define STOPPED 1
#define INIT_MEASUREMENTS 4
#define END -1

#define FILTER_TOL_BIN 0.05
#define CUT_OFF_TOL_BIN 0.8
#define MAX_PUK_VALUE 1024

#define LO_NORMAL_CUT_OFF_TOL 0.94
#define LO_NORMAL_FILTER_TOL 0.02
#define LO_NORMAL_UPPER_TOL 2900
#define LO_NORMAL_LOWER_TOL 2700

#define SUM_IC_PROFILES 22
#define SUM_HOLE_PROFILES 4
#define PROFILES_LENGTH 23
#define PROFILE_LENGTH 15
#define HOLE_UPPER 3800
#define HOLE_LOWER 3400
#define GROOVE_UPPER 2750
#define GROOVE_LOWER 2635
#define NORMALE_UPPER 2600
#define MINIMAL_WIDTH 5
#define EXTRA_CUT_OFF 5

#define TICK_SEC 0
#define TICK_NSEC 20000

static uint32_t last_type;
static bool is_stopped;
static int32_t _height_diff;

int timer_cid;
int timer_con;
timer_t timerId;
struct itimerspec timerSpec;
struct sigevent   timerEvent;

static uint32_t const INVALID_PUK_TYPE_ID_E = ERR_UNDEFINED_PUK_E_ID;
static uint32_t const HM_START_EVENT_ID_E = HEIGHT_SENSOR_MEASURE_START_E_ID;
static uint32_t const HM_DONE_EVENT_ID_E = IDENTIFIED_PUK_E_ID;

double Height_Measurement::profiles[26][15] = {
		/* 0 iron_core */        {2, 1, 2, 0, 2, 1, 2, END, END, END, END, END, END, END, END},
		/* 1 iron_core */        {2, 1, 0, 2, 1, 2, END, END, END, END, END, END, END, END, END},
		/* 2 iron_core */        {2, 1, 2, 1, 2, 0, 2, 1, 2, END, END, END, END, END, END},
		/* 3 iron_core */        {2, 1, 2, 1, 0, 2, 1, 2, END, END, END, END, END, END, END},
		/* 4 iron_core */        {1, 2, 1, 2, 0, 2, 1, 2, END, END, END, END, END, END, END},
		/* 5 iron_core */        {1, 2, 1, 0, 2, 1, 2, END, END, END, END, END, END, END, END},
		/* 6 iron_core */        {2, 1, 2, 0, 2, 1, 2, 1, 2, END, END, END, END, END, END},
		/* 7 iron_core */        {2, 1, 0, 2, 1, 2, 1, 2, END, END, END, END, END, END, END},
		/* 8 iron_core */        {2, 1, 2, 0, 2, 1, END, END, END, END, END, END, END, END, END},
		/* 9 iron_core */        {2, 1, 0, 2, 1, END, END, END, END, END, END, END, END, END, END},
		/* 10 iron_core */       {1, 2, 1, 2, 0, 2, 1, 2, 1, END, END, END, END, END, END},
		/* 11 iron_core */       {2, 1, 2, 0, 2, 1, 2, 1, END, END, END, END, END, END, END},
		/* 12 iron_core */       {1, 2, 1, 2, 0, 2, 1, 2, END, END, END, END, END, END, END},
		/* 13 iron_core */       {2, 1, 0, 1, 2, 1, 2, END, END, END, END, END, END, END, END},
		/* 14 iron_core */       {2, 1, 2, 0, 2, END, END, END, END, END, END, END, END, END, END},
		/* 15 iron_core */       {2, 2, 0, 2, 1, 2, END, END, END, END, END, END, END, END, END},
		/* 16 iron_core */       {2, 2, 0, 2, 2, END, END, END, END, END, END, END, END, END, END},
		/* 17 iron_core */       {2, 0, 2, 1, 2, END, END, END, END, END, END, END, END, END, END},
		/* 18 iron_core */       {2, 1, 2, 1, 0, 1, 2, 1, 2, END, END, END, END, END, END},
		/* 19 iron_core */       {2, 1, 2, 0, 2, 2, END, END, END, END, END, END, END, END, END},
		/* 20 iron_core */       {2, 2, 0, 2, 2, END, END, END, END, END, END, END, END, END, END},
		/* 21 iron_core */		 {2, 0, 2, 2, END, END, END, END, END, END, END, END, END, END, END},
	//	/* 22 iron_core */		 {2, 2, 0, 2, END, END, END, END, END, END, END, END, END, END, END},
		/* 21 hole */            {2, 0, 2, END, END, END, END, END, END, END, END, END, END, END, END},
		/* 22 hole */            {1, 2, 0, 2, 1, END, END, END, END, END, END, END, END, END, END},
		/* 23 hole */            {1, 2, 0, 2, END, END, END, END, END, END, END, END, END, END, END},
		/* 24 hole */            {2, 0, 2, 1, END, END, END, END, END, END, END, END, END, END, END},
};

Height_Measurement::Height_Measurement(){
	Dispatcher* d = Dispatcher::getInstance();
	d->addListener(this, SPEED_NORMAL_E_ID);
	d->addListener(this, SPEED_SLOW_E_ID);
	d->addListener(this, SPEED_STOP_E_ID);
	last_type = 0;
	is_stopped = false;
	_height_diff = 0;
	timer_cid = 0;
	timer_con = 0;
};

void Height_Measurement::execute(void*){
    /* Zugriffsrechte von QNX fuer diesen Thread, auf die Hardware erbitten. */
    if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        cout << "Can't get Hardware access, therefore can't do anything." << endl;
    }

    //Variablen deklarieren
	uint32_t msg;
	uint32_t level;
	struct height_array ha_unfiltered;
	Height_sensor* height_sensor = Height_sensor::get_instance();

	//Höhensensor kalibrieren
	get_height_difference(height_sensor);

	//Endlosschleife für Messung
	while(1){
	    //Wenn das Laufband steht keine neue Messungen durchführen
		while(is_stopped){usleep(200);}
		level = height_sensor -> get_height();

		//Wenn aktuelle Höhe kleiner als Bodenhöhe is
		if(level < (GROUND_HEIGHT + _height_diff)){
			ha_unfiltered = get_height_array();
			MsgSendPulse(3, -1, 5, TIMER_MEASURE_OUT_E_ID); //TODO MAGIC_NUMBER
			
			//DEBUG
			cout << "Array Length: " << ha_unfiltered.length << endl;
			for(msg = 0; msg < ha_unfiltered.length; msg++){
				cout << ha_unfiltered.array[msg] << ", ";
			}
			cout << endl;

			//Wenn die Größe größer als die Mindestgröße ist
			if(ha_unfiltered.length >= MIN_ARRAY_LENGTH){
//				last_type = compare_arrays(ha);
			    //Versuche Type zu erkennen
				last_type = evaluate_puk_type(ha_unfiltered);

				//Bereite Nachricht an Dispatcher vor
				msg = (last_type == UNDEFINED ? INVALID_PUK_TYPE_ID_E : HM_DONE_EVENT_ID_E);

				//DEBUG
				cout << "I_P value = " << IDENTIFIED_PUK_E_ID << endl;
				cout << "Msg value: " << msg << endl;

				//Informiere Dispatcher
				MsgSendPulse(3, -1, 5, msg); //TODO MAGICH_NUMBER
			}
		} 
	}
}

void Height_Measurement::shutdown(){
	// ignore
}

void  Height_Measurement::set_motor_speed(uint32_t speed){
	is_stopped = (speed == STOPPED);
}
void Height_Measurement::SPEED_STOP(){
    cout << "################### is stopped #####################" << endl;
	is_stopped = true;
}

void Height_Measurement::SPEED_NORMAL(){
    cout << "################### is normal #####################" << endl;
	is_stopped = false;
}

void Height_Measurement::SPEED_SLOW(){
    cout << "################### is slow #####################" << endl;
	is_stopped = false;
}


uint32_t Height_Measurement::get_type(void){
	return last_type;
}

/*
 * Erstellt ein Höhenmessungsarray
 */
height_array Height_Measurement::get_height_array(void){
    //Vars deklarieren
	Height_sensor* height_sensor = Height_sensor::get_instance();
	struct _pulse pulse;
	std::vector<double> height_vector;
	bool in_measurement = true;
	uint32_t length_counter = 0;
	uint32_t level;
	uint32_t ground_debouncer = 0;
	struct height_array ha;
	ha.min = 0;
	ha.max = 0;


	do{
		// wait for tick
		MsgReceivePulse(timer_cid, &pulse, sizeof (pulse), NULL);

		//Aktueller Messwert
		level = (double) height_sensor -> get_height();

		//Wenn Anzahl der Messwerte größer als erlaubt ist oder der Messwert zu klein ist
		if(++length_counter >= MAX_LENGTH || level <= MAX_HEIGHT){
			//Messwerte verwerfen
		    ha.length = 1;
			height_vector.clear();
			height_vector.push_back(INVALID_PUK_TYPE_ID_E);
			ha.array = &height_vector[0];

			//Dispatcher informieren TODO MAGIC_NUMBER
			MsgSendPulse(3, -1, 5, INVALID_PUK_TYPE_ID_E);

			//Kennung für ungültigen Wert zurückgeben
			return ha;

		}
		//ansonsten wenn der Messwert größer gleich als Boden-Niveau ist
		else if(level >= GROUND_HEIGHT) {
		    //Wenn 10 Werte größer waren als Boden-Niveau, dann beende Messung
			if(ground_debouncer >= 10){
				in_measurement = false;
			}
			++ground_debouncer;
		}
		//ansonsten nehme Messwert auf
		else {
			height_vector.push_back(level);
		}

		//Wenn Mindestanzahl an Messwerten aufgenommen wurde, informiere Dispatcher über Messung
		if(length_counter == MIN_LENGTH ){
			MsgSendPulse(3, -1, 5, HM_START_EVENT_ID_E); //TODO MAGIC_NUMBER
		}

		//Wenn das Laufband steht, führe keine weiteren Messungen aus
		while(is_stopped){
		    MsgReceivePulse(timer_cid, &pulse, sizeof (pulse), NULL);
		}
	//Laufe solange weiter, bis Messung beendet wird
	} while(in_measurement);

	//Setze Struct zum Rückgegeben, falls Werte vorhanden
	if(!height_vector.empty()){
		ha.length = height_vector.size();
		ha.array = &height_vector[0];
		return ha;
	}
	else {
        //Undefinierte Messung (keine Werte aufgenommen)
        ha.length = 1;
        height_vector.clear();
        height_vector.push_back(UNDEFINED);
        ha.array = &height_vector[0];
        return ha;
	}
}

/*
 * Kalibriert den Höhensensor
 */
void Height_Measurement::get_height_difference(Height_sensor* height_sensor){
	int i;
	_height_diff = 0;

	//Nimmt n-Initialmesswerte
	for(i = 0; i < INIT_MEASUREMENTS; i++){
		_height_diff += height_sensor -> get_height();
	}

	//Durchschnitt der Initialmesswerte minus Soll-Bodenwert
	_height_diff = _height_diff / INIT_MEASUREMENTS - INIT_HEIGHT;
	cout << "_height_diff" << _height_diff << endl;
}

/*
 * Identifiziere Puk anhand ungefilterter Messwerte
 */
uint32_t Height_Measurement::evaluate_puk_type(height_array ha_unfiltered){
	uint32_t type = UNDEFINED;

	//Plausi
	if (Height_Measurement::mean(ha_unfiltered) > MIN_HEIGHT){
	    return UNDEFINED;
	}

	//Erkenne, ob der Typ Little-One oder Normal ist
	type = is_little_or_normal(ha_unfiltered);

	//Wenn der Typ undefiniert ist (d.h. noch nicht erkannt wurde)
	if(type == UNDEFINED){
	    //Dann, erkenne ob der Typ IC oder Hole ist
		type = is_ic_or_hole(ha_unfiltered);
	}

	//Wenn der Typ weiterhin undefiniert ist (d.h. noch nicht erkannt wurde)
	if(type == UNDEFINED){
	    //Dann, erkenne den Bitcode
		type = is_bit_code(ha_unfiltered);
	}


	cout << "type = " << type << endl;
	return type;
}

uint32_t Height_Measurement::is_bit_code(height_array ha_unfiltered){
	//Vars deklarieren
    int counter = 0;
	uint32_t type = UNDEFINED;
	int indexer = 0;
	int i;
	struct height_array ha_filtered;
	ha_filtered = filter_array(ha_unfiltered);
	type = 0;

	//Wenn das Array nicht leer ist
	if(ha_filtered.length != 0){

	    //Merke den zuletzt erfassten Wert
		double last_height = ha_filtered.array[0];

		//Durchlaufe das ganze Array
		for(i = 0; i < ha_filtered.length; i++){
		    //Wenn der Wert gleich dem letzten Wert ist und der Zähler nicht beim vorletzten Wert ist
			if(ha_filtered.array[i] == last_height && i != (ha_filtered.length - 1)){
			    //zähle den Counter hoch
				++counter;
			}
			//Ansonsten
			else {
			        //DEBUG
					cout << counter << endl;

					//Wenn der Indexer gleich 0 ist
					if(indexer == 0){
					    //erhöhe den Counter und verdopple diesen
						counter = (counter + 1) * 2;
					}
//					if(indexer == 1){
//						counter += 6;
//					}

					//Wenn der Counter größer als 40 ist
					if(counter > 40){
					    //Typ wird verodert mit Indexer und vorangestellter 1
						type |= 1 << indexer;
						++indexer;
					}
					//Wenn Counter größer als 5 ist
					else if(counter > 4){
						++indexer;
					}

					//Merke letzten Wert und setze Counter auf 1
					last_height = ha_filtered.array[i];
					counter = 1;
			}
		}
		//DEBUG
		cout << type << endl;

		//Wenn der Typ kleiner als maximaler Puk-Wert ist
		if(type < MAX_PUK_VALUE){
		    //Shifte 4 Bits nach rechts
			type >>= 4;
			//Nur die ersten 5 Bits erlauben
			type &= 0x1F;
		}
		else{
			type = UNDEFINED;
		}
	}
	else{
		type = UNDEFINED;
	}


	return type;
}

uint32_t Height_Measurement::is_ic_or_hole(height_array ha_unfiltered){
	//Vars deklarieren
    uint32_t type = UNDEFINED;
	int i;
	int y;
	int counter = 0;
	bool is_type = false;
	double temp_array[ha_unfiltered.length - CUT_OFF_FRONT];
    uint32_t new_length = 0;

    //Last ist der letzte Typus der erfasst wurde
    double last = -1;

    double mean = Height_Measurement::mean(ha_unfiltered);

	//Fülle erste TODO MAGIC_NUMBER 15 Stellen mit -1
	for(i = 0; i < 15; i++){
		temp_array[i] = -1;
	}

	//Zähle von Cut-Off-Front plus zusätliches Cut-Off bis Länge minus zusätzliches Cut-Off
	for(i = CUT_OFF_FRONT + EXTRA_CUT_OFF; i < ha_unfiltered.length - EXTRA_CUT_OFF; i++){
	    //Wenn der prozentuale Unterschied vom aktuellen Wert zum Mittelwert kleiner als TODO MAGIC_NUMBER kleiner als 25% ist
		if(((ha_unfiltered.array[i] - mean) / mean) < 0.25){
		    //Wenn der aktuelle Wert kleiner ist als Normale_Upper minus Höhendifferenz ist, dann
			if(ha_unfiltered.array[i] < NORMALE_UPPER - _height_diff){
			    //Wenn der letzte Wert ungleich zwei ist
				if(last != 2){
				    //setze Counter auf 0
					counter = 0;
				}
				//Counter hochzählen und last auf 2 setzen
				++counter;
				last = 2;

				//Wenn der Counter gleich der minimalen Breite ist
				if(counter == MINIMAL_WIDTH){
				    //Setze Wert im temp-Array auf 2 und zähle Länge von diesem hoch
					temp_array[new_length] = 2;
					++new_length;
				}
			}
			//ansonsten wenn der aktuelle Wert größer als GROOVE-Lower minus Höhendifferenz und kleiner als GROOVE-UPPER minus Höhendifferenz ist
			else if(ha_unfiltered.array[i] > GROOVE_LOWER - _height_diff && ha_unfiltered.array[i] < GROOVE_UPPER - _height_diff){
			    //Wenn Last ungleich 1 ist
				if(last != 1){
				    //setze Counter auf 0
					counter = 0;
				}

				//Counter hochzählen und last auf 1 setzen
				++counter;
				last = 1;

				//Wenn der Counter gleich der minimalen Breite ist
				if(counter == MINIMAL_WIDTH){
				    //Setze Wert im temp-Array auf 1 und zähle die Länge von diesem hoch
					temp_array[new_length] = 1;
					++new_length;
				}

			}
			//Wenn der aktuelle Wert zwischen HOLE-LOWER und HOLE-Upper liegt
			else if(ha_unfiltered.array[i] > HOLE_LOWER - _height_diff && ha_unfiltered.array[i] < HOLE_UPPER - _height_diff){
				//Wenn last ungleich ist
			    if(last != 0){
			        //setze Counter auf 0
					counter = 0;
				}

			    //Zähle Counter hoch und setze last auf 0
				++counter;
				last = 0;

				//Wenn der Counter gleich der minimalen Breite ist
				if(counter == MINIMAL_WIDTH){
				    //Setze Wert im temp-Array auf 1 und zähle die Länge von diesem hoch
					temp_array[new_length] = 0;
					++new_length;
				}
			}
		}
	}

	//DEBUG
	cout << new_length << endl;
	for(i = 0; i < new_length; i++){
		cout << temp_array[i] << ", ";
	}
	cout << endl;

	//Durchlaufe alle Muster von Iron-Core und Hole
	for(i = 0; i < PROFILES_LENGTH; i++){
	    //Durchlaufe alle Werte vom aktuellen Muster
		for(y = 0; y < PROFILE_LENGTH; y++){
		    //Wenn der Wert des Musters ungleich dem Wert der Messung ist
			if(profiles[i][y] != temp_array[y]){
			    //breche ab
				break;
			}
			//ansonsten, wenn der Wert der Messung gleich -1 (-1 steht für Ende) ist TODO MAGIC NUMBER
			else if(temp_array[y] == (double) -1){
			    //setze ist Typ
				is_type = true;
			}
		}

		//Wenn der Typ erkannt wurde, breche weitere Analyse ab
		if(is_type){
			type = i;
			break;
		}
	}

	//Wenn der Typ kleiner als der IC-Profil-Index ist, ist es ein IronCore
	if(type < SUM_IC_PROFILES){
		type = puk_iron_core;
		cout << "iron_core" << endl;
	}
	//Ansonsten, wenn der Typ kleiner als IC plus Hole-Profil ist, ist es ein Hole
	else if(type < SUM_IC_PROFILES + SUM_HOLE_PROFILES){
		type = puk_hole;
		cout << "puk_hole" << endl;
	}


	return type;
}

/*
 * Erkennt kleine oder normale Puks
 */
uint32_t Height_Measurement::is_little_or_normal(height_array ha_unfiltered){
    //Vars deklarieren
	uint32_t type = UNDEFINED;
	int i;
	int new_length = 0;
	double mittle_value = 0;
	double mean = Height_Measurement::mean(ha_unfiltered);

	//Zähle von CUT_OFF bis Array Length
	for(i = CUT_OFF_FRONT; i < ha_unfiltered.length; i++){
	    //Wenn der prozentuale Unterschied von Mittelwert und dem aktuellen Wert kleiner als Normal-Filter-Toleranz istg
		if(((ha_unfiltered.array[i] - mean) / mean) < LO_NORMAL_FILTER_TOL){
		    //Nehme den Wert, wenn er ab der Mitte liegt und noch kein Wert erfasst wurde
			if((ha_unfiltered.length / 2) < i && mittle_value == 0){
				mittle_value = ha_unfiltered.array[i];
			}
			++new_length;
		}
	}

	//Wenn die Anzahl der Werte, die dem Filter entsprechen größer als die Normal-Cut-off-Toleranz ist
	if((new_length + CUT_OFF_FRONT) >= (ha_unfiltered.length * LO_NORMAL_CUT_OFF_TOL)){
	    //DEBUG
		cout << mean << endl;

		//Wenn der Mittelwert kleiner als Upper-Toleranz ist und der Wert ab der Mitte größer als die Lower-Toleranz ist, dann
		if(mean < LO_NORMAL_UPPER_TOL && mittle_value > LO_NORMAL_LOWER_TOL){
			//Lege Typ auf Little One fest
		    type = puk_litte_one;
		    //DEBUG
			cout << "littleone" << endl;
		}
		//ansonsten wenn Mittelwert kleiner als TODO ist
		else if(mean < 2600){ //TODO Magic-Number
		    //Lege Typ auf Normal fest
			type = puk_normal;
			//DEBUG
			cout << "normal" << endl;
		}
	}


	return type;
}

height_array Height_Measurement::filter_array(height_array ha_raw){
	//Vars deklarieren
    int i;
	double mean = Height_Measurement::mean(ha_raw);
	int new_length = 0;
	double werte_filtered[ha_raw.length - CUT_OFF_FRONT];
//	double* binary_array;
	double min = -1;
	double max = -1;

	//DEBUG
	cout << "mean: " << mean << endl;

	//Zähle von CUT_OFF bis Ende des Arrays
	for(i = CUT_OFF_FRONT; i < ha_raw.length; i++){
	    //Wenn der prozentuale Unterschied vom aktuellen Wert zum Mittelwert kleiner als FILTER_TOL_BIN
		if(((ha_raw.array[i] - mean) / mean) < FILTER_TOL_BIN){
		    //Nehme Wert im neuen Array auf
			werte_filtered[new_length] = ha_raw.array[i];
			++new_length;

			//Berechne min und max
			if(min == -1 || min > ha_raw.array[i]){
				min = ha_raw.array[i];
			}
			if(max < ha_raw.array[i]){
				max = ha_raw.array[i];
			}
		}
	}

	//Wenn die neue Länge kleiner als ?
	if(new_length < (ha_raw.length - CUT_OFF_FRONT) * CUT_OFF_TOL_BIN){
		ha_raw.length = 0;
		return ha_raw;
	}

	//DEBUG
	cout << "Array Length: " << new_length << endl;
	for(i = 0; i < new_length; i++){
		cout << werte_filtered[i] << ", ";
	}
	cout << endl;

	//Lege binäres Array an
	double binary_array[new_length];

	//Durchlaufe neues Array
	for(i = 0; i < new_length; i++){
	    //Wenn der Wert kleiner ist als der Durchschnitt von min und max
		if(werte_filtered[i] < ((max + min) / 2)){
		    //Logisch 1
			binary_array[i] = 1;
		} else {
		    //Logisch 0
			binary_array[i] = 0;
		}
	}

	//DEBUG
	cout << "Array Length: " << new_length << endl;
	for(i = 0; i < new_length; i++){
		cout << binary_array[i] << ", ";
	}
	cout << endl;

	//Setze neue Werte
	ha_raw.length = new_length;
	ha_raw.array = binary_array;
	ha_raw.min = min;
	ha_raw.max = max;


	return ha_raw;
}

/*
 * Ermittle Mittelwert
 */
double Height_Measurement::mean(height_array ha){
	double mean = 0;
	int i;
	for(i = 0; i < ha.length; i++){
		mean += ha.array[i];
	}
	mean /= ha.length;
	return mean;
}

void  Height_Measurement::init_timer(){
	// init pulse message
	if( (timer_cid  = ChannelCreate(0)) == -1){
		cout << "ChannelCreate() failed"<< endl;
	}
	if( (timer_con = ConnectAttach(0, 0, timer_cid, 0, 0)) == -1){
		cout << "ConnectAttach() failed"<< endl;
	}
	SIGEV_PULSE_INIT (&timerEvent, timer_con, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    if (timer_create (CLOCK_REALTIME, &timerEvent, &timerId) == -1) {
    	cout << "timer_create() failed"<< endl;
    }

    // init timer
	timerSpec.it_value.tv_sec = TICK_SEC;
	timerSpec.it_value.tv_nsec = TICK_NSEC;
	timerSpec.it_interval.tv_sec = TICK_SEC;
	timerSpec.it_interval.tv_nsec = TICK_NSEC;
	timer_settime (timerId, 0, &timerSpec, NULL);
}
