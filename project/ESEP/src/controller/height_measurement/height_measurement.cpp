#include "src/controller/height_measurement/height_measurement.h"
#include "Src/controller/event_table.h"
#include "lib/dispatcher/Dispatcher.cpp"
#include "config.h"

#define UNDEFINED 1576
#define MAX_HEIGHT 2200
#define GROUND_HEIGHT 4000
#define INIT_HEIGHT 4076 // band 8
#define MAX_LENGTH 750
#define MIN_LENGTH 5
#define SUM_IC_WM 3
#define SMALLEST_ABS_ERROR 20000
#define PROTOTYPE_LENGTH 450
#define CUT_OFF_FRONT 12
#define SUM_PUKS 10
#define STOPPED 1
#define INIT_MEASUREMENTS 4

#define FILTER_TOL_BIN 0.05
#define CUT_OFF_TOL_BIN 0.8
#define MAX_PUK_VALUE 1024

#define LO_NORMAL_CUT_OFF_TOL 0.94
#define LO_NORMAL_FILTER_TOL 0.02
#define LO_NORMAL_UPPER_TOL 2900
#define LO_NORMAL_LOWER_TOL 2700

#define SUM_IC_PROFILES 21
#define SUM_HOLE_PROFILES 4
#define PROFILES_LENGHT 23
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
static int32_t height_diff;

int timer_cid;
int timer_con;
timer_t timerId;
struct itimerspec timerSpec;
struct sigevent   timerEvent;

static uint32_t const INVALID_PUK_TYPE_ID_E = ERR_UNDEFINED_PUK_E_ID;
static uint32_t const HM_START_EVENT_ID_E = HEIGHT_SENSOR_MEASURE_START_E_ID;
static uint32_t const HM_DONE_EVENT_ID_E = IDENTIFIED_PUK_E_ID;

