/*
 * Switch_test.cpp
 *
 *  Created on: 13.12.2016
 *      Author: aby520
 */

#include "Switch_test.h"
#include "src/controller/switch_control/switch_control.h"

Switch_test::Switch_test() {
	// TODO Auto-generated constructor stub

}

Switch_test::~Switch_test() {
	// TODO Auto-generated destructor stub
}

void Switch_test::test() {
	Switch_control sc;
	sc.switch_open();
	sleep(3);
	sc.error_switch();
	sleep(3);
	sc.error_switch_ok();
	sleep(3);
	sc.timer_switch_out();
	sleep(3);
	sc.error_switch();
	sleep(3);
	sc.error_switch_ok();
}

