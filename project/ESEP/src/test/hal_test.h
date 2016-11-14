/*
 * hal_test.h
 *
 *  Created on: 14.11.2016
 *      Author: abl395
 */

#ifndef HAL_TEST_H_
#define HAL_TEST_H_

class HAL_Test {
    public:
        HAL_Test(void);
        virtual ~HAL_Test();
        void test_sensors(void);
        void test_actuators(void);

    private:
        HAL_Test(const HAL_Test& l);
        HAL_Test& operator=(HAL_Test& l);
};

#endif /* HAL_TEST_H_ */
