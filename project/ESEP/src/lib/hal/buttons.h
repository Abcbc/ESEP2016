/*
 * buttons.h
 *
 *  Created on: 25.10.2016
 *      Author: abr468
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

/**
 *
 */
enum Button{
	start,
	reset,
	stop
};

/**
 *
 */
class Buttons:Hal_component{
	public:
		bool is_pressed(Button btn);
		void button_light_on(Button btn);
		void button_light_off(Button btn);
	private:
		uint16_t BASE;

		uint8_t button_bit(Button btn);

};

#endif /* BUTTONS_H_ */
