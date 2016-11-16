/*
 * height_sensor_test.cpp
 *
 *  Created on: 14.11.2016
 *      Author: abl395
 */

#include "height_sensor_test.h"
#include "src/lib/hal/motor.h"
#include "src/lib/hal/puk_switch.h"
#include <sys/time.h>

#define SUM_VALUES 400

Height_Sensor_Test::Height_Sensor_Test() {
}


Height_Sensor_Test::~Height_Sensor_Test() {
}

void Height_Sensor_Test::test_height(){
	std::cout << "test height_sensor" << std::endl;
	uint16_t* height_array = new uint16_t[SUM_VALUES];
	Height_sensor* component = Height_sensor::get_instance();
	Motor* component1 = Motor::get_instance();
	Puk_switch* component2 = Puk_switch::get_instance();
	component1 -> go_slow();
	component1 -> go_right();
	component1 -> start();

	struct timeval tp;
	gettimeofday(&tp, NULL);
	uint64_t start_time_ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	int i;
	for(i = 0; i < SUM_VALUES; i++){
		height_array[i] = component -> get_height();
		if(i == 100){
			//component2 -> open();
		}
		usleep(5);
	}

	gettimeofday(&tp, NULL);
	uint64_t end_time_ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	component2 -> close();

	for(i = 0; i < SUM_VALUES; i++){
		std::cout << height_array[i] << ",";
	}
	std::cout << std::endl;

	component1 -> stop();

	std::cout << "time: "<< end_time_ms - start_time_ms << " ms" << std::endl;
}

