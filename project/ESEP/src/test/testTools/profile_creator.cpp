/*
 * profile_creator.cpp
 *
 *  Created on: 19.12.2016
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
#include "profile_creator.h"

void Profile_Creator::create_profile(){
	Height_sensor* height_sensor = Height_sensor::get_instance();
	int i;
	bool in_measurement = true;
	uint32_t typecounter = 0;
	uint32_t level;
	uint32_t array[550];
	Motor* motor = Motor::get_instance();
	motor -> go_slow();
	motor -> go_right();
	motor -> start();

	while(1){
		level = height_sensor -> get_height();
		if(level < 4035){
			for(i = 0; i < 8 ; i++){
				level = height_sensor -> get_height();
				cout << "....";
			}
			for(i = 0; i < 550 ; i++){
				level = height_sensor -> get_height();
				array[i] +=  level;
				cout << "....";
			}
			++typecounter;
		}
		if(typecounter == 4){
			cout << endl;
			for(i = 0; i < 550 ; i++){
				array[i] /=  4;
				cout << array[i] << ", ";
				array[i] = 0;
			}
			cout << endl;
			typecounter = 0;
		}

	}

}


