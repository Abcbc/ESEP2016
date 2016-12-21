#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include "src/test/hal_test.h"
#include "src/test/height_sensor_test.h"
#include "src/test/ir_test.h"
#include "src/test/timer_test.h"
#include "src/test/testTools/distance_time.h"
#include "src/controller/Puk_control/Puk_control.h"
#include "src/controller/Puk_control/puk_fsm.h"
#include "src/controller/motor_controler/motorcontroler.h"
#include "src/controller/event_table.h"
#include "src/controller/ir_handler.h"

int main(int argc, char *argv[]) {
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



	Dispatcher* dis = Dispatcher::getInstance();
	dis->start(NULL);
	Ir_handler *ir = Ir_handler::get_instance();
	ir->connect(3);
	Motorcontroler* mc = Motorcontroler::get_instance();
	mc->start(NULL);
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
