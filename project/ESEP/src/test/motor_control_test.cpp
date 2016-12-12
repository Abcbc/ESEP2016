/*
 * motor_control_test.cpp
 *
 *  Created on: 12.12.2016
 *      Author: aby520
 */
#include <unistd.h>
#include "motor_control_test.h"
#include "src/controller/motor_controler/motorcontroler.h"

Motor_Control_Test::Motor_Control_Test() {
	// TODO Auto-generated constructor stub

}

Motor_Control_Test::~Motor_Control_Test() {
	// TODO Auto-generated destructor stub
}

void Motor_Control_Test::test() {
	cout << "Motor Controler Test:" << endl;
	motorcontroler mc;
	mc.motor_normal();
	sleep(3);
	mc.motor_slow();
	sleep(3);
	mc.motor_stop();
	sleep(3);
	mc.motor_start();
	sleep(3);
	mc.time_measure_out();
	sleep(3);
	mc.stop_error();
	sleep(3);
	mc.motor_start();
	sleep(5);
	mc.start_error();
	sleep(3);
	mc.motor_stop();
}
