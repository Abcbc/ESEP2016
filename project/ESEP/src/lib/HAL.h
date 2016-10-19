#ifndef HAL_H
#define HAL_H

#include <HWaccess.h>
#include <stdint.h>

class hal {
	enum Color {
		green, yellow, red
	};

public:

	hal();
	~hal();

	void set_light(Color c) {
		set_bit(color_bit(c));
	};
	void clear_light(Color c) {
		clear_bit(color_bit(c));
	};
	void togle_light(Color c) {
//				out8(BASE, BASE ^ (1 << bit));
	};

	void start() {
		set_bit(get_cur_direction());
		clear_bit(STOP_BIT);
	};
	u_int8_t get_cur_direction() {
		return 0;
	};
	void stop()  {
		set_bit(STOP_BIT);
	};
	void fast() {
		clear_bit(SLOW_BIT);
	};
	void slow() {
		set_bit(SLOW_BIT);
	};
	void go_left() {
		clear_set(RIGHT_BIT, LEFT_BIT);
	};
	void go_right() {
		clear_set(LEFT_BIT, RIGHT_BIT);
	};

	bool is_slow();
	bool is_fast();
	bool is_stopped();
	bool is_running();
	bool is_;
	//				Motor& operation=(const Motor& m) {
	//
	//				};
	const uint16_t BASE = 0x302;
	const uint8_t GREEN = 5;
	const uint8_t YELLOW = 6;
	const uint8_t RED = 7;
	const uint8_t STOP_BIT = 3;
	const uint8_t SLOW_BIT = 2;
	const uint8_t LEFT_BIT = 1;
	const uint8_t RIGHT_BIT = 0;

		uint8_t cur_direction;			//0 = left 1 = right

	void set_bit(uint8_t bit) {
		out8(BASE, BASE | (1 << bit));
	};

	void clear_bit(uint8_t bit){
		out8(BASE, BASE & ~(1 << bit));
	};

	uint8_t color_bit(Color c) {
		switch (c) {
		case green:
			return GREEN;
		case yellow:
			return YELLOW;
		case red:
			return RED;
		}
	};


};
#endif
