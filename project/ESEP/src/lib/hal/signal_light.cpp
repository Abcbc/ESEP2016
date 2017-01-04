/*
 * signal_light.cpp
 *
 *  Created on: 19.10.2016
 *      Author: abr468
 *  last Changed on 07.11. by abl395
 */
#include <stdint.h>
#include "signal_light.h"
#include "blink_green.h"
#include "blink_yellow.h"
#include "blink_red.h"

pthread_mutex_t Signal_light::init_mtx = PTHREAD_MUTEX_INITIALIZER;
Signal_light* Signal_light::instance_ = NULL;

struct blink_threads{
	Blink_green* green_thread;
	uint32_t green_hz;
	Blink_yellow* yellow_thread;
	uint32_t yellow_hz;
	Blink_red* red_thread;
	uint32_t red_hz;
};

struct blink_threads bl_threads;

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
void Signal_light::blink(Color c, uint32_t hz){
	switch(c){
		case green:
			if(bl_threads.green_thread != NULL){
				stop_blink(green);
			}
			if(bl_threads.green_thread == NULL){
				bl_threads.green_thread = new Blink_green(hz);
				bl_threads.green_thread -> start(NULL);
				bl_threads.green_hz = hz;
			}
			break;
		case yellow:
			if(bl_threads.green_thread != NULL){
				stop_blink(yellow);
			}
			if(bl_threads.yellow_thread == NULL){
				bl_threads.yellow_thread = new Blink_yellow(hz);
				bl_threads.yellow_thread -> start(NULL);
				bl_threads.yellow_hz = hz;
			}
			break;
		case red:
			if(bl_threads.green_thread != NULL){
				stop_blink(red);
			}
			if(bl_threads.red_thread == NULL || hz != bl_threads.red_hz){
				bl_threads.red_thread = new Blink_red(hz);
				bl_threads.red_thread -> start(NULL);
				bl_threads.red_hz = hz;
			}
			break;
	}
}
void Signal_light::stop_blink(Color c){
	switch(c){
		case green:
			if(bl_threads.green_thread != NULL){
				bl_threads.green_thread -> stop();
				delete bl_threads.green_thread;
				bl_threads.green_thread = NULL;
			}
			break;
		case yellow:
			if(bl_threads.yellow_thread != NULL){
				bl_threads.yellow_thread -> stop();
				delete bl_threads.yellow_thread;
				bl_threads.yellow_thread = NULL;
			}
			break;
		case red:
			if(bl_threads.red_thread != NULL){
				bl_threads.red_thread -> stop();
				delete bl_threads.red_thread;
				bl_threads.red_thread = NULL;
			}
			break;
	}
}

void Signal_light::TRAFFIC_LIGHT_NORMAL() {

}

void Signal_light::TRAFFIC_LIGHT_WARNING() {

}

void Signal_light::TRAFFIC_LIGHT_UNACK_ERROR() {

}

void Signal_light::TRAFFIC_LIGHT_ACKED_ERROR() {

}

void Signal_light::TRAFFIC_LIGHT_PASSED_ERROR() {

}

void Signal_light::TRAFFIC_LIGHT_RDY() {

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
