/*
 * hal_component.h
 *
 *  Created on: 19.10.2016
 *      Author: abr468
 */

#ifndef HAL_COMPONENT_H_
#define HAL_COMPONENT_H_
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#define PORT_A 			0x300
#define PORT_B 			0x301
#define PORT_C 			0x302
#define PORT_AD 		0x320
#define CONTROL 		0x303
#define CTR_SETTINGS 	0b10001010

class Hal_component{
	protected:
		void set_bit(uint16_t base,uint8_t bit);
		void clear_bit(uint16_t base,uint8_t bit);
		void clear_set(uint16_t base,uint8_t c, uint8_t s);
		bool is_set(uint16_t base, uint8_t bit);
		Hal_component();
	private:

		static pthread_mutex_t init_mtx;

};


#endif /* HAL_COMPONENT_H_ */
