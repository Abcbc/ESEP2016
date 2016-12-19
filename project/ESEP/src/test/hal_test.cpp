/*
 * HAL_test.cpp
 *
 * Test class for HAL components.
 * Only a few functions get tested automatically.
 * After the test is started a test sequence is running.
 * The expected behavior is printed in the console.
 * The user has "DELAY_TIME" s to check the behavior.
 *
 * This test does not prove that the HAL is free from failures.
 *
 *  Created on: 31.10.2016
 *      Author: abl395
 */


#include "hal_test.h"

#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>

#include "src/lib/hal/hal_component.h"
#include "src/lib/hal/motor.h"
#include "src/lib/hal/signal_light.h"
#include "src/lib/hal/puk_switch.h"
#include "src/lib/hal/height_sensor.h"

#define is_True(result, message) if(result != true){std::cout << message << std::endl;}
#define is_False(result, message) if(result == true){std::cout << message << std::endl;}
#define is_not_Null(result, message) if(result == NULL){std::cout << message << std::endl;}

#define HEIGHT_GLT 4070 //Ground lower tolerance
#define HEIGHT_GUT 4050 //Ground upper tolerance
#define HEIGHT_PLT 2630 //Puk lower tolerance
#define HEIGHT_PUT 2608 //Puk upper tolerance
#define DELAY_TIME 2

void HAL_Test::test_actuators(){
	std::cout << "test sequence for the HAL" << std::endl;
	sleep(DELAY_TIME);

	// test puk switch
	std::cout << "test puk_switch" << std::endl;
	Puk_switch* component = Puk_switch::get_instance();
		is_not_Null(component, "failed test: switch component is null");
	std::cout << "open switch" << std::endl;
	component -> open();
	sleep(DELAY_TIME);
	std::cout << "release/close switch" << std::endl;
	component -> close();
	sleep(DELAY_TIME);

	//test motor
	std::cout << "test motor" << std::endl;
	Motor* component1 = Motor::get_instance();
	is_not_Null(component1, "failed test: motor component is null");
	std::cout << "run slow right" << std::endl;
	component1 -> go_slow();
	component1 -> go_right();
	component1 -> start();
	sleep(DELAY_TIME);
	std::cout << "run slow left" << std::endl;
	component1 -> go_left();
	sleep(DELAY_TIME);
	std::cout << "run fast left" << std::endl;
	component1 -> go_fast();
	sleep(DELAY_TIME);
	std::cout << "run fast right" << std::endl;
	component1 -> go_right();
	sleep(DELAY_TIME);
	std::cout << "stop motor" << std::endl;
	component1 -> stop();
	sleep(DELAY_TIME);

	//test lights
	std::cout << "test signal_light" << std::endl;
	Signal_light* component2 = Signal_light::get_instance();
	is_not_Null(component2, "failed test: signal_light component is null");
	std::cout << "test green" << std::endl;
	component2 -> set_light(green);
	sleep(DELAY_TIME);
	std::cout << "test yellow" << std::endl;
	component2 -> set_light(yellow);
	sleep(DELAY_TIME);
	std::cout << "test red" << std::endl;
	component2 -> set_light(red);
	sleep(DELAY_TIME);
	std::cout << "test clear" << std::endl;
	component2 -> clear_all_lights();
	sleep(DELAY_TIME);
	std::cout << "test all" << std::endl;
	component2 -> set_all_lights();
	sleep(DELAY_TIME);
	std::cout << "test clear red" << std::endl;
	component2 -> clear_light(red);
	sleep(DELAY_TIME);
	std::cout << "test set all clear yellow" << std::endl;
	component2 -> set_all_lights();
	component2 -> clear_light(yellow);
	sleep(DELAY_TIME);
	std::cout << "test set all clear green" << std::endl;
	component2 -> set_all_lights();
	component2 -> clear_light(green);
	sleep(DELAY_TIME);
	std::cout << "test blink yellow" << std::endl;
	component2 -> blink(yellow, 800000);
	sleep(DELAY_TIME);
	std::cout << "test blink yellow faster" << std::endl;
	component2 -> blink(yellow, 400000);
	sleep(DELAY_TIME);
	std::cout << "test stop_blink yellow" << std::endl;
	component2 -> stop_blink(yellow);
	sleep(DELAY_TIME);
	std::cout << "test blink green&red" << std::endl;
	component2 -> blink(green, 400000);
	component2 -> blink(red, 400000);
	sleep(DELAY_TIME);
	std::cout << "test stop_blink green&red" << std::endl;
	component2 -> stop_blink(green);
	component2 -> stop_blink(red);
	sleep(DELAY_TIME);
	//TODO missing functions: errors

	//test height_sensor
//	std::cout << "test height_sensor" << std::endl;
//	Height_sensor* component3 = Height_sensor::get_instance();
//	uint16_t height;
//	height = component3 -> get_height();
//	std::cout << "height1: " << height << std::endl;
//	sleep(DELAY_TIME);
//	height = component3 -> get_height();
//	std::cout << "height2: " << height << std::endl;
//
//	is_not_Null(component3, "failed test: height_sensor component is null");
//	std::cout << "test ground level" << std::endl;
//	height = component3 -> get_height();
//	std::cout << "height: " << height << std::endl;
//	is_True((height <= HEIGHT_GLT) && (height >= HEIGHT_GUT), "failed test: wrong ground height");
//	sleep(DELAY_TIME);
//	std::cout << "put normal_puk into height sensor" << std::endl;
//	sleep(DELAY_TIME * 2);
//	std::cout << "test normal_puk height" << std::endl;
//	height = component3 -> get_height();
//	std::cout << "height1: " << height << std::endl;
//	is_True((height <= HEIGHT_PLT) && (height >= HEIGHT_PUT), "failed test: wrong puk height");
//	sleep(DELAY_TIME);
}

HAL_Test::HAL_Test(){
	// Ignore
}

HAL_Test::~HAL_Test(){
	// Ignore
}
