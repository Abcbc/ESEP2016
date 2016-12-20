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

int main(int argc, char *argv[]) {
    /* Zugriffsrechte von QNX fuer diesen Thread, auf die Hardware erbitten. */
    if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        cout << "Can't get Hardware access, therefore can't do anything." << endl;
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
	Height_Measurement_Test hm;
	hm.test_measurement();
}
