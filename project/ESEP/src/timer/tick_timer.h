/*
 * timer.h
 *
 * The Tick_timer counts down a value in dependence of motor speed.
 * If a Timer runs out a pulse message is send: 
 * If the pulse message value is the timer id.
 *
 *  Created on: 01.12.2016
 *      Author: abl395
 */

#ifndef TICK_TIMER_H_
#define TICK_TIMER_H_

#include <stdint.h>
#include <pthread.h>
#include "lib/HAWThread.h"
#include "timer/timer_event_table.h"

using namespace thread;

const unsigned int BITMASK_TIMER_RUNOUT_EVENT = 0x8FF00;
const unsigned int BITMASK_ID = 0xFF;

class Tick_timer: public HAWThread {
	public:
		/*
		 * Start new timer.
		 * @param duration
		 * @param id
		 * unique timer id used to identify this timer.
		 * @return in case of a positive number everything worked as expected.
		 * If not you can find the error code in the Ir_handler description
		 */
		void start_timer(uint32_t duration, uint8_t id);

		/*
		 * Stops a timer if it exists.
		 * @param id
		 * @return value of intern_timer.duration
		 * Returns -1 if id does not exist.
		 */
		uint32_t stop_timer(uint8_t id);

		/*
		* Getter for intern_timer.duration.
		* @param id
		* @return value of intern_timer.duration
		* Returns -1 if id does not exist.
		*/
		uint32_t get_duration(uint8_t id);

		/*
		 * This Method return an instance of IR_Handler(Singelton)
		 * @return Return the pointer of the instance.
		 */
		static Tick_timer* get_instance(int cid);

		virtual ~Tick_timer();
	private:

		virtual void execute(void*);
		virtual void shutdown();

		static Tick_timer* instance_;
		static pthread_mutex_t init_mtx;
		static pthread_mutex_t vector_access_mtx;
		int cid;
		int con;
		int con_dispatcher;
		Motor* motor;
		
		Tick_timer();
		int find_timer(uint8_t id);

};

#endif /* TICK_TIMER_H_ */
