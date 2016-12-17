/*
*/
#ifndef HEIGHT_MEASUREMENT_H_
#define HEIGHT_MEASUREMENT_H_

#include "src/lib/HAWThread.h"
#include <stdint.h>
#include <vector>
#include <cstring>
#include <sys/neutrino.h>
#include "src/lib/hal/height_sensor.h"

using namespace thread;

struct PROFILES {
	int* iron_core;
	int* iron_core_wmf;
	int* iron_core_wmb;
	int* little_one;
	int* normal;
	int* hole;
	int* sunshine;
	int* black_beauty;
	int* white_innocence;
	int* deep_blue;
};

struct height_array {
	uint32_t length;
	uint32_t* array;
}

class Height_Measurement: public HAWThread {

	private:
        Height_Measurement& operator=(Height_Measurement& b);
        Height_Measurement(const Height_Measurement& b);

        virtual void execute(void*);
        virtual void shutdown();
		
        struct height_array get_height_array(void);
		uint32_t compare_arrays(struct height_array ha);
		
		static uint32_t last_type;
		struct PROFILES profiles;

	public:
		Height_Measurement(){
			last_type = 0;

			//TODO new height profile 
			int iron_core[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int iron_core_wmf[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int iron_core_wmb[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int little_one[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int normal[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int hole[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int sunshine[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int black_beauty[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int white_innocence[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int deep_blue[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			profiles.iron_core = iron_core;
			profiles.iron_core_wmf = iron_core_wmf;
			profiles.iron_core_wmb = iron_core_wmb;
			profiles.little_one = little_one;
			profiles.normal = normal;
			profiles.hole = hole;
			profiles.sunshine = sunshine;
			profiles.black_beauty = black_beauty;
			profiles.white_innocence = white_innocence;
			profiles.deep_blue = deep_blue;
		};
		
        ~Height_Measurement(){};
        int get_type(void);
		void set_motor_speed(uint32_t speed);

		static uint32_t const INVALID_PUK_TYPE_ID_E = 12345;
		static uint32_t const HM_START_EVENT_ID_E = 54321;
		static uint32_t const HM_DONE_EVENT_ID_E = 264864;
};
#endif
