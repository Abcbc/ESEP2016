/*
 * motor.h
 *
 *  Created on: 19.10.2016
 *      Author: abr468
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "hal_component.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

class Motor:Hal_component{
	public:
			
			/*
			 * Starts the asembly line.
			 */
			void start();
			/*
			 * Stops the asembly line.
			 */
			void stop();
			/*
			 * Sets the asembly line on fast.
			 */
			void go_fast();
			/*
			 * Sets the asembly line on slow.
			 */
			void go_slow();
			/*
			 * Sets the asembly lines running direction on left.
			 */
			void go_left();
			/*
			 * Sets the asembly lines running direction on right.
			 */
			void go_right();
			
			/*
			 * Returns if the asembly line is slow.
			 */
			bool is_slow();
			/*
			 * Returns if the asembly line is fast.
			 */
			bool is_fast();
			/*
			 * Returns if the asembly line is stopped.
			 */
			bool is_stopped();
			/*
			 * Returns if the asembly line is running.
			 */
			bool is_running();
			
			/*
			 * Returns the pointer of the instance
			 * @return 
			 */
			static Motor* get_instance();
			virtual ~Motor();
	private:
			Motor();

			uint8_t get_cur_direction();

			static Motor* instance_;
			static pthread_mutex_t init_mtx;
			uint8_t cur_direction;
			uint16_t BASE;
			uint8_t STOP_BIT;
			uint8_t SLOW_BIT;
			uint8_t LEFT_BIT;
			uint8_t RIGHT_BIT;
};




#endif /* MOTOR_H_ */
