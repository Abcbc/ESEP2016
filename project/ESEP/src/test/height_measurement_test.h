/*
 * height_measurement_test.h
 *
 *  Created on: 11.12.2016
 *      Author: abl395
 */

#include "hal_component.h"
#include "lib/HWaccess.h"
#include <unistd.h>
#include <iomanip>
#include <stdint.h>
#include <pthread.h>
#include "lib/Lock.h"
#include "lib/HAWThread.h"
#include <cstdlib>
#include <iostream>
#include "src/lib/hal/height_sensor.h"
#include "src/controller/height_measurement/height_measurement.h"

#ifndef HEIGHT_MEASUREMENT_TEST_H_
#define HEIGHT_MEASUREMENT_TEST_H_

class Height_Measurement_Test {
    public:
	Height_Measurement_Test(void);
    virtual ~Height_Measurement_Test();
    void test_measurement(void);

    private:
        Height_Measurement_Test(const Height_Measurement_Test& l);
        Height_Measurement_Test& operator=(Height_Measurement_Test& l);
};

#endif /* HEIGHT_SENSOR_TEST_H_ */
