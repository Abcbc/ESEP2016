/*
 * height_measurement_test.cpp
 *
 *  Created on: 11.12.2016
 *      Author: abl395
 */

#include "height_measurement_test.h"
#include "src/lib/hal/motor.h"
#include "src/lib/hal/puk_switch.h"
#include <sys/time.h>

Motor* motor;
Height_Measurement* height_measurement;

static uint32_t const INVALID_PUK_TYPE_ID_E = 12345;
static uint32_t const HM_START_EVENT_ID_E = 54321;
static uint32_t const HM_DONE_EVENT_ID_E = 264864;

Height_Measurement_Test::Height_Measurement_Test() {
}


Height_Measurement_Test::~Height_Measurement_Test() {
	if(motor != NULL){
		motor -> stop();
	}
	if(height_measurement != NULL){
		delete height_measurement;
	}
}

void Height_Measurement_Test::test_measurement(){
	int isrChannel;
	int isrConnection;
	std::cout << "test height_measurement" << std::endl;
	height_measurement = new Height_Measurement();
	height_measurement -> start(NULL);
	
	motor = Motor::get_instance();
	motor -> go_slow();
	motor -> go_right();
	motor -> start();

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
		cout << "ThreadCtl() failed." << endl;
	}

	if (( isrChannel = ChannelCreate(0)) == -1){
	    cout << "ChannelCreate() failed." << endl;
	}

	if ((isrConnection = ConnectAttach(0, 0, isrChannel, 0, 0)) == -1){
	    cout << "ConnectAttach() failed." << endl;
	}

	struct _pulse pulse;
	
	while(1){
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
		if(pulse.value.sival_int == INVALID_PUK_TYPE_ID_E){
			cout << "INVALID_PUK_TYPE_ID_E" << endl;
		}else if(pulse.value.sival_int == HM_START_EVENT_ID_E){
			cout << "start height measurement" << endl;
		}else if(pulse.value.sival_int == HM_DONE_EVENT_ID_E){
			switch(height_measurement -> get_type()){
				case 0 :
					cout << "iron_core" << endl;
					break;
				case 3 :
					cout << "little_one" << endl;
					break;
				case 4 :
					cout << "normal" << endl;
					break;
				case 5 :
					cout << "hole" << endl;
					break;
				case 6 :
					cout << "sunshine" << endl;
					break;
				case 7 :
					cout << "black_beauty" << endl;
					break;
				case 8 :
					cout << "white_innocence" << endl;
					break;
				case 9 :
					cout << "deep_blue" << endl;
					break;
				default :
					cout << "wrong type id: " << height_measurement -> get_type() << endl;
			}
		}
	}
}

