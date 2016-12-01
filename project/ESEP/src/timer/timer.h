/*
 * timer.h
 *
 *  Created on: 01.12.2016
 *      Author: abl395
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <pthread.h>
#include "lib/HAWThread.h"

using namespace thread;

class Timer: public HAWThread {
	public:
		/*
		 * Start new timer.
		 * @param duration
		 * @param id
		 * unique timer id used to identify this timer.
		 * @return in case of a positive number everything worked as expected.
		 * If not you can find the error code in the Ir_handler description
		 */
		void start_timer(uint16_t duration, uint16_t id);

		/*
		 * Stops a timer if it exists.
		 * @param id
		 * @return value of intern_timer.duration
		 * Returns -1 if id does not exist.
		 */
		uint16_t stop_timer(uint16_t id);

		/*
		* Getter for intern_timer.duration.
		* @param id
		* @return value of intern_timer.duration
		* Returns -1 if id does not exist.
		*/
		uint16_t get_duration(uint16_t id);

		/*
		 * This Method return an instance of IR_Handler(Singelton)
		 * @return Return the pointer of the instance.
		 */
		static Timer* get_instance();

		virtual ~Timer();
	private:

		virtual void execute(void*);
		virtual void shutdown();

		static Timer* instance_;
		static pthread_mutex_t init_mtx;
		Timer();
		int find_timer(uint16_t id);

};

#endif /* TIMER_H_ */
