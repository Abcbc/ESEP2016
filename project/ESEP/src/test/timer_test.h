/*
 * timer_test.h
 * 
 * Test sequenz for the Timer.
 *
 *  Created on: 02.12.2016
 *      Author: abl395
 */

#ifndef TIMER_TEST_H_
#define TIMER_TEST_H_

class Timer_test {
	public:
		/*
		* starts the Test Sequenz
		*/
		void test_timer(void);

		~Timer_test();
		Timer_test();
	private:
		int cid;
		int con;
		struct _pulse pulse;
		Timer_test(const Timer_test& l);
		Timer_test& operator=(Timer_test& l);
};


#endif /* TIMER_TEST_H_ */
