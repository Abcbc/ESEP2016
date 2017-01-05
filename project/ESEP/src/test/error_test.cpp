/*
 * error_test.cpp
 *
 *  Created on: 05.01.2017
 *      Author: abw181
 */


#include "error_test.h"

#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>

#include "src/lib/hal/hal_component.h"
#include "src/lib/hal/motor.h"
#include "src/lib/hal/signal_light.h"


void Error_Test::test_error_controller(void) {
	Signal_light* signal_light = Signal_light::get_instance();
	// Init Signal light
	signal_light->clear_all_lights();
	signal_light->set_light(green);
	//
	MsgSendPulse(3, -1, 5, MOTOR_NORMAL_E_ID);
	// Wait some time
	sleep(4);
    // Test Lost Puk
	cerr << "Test Lost Puk \n";
	MsgSendPulse(3, -1, 5, ERR_LOST_PUK_E_ID);
	sleep(4);
	// Test Acked
	cerr << "Test Acked Lost Puk \n";
	MsgSendPulse(3, -1, 5, BUTTON_RESET_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_START_E_ID);
	sleep(2);
	// Test ERR_TO_MANY_PUK
	cerr << "Test ERR_TO_MANY_PUK \n";
	MsgSendPulse(3, -1, 5, ERR_TO_MANY_PUK_E_ID);
	sleep(4);
	MsgSendPulse(3, -1, 5, BUTTON_RESET_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_START_E_ID);
	sleep(2);
	// Test ERR_SLIDE_FULL
	cerr << "Test ERR_SLIDE_FULL \n";
	MsgSendPulse(3, -1, 5, ERR_SLIDE_FULL_E_ID);
	sleep(4);
	MsgSendPulse(3, -1, 5, BUTTON_RESET_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_START_E_ID);
	sleep(2);
	// Test UNDEFINED_PUK
    cerr << "Test UNDEFINED_PUK \n";
	MsgSendPulse(3, -1, 5, ERR_UNDEFINED_PUK_E_ID);
	sleep(4);
	MsgSendPulse(3, -1, 5, BUTTON_RESET_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_START_E_ID);
	sleep(2);
	// Test RDY_TAKING_E_ID
    cerr << "Test RDY_TAKING_E_ID \n";
	MsgSendPulse(3, -1, 5, RDY_TAKING_E_ID);
	sleep(4);
	MsgSendPulse(3, -1, 5, BUTTON_RESET_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_START_E_ID);
	sleep(2);
	// Test ESTOP_THIS_E_ID
	cerr << "Test ESTOP_THIS_E_ID \n";
	MsgSendPulse(3, -1, 5, ESTOP_THIS_E_ID);
	sleep(4);
	MsgSendPulse(3, -1, 5, ESTOP_OK_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_RESET_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_START_E_ID);
	// Test UNDEFINED_PUK
    cerr << "Test UNDEFINED_PUK \n";
	MsgSendPulse(3, -1, 5, ERR_UNDEFINED_PUK_E_ID);
	sleep(4);
	MsgSendPulse(3, -1, 5, BUTTON_RESET_E_ID);
	sleep(2);
	MsgSendPulse(3, -1, 5, BUTTON_START_E_ID);
	sleep(2);
}

Error_Test::Error_Test() {

}

Error_Test::~Error_Test() {

}
