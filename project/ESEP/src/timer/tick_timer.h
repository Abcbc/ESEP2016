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

using namespace thread;

const uint32_t MOTOR_FAST = 23;
const uint32_t  MOTOR_SLOW = 10;
const uint32_t  MOTOR_STOP = 0;

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
		void start_timer(uint32_t duration, uint32_t id);

		/*
		 * Stops a timer if it exists.
		 * @param id
		 * @return value of intern_timer.duration
		 * Returns -1 if id does not exist.
		 */
		uint32_t stop_timer(uint32_t id);

		/*
		* Getter for intern_timer.duration.
		* @param id
		* @return value of intern_timer.duration
		* Returns -1 if id does not exist.
		*/
		uint32_t get_duration(uint32_t id);

		/*
		 * set the motor speed.
		 * @param speed
		 * Only MOTOR_STOP, MOTOR_FAST or MOTOR_SLOW are allowed.
		 */
		void set_speed(uint32_t speed);

		/*
		 * Get the ID from the last send pulsemessage.
		 * @return ID from pulsemessage
		 */
		uint32_t get_id(void);

		/*
		 * This Method return an instance of Tick_timer(Singelton)
		 * @return Return the pointer of the instance.
		 */
		static Tick_timer* get_instance(int con);

		virtual ~Tick_timer();
	private:

		virtual void execute(void*);
		virtual void shutdown();

		static Tick_timer* instance_;
		static pthread_mutex_t init_mtx;
		static pthread_mutex_t vector_access_mtx;
		int cid;
		int con;
//		static int con_dispatcher;
		uint32_t step;
		
		Tick_timer(int con);
		int find_timer(uint32_t id);

};

#endif /* TICK_TIMER_H_ */
