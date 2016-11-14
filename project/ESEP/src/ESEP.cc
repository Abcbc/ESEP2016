#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>
#include "src/test/hal_test.h"
#include "src/test/height_sensor_test.h"

int main(int argc, char *argv[]) {
	// Test HAL
//	HAL_Test hal();
//	hal.test_actuators();
//	hal.test_sensors();

	// Test height sensor
	Height_Sensor_Test* hs_test = new Height_Sensor_Test();
	hs_test -> test_height();
}
