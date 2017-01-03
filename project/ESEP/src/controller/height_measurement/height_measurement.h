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

using namespace thread;

//struct PROFILES {
//	int* iron_core;
//	int* iron_core_wmf;
//	int* iron_core_wmb;
//	int* little_one;
//	int* normal;
//	int* hole;
//	int* sunshine;
//	int* black_beauty;
//	int* white_innocence;
//	int* deep_blue;
//};

struct height_array {
	uint32_t length;
	double* array;
	double max;
	double min;
};

class Height_Measurement: public HAWThread {

	private:
        Height_Measurement& operator=(Height_Measurement& b);
        Height_Measurement(const Height_Measurement& b);

        virtual void execute(void*);
        virtual void shutdown();
		
        height_array get_height_array(void);
//		uint32_t compare_arrays(height_array ha);
		void get_height_difference(Height_sensor* height_sensor);
		uint32_t evaluate_puk_type(height_array ha_unfiltered);
		void  init_timer(void);
		height_array filter_array(height_array ha);
		double mean(height_array ha);
		
//		static uint32_t profiles[10][450];

	public:
		Height_Measurement();
		static Height_Measurement* get_instance(){
			static Height_Measurement instance_;
			return &instance_;
		}
		
        ~Height_Measurement(){};
        static uint32_t get_type(void);
		static void set_motor_speed(uint32_t speed);
};
#endif
