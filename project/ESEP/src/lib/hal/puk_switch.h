/*
 * puk_switch.h
 *
 *  Created on: 19.10.2016
 *      Author: abr468
 */

#ifndef PUK_SWITCH_H_
#define PUK_SWITCH_H_
#include "hal_component.h"
#include <pthread.h>
class Puk_switch:Hal_component{

	public:
			void open();
			void close();
			static Puk_switch* get_instance();
	private:
			static pthread_mutex_t init_mtx;
			static Puk_switch* instance_;
			Puk_switch();
			~Puk_switch();
			uint16_t BASE;
			uint8_t OPEN_BIT;


};
#endif /* PUK_SWITCH_H_ */
