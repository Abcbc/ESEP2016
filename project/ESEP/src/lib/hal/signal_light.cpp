/*
 * signal_light.cpp
 *
 *  Created on: 19.10.2016
 *      Author: abr468
 */
#include <stdint.h>
#include "signal_light.h"
pthread_mutex_t Signal_light::init_mtx = PTHREAD_MUTEX_INITIALIZER;
Signal_light* Signal_light::instance_ = NULL;

Signal_light::Signal_light(){
	BASE 	= PORT_A;
	GREEN 	= 5;
	YELLOW 	= 6;
	RED 	= 7;
}
Signal_light::~Signal_light(){
	pthread_mutex_destroy(&init_mtx);
}


void Signal_light::set_light(Color c){
	set_bit(BASE,color_bit(c));
}
void Signal_light::set_all_lights(){
	set_light(red);
	set_light(yellow);
	set_light(green);
}
void Signal_light::clear_light(Color c){
	clear_bit(BASE,color_bit(c));
}
void Signal_light::clear_all_lights(void){
	clear_light(red);
	clear_light(yellow);
	clear_light(green);
}
void Signal_light::blink_fast(Color c){

}

void Signal_light::blink_slow(Color c){

}

void Signal_light::warning_on(){

}
void Signal_light::warning_off(){

}

void Signal_light::active_on(){

}
void Signal_light::active_off(){

}

void Signal_light::ack_error_on(){

}
void Signal_light::ack_error_off(){

}

void Signal_light::unack_error_on(){

}
void Signal_light::unack_error_off(){

}

uint8_t Signal_light::color_bit(Color c){
	switch(c){
		case green: return GREEN;
		case yellow: return YELLOW;
		case red: return RED;
	}
	return red;
}

Signal_light* Signal_light::get_instance(){
	if(instance_ == NULL){
			pthread_mutex_lock(&init_mtx);
			if(instance_ == NULL){
				instance_ = new Signal_light();
			}
			pthread_mutex_unlock(&init_mtx);
		}
		return instance_;
}
