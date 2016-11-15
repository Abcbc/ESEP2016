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
#include <stdint.h>
/**
 * 
 */
class Puk_switch:Hal_component{

	public:	
			/*
			 * Opens the Puk-Switch
			 */
			void open();
			/*
			 * Closes the Puk-Switch
			 */
			void close();
			
			/*
			 * Returns the pointer of the instance
			 * @return 
			 */
			static Puk_switch* get_instance();
			~Puk_switch();
	private:
			static pthread_mutex_t init_mtx;
			static Puk_switch* instance_;
			Puk_switch();
			uint16_t BASE;
			uint8_t OPEN_BIT;


};
#endif /* PUK_SWITCH_H_ */
