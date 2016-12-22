#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>
#include "src/test/hal_test.h"
#include "src/test/height_sensor_test.h"
#include "src/test/ir_test.h"
#include "src/test/timer_test.h"
#include "src/test/testTools/distance_time.h"
#include "src/test/testTools/profile_creator.h"
#include "src/test/height_measurement_test.h"
#include "src/controller/motor_controler/motorcontroler.h"
#include "src/controller/puk_control/Puk_control.h"
#include "src/controller/event_table.h"
#include "src/controller/ir_handler.h"
#include "src/controller/switch_control/Switch_control.h"

int main(int argc, char *argv[]) {
	/* Zugriffsrechte von QNX fuer diesen Thread, auf die Hardware erbitten. */
	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		cout << "Can't get Hardware access, therefore can't do anything."
				<< endl;
	}

	// Test HAL
//	HAL_Test hal_test;
//	hal_test.test_actuators();

// Test height sensor
//	Height_Sensor_Test hs_test;
//	hs_test.test_height();

// Test Interrupt
//	IR_Test ir_test;
//	ir_test.test_ir();

// get distance/time
//	Distance_Time dt;
//	dt.measure_distance();

// Test tick_timer
//	Timer_test timer_test;
//	timer_test.test_timer();

// get profile
//	Profile_Creator creator;
//	creator.create_profile();

// Test Height_measurement
//	Height_Measurement_Test hm;
//	hm.test_measurement();

	Dispatcher* dis = Dispatcher::getInstance();
	dis->start(NULL);
	Ir_handler *ir = Ir_handler::get_instance();
	ir->connect(3);
	Motorcontroler* mc = Motorcontroler::get_instance();
	mc->start(NULL);
	Switch_control* sc = Switch_control::get_instance();
	Puk_control* pc = Puk_control::get_instance();
	pc->start(NULL);

	//	MsgSendPulse(3, -1, 5, LIGHT_BARRIER_ENTRY_CLOSE_E_ID);
	//	usleep(2000000); // seep 2000 ms
	//	MsgSendPulse(3, -1, 5, MOTOR_STOP_E_ID);
	//	std::cout << "Main finished" << std::endl;

	// Wait for threads
	dis->join();
	mc->join();
	pc->join();

}
