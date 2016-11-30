/*
 * height_sensor_test.h
 *
 *  Created on: 14.11.2016
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

#ifndef HEIGHT_SENSOR_TEST_H_
#define HEIGHT_SENSOR_TEST_H_

class Height_Sensor_Test {
    public:
	Height_Sensor_Test(void);
    virtual ~Height_Sensor_Test();
    void test_height(void);

    private:
        Height_Sensor_Test(const Height_Sensor_Test& l);
        Height_Sensor_Test& operator=(Height_Sensor_Test& l);
};

#endif /* HEIGHT_SENSOR_TEST_H_ */
