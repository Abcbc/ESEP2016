#ifndef SIGNAL_LIGHT_H_
#define SIGNAL_LIGHT_H_
#include "hal_component.h"
#include <pthread.h>
enum Color{
	green,
	yellow,
	red
};


/**
 *
 */
class Signal_light:Hal_component{
	public:
			void set_light(Color c);
			void set_all_lights(void);
			void clear_light(Color c);
			void clear_all_lights(void);
			void togle_light(Color c);

			void blink(Color c, uint32_t hz);
			void stop_blink(Color c);

			void warning_on();
			void warning_off();

			void active_on();
			void active_off();

			void ack_error_on();
			void ack_error_off();

			void unack_error_on();
			void unack_error_off();

			static Signal_light* get_instance();
	private:
		Signal_light();
		~Signal_light();
		uint8_t color_bit(Color c);

		static Signal_light* instance_;
		static pthread_mutex_t init_mtx;

		uint16_t BASE;
		uint8_t GREEN;
		uint8_t YELLOW;
		uint8_t RED;


};
#endif /* SIGNAL_LIGHT_H_ */
