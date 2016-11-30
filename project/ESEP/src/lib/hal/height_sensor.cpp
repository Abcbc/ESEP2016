/*
 * height_sensor.cpp
 *
 *  Created on: 25.10.2016
 *      Author: abr468
 */

#include "hal_component.h"
#include "lib/HWaccess.h"
#include <unistd.h>
#include <iomanip>
#include <stdint.h>
#include <pthread.h>
#include "lib/Lock.h"
#include "lib/HAWThread.h"
#include <cstdlib>
#include <iostream>
#include "height_sensor.h"

pthread_mutex_t Height_sensor::init_mtx = PTHREAD_MUTEX_INITIALIZER;
Height_sensor* Height_sensor::instance_ = NULL;
int Height_sensor::get_height(){
	  uint16_t height = 0;

	  // write opcode to start conversion
	  out8(BASE + START_CONVERSATION_OFFSET, CONVERSATION_CODE);

	  //wait for conversion to complete
	  usleep(SLEEP_TIME);

	  //read low bytes
	  height = in8(BASE + READ_LOW_OFFSET);

	  //read high bytes
	  height |= (uint16_t)in8(BASE + READ_HIGH_OFFSET) << 8;

	  // height sensor value is only 12 bit
	  height &= BITMASK_12BIT;

	  return height;
}

bool Height_sensor::is_metal_puk(void){
	return false;
}

bool Height_sensor::is_blank_puk(void){
	return false;
}

bool Height_sensor::is_hole_puk(void){
	return false;
}


Height_sensor* Height_sensor::get_instance(void){
	if(instance_ == NULL){
			pthread_mutex_lock(&init_mtx);
			if(instance_ == NULL){
				instance_ = new Height_sensor();
			}
			pthread_mutex_unlock(&init_mtx);
		}
	return instance_;
}

Height_sensor::Height_sensor(){
	START_CONVERSATION_OFFSET = 0x2;
	CONVERSATION_CODE = 		0x10;
	READ_HIGH_OFFSET = 			0x3;
	READ_LOW_OFFSET = 			0x2;
	BITMASK_12BIT = 			0x0FFF;
	SLEEP_TIME = 				10;
	BASE = 						PORT_AD;
}

Height_sensor::~Height_sensor(){
	pthread_mutex_destroy(&init_mtx);
}

