#include <stdint.h>
#include <array>
#inlclude <algorithm>
#include <cmath>

#include "height_measurement.h"

#define UNDEFINED 0
#define MAX_HEIGHT 2420
#define GROUND_HEIGHT 4035
#define MAX_LENGTH 600
#define MIN_LENGTH 5
#define SUM_IC_WM 3

void Height_Measurement::execute(void){
	uint32_t msg;
	struct height_array ha;
	Height_sensor* height_sensor = Height_sensor::get_instance();
	while(1){
		level = check_value(height_sensor -> get_height());
		if(level < GROUND_HEIGHT){
			ha = get_height_array();
			
			// !!!!!!--------!!!!!!!!!!
			// for profile measurement
				cout << "[ ";
				for(msg = 0; msg < ha.length; i++){
					cout << ha.array[i] << ", ";
				}
				cout << "]" << endl;
			// !!!!!!--------!!!!!!!!!!
			
			if(ha.length >= MIN_LENGTH){
				last_type = compare_arrays(ha);
				msg = (last_type != UNDEFINED ? INVALID_PUK_TYPE_ID_E : HM_DONE_EVENT_ID_E);
				sendpulsemsg(.., msg);
		} 
	}
}

void Height_Measurement::shutdown(void){
	// ignore
}

void  Height_Measurement::set_motor_speed(uint32_t speed){
	is_stopped = (speed == stopped ? true : false);
}

uint32_t Height_Measurement::get_type(void){
	return last_type;
}

struct height_array Height_Measurement::get_height_array(void){
	std::vector<uint32_t> height_vector;
	bool in_measurement = true;
	uint32_t length_counter;
	uint32_t level;
	struct height_array ha;
	do{
		level = height_sensor -> get_height();
		if(++length_counter >= MAX_LENGTH || level >= MAX_HEIGHT){
			ha.length = 1;
			ha.array = {INVALID_PUK_TYPE_ID_E};
			return ha;
		}else if(level == GROUND){
			in_measurement == false;
		}else{
			height_vector.push_back(level);
		}
		if(length_counter = MIN_LENGTH ){
			sendpulsemsg(.., HM_START_EVENT_ID_E);
		}
		while(is_stopped);
	}while(in_measurement);
	if(!height_vector.empty()){
		ha.length = height_vector.size();
		ha.array = &height_vector[0];
		return ha;
	}
	ha.length = 1;
	ha.array = {UNDEFINED};
	return ha;
}

uint32_t Height_Measurement::compare_arrays(struct height_array ha){
	uint32_t i;
	uint32_t abs_sum 
	if(ha.length < prototype_array.length){
		return UNDEFINED;
	}
	for(i = 0; i < prototype_array.length){
		sum = cmath.abs(ha.array[i] - prototype_array[i]);
		if(sum <= smallest_abserror){
			i = (i < SUM_IC_WM ? 0 : i);
			return i;
		}
	}
	return UNDEFINED
}