double Height_Measurement::profiles[26][15] = {
		/* 0 iron_core */        {2, 1, 2, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 1 iron_core */        {2, 1, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 2 iron_core */        {2, 1, 2, 1, 2, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1},
		/* 3 iron_core */        {2, 1, 2, 1, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1},
		/* 4 iron_core */        {1, 2, 1, 2, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1},
		/* 5 iron_core */        {1, 2, 1, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 6 iron_core */        {2, 1, 2, 0, 2, 1, 2, 1, 2, -1, -1, -1, -1, -1, -1},
		/* 7 iron_core */        {2, 1, 0, 2, 1, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1},
		/* 8 iron_core */        {2, 1, 2, 0, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 9 iron_core */        {2, 1, 0, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 10 iron_core */       {1, 2, 1, 2, 0, 2, 1, 2, 1, -1, -1, -1, -1, -1, -1},
		/* 11 iron_core */       {2, 1, 2, 0, 2, 1, 2, 1, -1, -1, -1, -1, -1, -1, -1},
		/* 12 iron_core */       {1, 2, 1, 2, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1},
		/* 13 iron_core */       {2, 1, 0, 1, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 14 iron_core */       {2, 1, 2, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 15 iron_core */       {2, 2, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 16 iron_core */       {2, 2, 0, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 17 iron_core */       {2, 0, 2, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 18 iron_core */       {2, 1, 2, 1, 0, 1, 2, 1, 2, -1, -1, -1, -1, -1, -1},
		/* 19 iron_core */       {2, 1, 2, 0, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 20 iron_core */       {2, 2, 0, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 21 iron_core */		 {2, 0, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	//	/* 22 iron_core */		 {2, 2, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 21 hole */            {2, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 22 hole */            {1, 2, 0, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 23 hole */            {1, 2, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/* 24 hole */            {2, 0, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

Height_Measurement::Height_Measurement(){
	Dispatcher* d = Dispatcher::getInstance();
	d->addListener(this, SPEED_NORMAL_E_ID);
	d->addListener(this, SPEED_SLOW_E_ID);
	d->addListener(this, SPEED_STOP_E_ID);
	last_type = 0;
	is_stopped = false;
	height_diff = 0;
	timer_cid = 0;
	timer_con = 0;
};

void Height_Measurement::execute(void*){
    /* Zugriffsrechte von QNX fuer diesen Thread, auf die Hardware erbitten. */
    if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        cout << "Can't get Hardware access, therefore can't do anything." << endl;
    }

	uint32_t msg;
	uint32_t level;
	struct height_array ha;
	struct height_array ha_unfiltered;
	Height_sensor* height_sensor = Height_sensor::get_instance();
	get_height_difference(height_sensor);
	while(1){
		level = height_sensor -> get_height();
		if(level < (GROUND_HEIGHT + height_diff)){
			ha_unfiltered = get_height_array();
			MsgSendPulse(3, -1, 5, TIMER_MEASURE_OUT_E_ID);
			
			cout << "Array Length: " << ha_unfiltered.length << endl;
			for(msg = 0; msg < ha_unfiltered.length; msg++){
				cout << ha_unfiltered.array[msg] << ", ";
			}
			cout << endl;

			if(ha_unfiltered.length >= MIN_LENGTH){
//				last_type = compare_arrays(ha);
				last_type = evaluate_puk_type(ha_unfiltered);
				msg = (last_type == UNDEFINED ? INVALID_PUK_TYPE_ID_E : HM_DONE_EVENT_ID_E);
				cout << "I_P value = " << IDENTIFIED_PUK_E_ID << endl;
				cout << "Msg value: " << msg << endl;
				MsgSendPulse(3, -1, 5, msg);
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
//	is_stopped = true;
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

height_array Height_Measurement::get_height_array(void){
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

		level = (double) height_sensor -> get_height();
		if(++length_counter >= MAX_LENGTH || level <= MAX_HEIGHT){
			ha.length = 1;
			height_vector.clear();
			height_vector.push_back(INVALID_PUK_TYPE_ID_E);
			ha.array = &height_vector[0];
			MsgSendPulse(3, -1, 5, INVALID_PUK_TYPE_ID_E);
			return ha;
		}else if(level >= GROUND_HEIGHT){
			if(ground_debouncer >= 10){
				in_measurement = false;
			}
			++ground_debouncer;
		}else{
			height_vector.push_back(level);
		}
		if(length_counter == MIN_LENGTH ){
			MsgSendPulse(3, -1, 5, HM_START_EVENT_ID_E);
		}
		while(is_stopped);
	}while(in_measurement);
	if(!height_vector.empty()){
		ha.length = height_vector.size();
		ha.array = &height_vector[0];
		return ha;
	}
	ha.length = 1;
	height_vector.clear();
	height_vector.push_back(UNDEFINED);
	ha.array = &height_vector[0];
	return ha;
}

void Height_Measurement::get_height_difference(Height_sensor* height_sensor){
	int i;
	height_diff = 0;
	for(i = 0; i < INIT_MEASUREMENTS; i++){
		height_diff += height_sensor -> get_height();
	}
	height_diff = height_diff / INIT_MEASUREMENTS - INIT_HEIGHT;
	cout << "height_diff" << height_diff << endl;
}

uint32_t Height_Measurement::evaluate_puk_type(height_array ha_unfiltered){
	uint32_t type = UNDEFINED;

	// check little one and normal
	type = is_little_or_normal(ha_unfiltered);

	// check iron core and hole
	if(type == UNDEFINED){
		type = is_ic_or_hole(ha_unfiltered);
	}

	if(type == UNDEFINED){
		type = is_bit_code(ha_unfiltered);
	}
	cout << "type = " << type << endl;
	return type;
}

uint32_t Height_Measurement::is_bit_code(height_array ha_unfiltered){
	int counter = 0;
	uint32_t type = UNDEFINED;
	int indexer = 0;
	int i;
	struct height_array ha;
	type = 0;
	ha = filter_array(ha_unfiltered);
	if(ha.length != 0){
		double last_height = ha.array[0];
		// check for bit code
		for(i = 0; i < ha.length; i++){
			if(ha.array[i] == last_height && i != (ha.length - 1)){
				++counter;
			}else if(ha.array[i] != last_height || i == (ha.length - 1)){
					cout << counter << endl;
					if(indexer == 0){
						counter = (counter + 1) * 2;
					}
//					if(indexer == 1){
//						counter += 6;
//					}
					if(counter > 40){
						type |= 1 << indexer;
						++indexer;
					}else if(counter > 4){
						++indexer;
					}
					last_height = ha.array[i];
					counter = 1;
			}
		}
		cout << type << endl;
		if(type < MAX_PUK_VALUE){
			type >>= 4;
			type &= 0x1F;
			//check other side?
		}else{
			type = UNDEFINED;
		}
	}else{
		type = UNDEFINED;
	}
	return type;
}

uint32_t Height_Measurement::is_ic_or_hole(height_array ha_unfiltered){
	uint32_t type = UNDEFINED;
	int i;
	int y;
	int counter = 0;
	bool is_type = false;
	double temp_array[ha_unfiltered.length - CUT_OFF_FRONT];
	//init array
	for(i = 0; i < 15; i++){
		temp_array[i] = -1;
	}

	uint32_t new_length = 0;
	double last = -1;
	double mean = Height_Measurement::mean(ha_unfiltered);

	for(i = CUT_OFF_FRONT + EXTRA_CUT_OFF; i < ha_unfiltered.length - EXTRA_CUT_OFF; i++){
		if(((ha_unfiltered.array[i] - mean) / mean) < 0.25){
			if(ha_unfiltered.array[i] < NORMALE_UPPER - height_diff){
				if(last != 2){
					counter = 0;
				}
				++counter;
				last = 2;
				if(counter == MINIMAL_WIDTH){
					temp_array[new_length] = 2;
					++new_length;
				}
			}else if(ha_unfiltered.array[i] > GROOVE_LOWER - height_diff && ha_unfiltered.array[i] < GROOVE_UPPER - height_diff){
				if(last != 1){
					counter = 0;
				}
				++counter;
				last = 1;
				if(counter == MINIMAL_WIDTH){
					temp_array[new_length] = 1;
					++new_length;
				}
			}else if(ha_unfiltered.array[i] > HOLE_LOWER - height_diff && ha_unfiltered.array[i] < HOLE_UPPER - height_diff){
				if(last != 0){
					counter = 0;
				}
				++counter;
				last = 0;
				if(counter == MINIMAL_WIDTH){
					temp_array[new_length] = 0;
					++new_length;
				}
			}
		}
	}

	cout << new_length << endl;
	for(i = 0; i < new_length; i++){
		cout << temp_array[i] << ", ";
	}
	cout << endl;

	for(i = 0; i < PROFILES_LENGHT; i++){
		for(y = 0; y < PROFILE_LENGTH; y++){
			if(profiles[i][y] != temp_array[y]){
				break;
			}else if(temp_array[y] == (double) -1 && profiles[i][y] == (double) -1){
				is_type = true;
			}
		}
		if(is_type == true){
			type = i;
			break;
		}
	}
	if(type < SUM_IC_PROFILES){
		type = puk_iron_core;
		cout << "iron_core" << endl;
	}else if(type < SUM_IC_PROFILES + SUM_HOLE_PROFILES){
		type = puk_hole;
		cout << "puk_hole" << endl;
	}
	return type;
}

uint32_t Height_Measurement::is_little_or_normal(height_array ha_unfiltered){
	uint32_t type = UNDEFINED;
	int i;
	double mean = Height_Measurement::mean(ha_unfiltered);
	int new_length = 0;
	double mittle_value = 0;

	for(i = CUT_OFF_FRONT; i < ha_unfiltered.length; i++){
		if(((ha_unfiltered.array[i] - mean) / mean) < LO_NORMAL_FILTER_TOL){
			if((ha_unfiltered.length / 2) < i && mittle_value == 0){
				mittle_value = ha_unfiltered.array[i];
			}
			++new_length;
		}
	}
	if((new_length + CUT_OFF_FRONT) >= (ha_unfiltered.length * LO_NORMAL_CUT_OFF_TOL)){
		cout << mean << endl;
		if(mean < LO_NORMAL_UPPER_TOL && mittle_value > LO_NORMAL_LOWER_TOL){
			type = puk_litte_one;
			cout << "littleone" << endl;
		}else if(mean < 2600){
			type = puk_normal;
			cout << "normal" << endl;
		}
	}
	return type;
}

height_array Height_Measurement::filter_array(height_array ha){
	int i;
	double mean = Height_Measurement::mean(ha);
	int new_length = 0;
	double temp_array[ha.length - CUT_OFF_FRONT];
//	double* binary_array;
	double min = -1;
	double max = -1;

	cout << "mean: " << mean << endl;
	for(i = CUT_OFF_FRONT; i < ha.length; i++){
		if(((ha.array[i] - mean) / mean) < FILTER_TOL_BIN){
			temp_array[new_length] = ha.array[i];
			++new_length;
			if(min == -1){
				min = ha.array[i];
				max = ha.array[i];
			}
			if(min > ha.array[i]){
				min = ha.array[i];
			}
			if(max < ha.array[i]){
				max = ha.array[i];
			}
		}
	}

	if(new_length < (ha.length - CUT_OFF_FRONT) * CUT_OFF_TOL_BIN){
		ha.length = 0;
		return ha;
	}

	cout << "Array Length: " << new_length << endl;
	for(i = 0; i < new_length; i++){
		cout << temp_array[i] << ", ";
	}
	cout << endl;

	double binary_array[new_length];

	for(i = 0; i < new_length; i++){
		if(temp_array[i] < ((max + min) / 2)){
			binary_array[i] = 1;
		} else {
			binary_array[i] = 0;
		}
	}

	cout << "Array Length: " << new_length << endl;
	for(i = 0; i < new_length; i++){
		cout << binary_array[i] << ", ";
	}
	cout << endl;

	ha.length = new_length;
	ha.array = binary_array;
	ha.min = min;
	ha.max = max;
	return ha;
}

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
