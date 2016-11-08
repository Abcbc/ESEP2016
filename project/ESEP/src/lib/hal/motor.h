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
			void start();
			void stop();
			void go_fast();
			void go_slow();
			void go_left();
			void go_right();

			bool is_slow();
			bool is_fast();
			bool is_stopped();
			bool is_running();

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
