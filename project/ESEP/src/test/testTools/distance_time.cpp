/*
 * distance_time.c
 *
 *  Created on: 01.12.2016
 *      Author: abl395
 */

#include "ir_test.h"
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>
#include "src/controller/event_table.h"
#include "src/controller/ir_handler.h"
#include <bitset>
#include "src/test/testTools/distance_time.h"
#include "src/lib/hal/hal_component.h"
#include "src/lib/hal/motor.h"
#include "src/lib/hal/signal_light.h"
#include "src/lib/hal/puk_switch.h"
#include "src/lib/hal/height_sensor.h"
#include <sys/time.h>

int isrId_dt = 0;
int isrChannel_dt = 0;
int isrConnection_dt = 0;

Distance_Time::Distance_Time(){
	Ir_handler *ir = Ir_handler::get_instance();

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
		cout << "ThreadCtl() failed." << endl;
	}

	if (( isrChannel_dt = ChannelCreate(0)) == -1){
		cout << "ChannelCreate() failed." << endl;
	}

	if ((isrConnection_dt = ConnectAttach(0, 0, isrChannel_dt, 0, 0)) == -1){
	    cout << "ConnectAttach() failed." << endl;
	}
	ir->connect(isrConnection_dt);
}

void Distance_Time::measure_distance(){
	struct _pulse pulse;

	struct timeval tp;
	Motor* motor = Motor::get_instance();
	motor -> go_right();
	motor -> go_slow();
	motor -> stop();

	string speed = "slow: ";
	for(int i = 0; i < 8; i++){
		if(i == 4){
			motor -> go_fast();
			speed = "fast: ";
		}
		cout << "put puk on entry of system then press the start button" << endl;
		do{
			MsgReceivePulse(isrChannel_dt,&pulse,sizeof(pulse),NULL);
		}while(pulse.value.sival_int != BUTTON_START_PRESSED_E_ID);

		gettimeofday(&tp, NULL);
		uint64_t start_time_ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

		motor -> start();

		do{
			MsgReceivePulse(isrChannel_dt,&pulse,sizeof(pulse),NULL);
		}while(pulse.value.sival_int != LIGHT_BARRIER_HEIGHT_MEASUREMENT);

		gettimeofday(&tp, NULL);
		uint64_t stop_time_ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

		motor -> stop();

		cout << speed << stop_time_ms - start_time_ms << "ms" << endl;
	}


}

