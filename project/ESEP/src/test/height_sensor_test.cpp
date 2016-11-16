/*
 * height_sensor_test.cpp
 *
 *  Created on: 14.11.2016
 *      Author: abl395
 */

#include "height_sensor_test.h"
#include "src/lib/hal/motor.h"
#include <sys/time.h>

Height_Sensor_Test::Height_Sensor_Test() {
}


Height_Sensor_Test::~Height_Sensor_Test() {
}

void Height_Sensor_Test::test_height(){
	std::cout << "test height_sensor" << std::endl;
	uint16_t* height_array = new uint16_t[400];
	Height_sensor* component = Height_sensor::get_instance();

	Motor* component1 = Motor::get_instance();
	component1 -> go_slow();
	component1 -> go_right();
	component1 -> start();

	struct timeval tp;
	gettimeofday(&tp, NULL);
	uint64_t start_time_ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	int i;
	for(i = 0; i < 400; i++){
		height_array[i] = component -> get_height();
		std::cout << height_array[i] << ",";
		usleep(5);
	}

	gettimeofday(&tp, NULL);
	uint64_t end_time_ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	component1 -> stop();

	std::cout << "ms need to get 200 values: "<< end_time_ms - start_time_ms << std::endl;
}

