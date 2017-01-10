/*
*/
#ifndef HEIGHT_MEASUREMENT_H_
#define HEIGHT_MEASUREMENT_H_

#include "src/lib/HAWThread.h"
#include <stdint.h>
#include <vector>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include "src/lib/hal/height_sensor.h"
#include "lib/dispatcher/State.cpp"

using namespace thread;

struct height_array {
	uint32_t length;
	double* array;
	double max;
	double min;
};

class Height_Measurement: public HAWThread, public State {

	private:
        Height_Measurement& operator=(Height_Measurement& b);
        Height_Measurement(const Height_Measurement& b);

        virtual void execute(void*);
        virtual void shutdown();
		
        height_array get_height_array(void);
		void get_height_difference(Height_sensor* height_sensor);
		uint32_t evaluate_puk_type(height_array ha_unfiltered);
		uint32_t is_ic_or_hole(height_array ha_unfiltered);
		uint32_t is_little_or_normal(height_array ha_unfiltered);
		uint32_t is_bit_code(height_array ha_unfiltered);
		void  init_timer(void);
		height_array filter_array(height_array ha);
		double mean(height_array ha);
		
		static double profiles[27][15];

	public:
		Height_Measurement();
		static Height_Measurement* get_instance(){
			static Height_Measurement instance_;
			return &instance_;
		}
		
        ~Height_Measurement(){};
        static uint32_t get_type(void);
		static void set_motor_speed(uint32_t speed);

		virtual void SPEED_STOP();
		virtual void SPEED_NORMAL();
		virtual void SPEED_SLOW();
};
#endif
