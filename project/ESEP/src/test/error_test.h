/*
 * error_test.h
 *
 *  Created on: 14.11.2016
 *      Author: abl395
 */

#ifndef ERROR_TEST_H_
#define ERROR_TEST_H_

class Error_Test {
public:
	Error_Test(void);
	virtual ~Error_Test();
	void test_error_controller(void);

private:
	Error_Test(const Error_Test& l);
	Error_Test& operator=(Error_Test& l);
};

#endif /* HAL_TEST_H_ */
