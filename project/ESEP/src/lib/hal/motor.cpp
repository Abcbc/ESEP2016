/*
 * motor.cpp
 *
 *  Created on: 19.10.2016
 *      Author: abr468
 */


#include "motor.h"
#include <pthread.h>
#include "lib/HWaccess.h"
#include "HAL.h"
#include <unistd.h>
#include <iomanip>


Motor* Motor::instance_ = NULL;
pthread_mutex_t Motor::init_mtx = PTHREAD_MUTEX_INITIALIZER;


Motor::Motor(){
	BASE 			= PORT_A;
	STOP_BIT 		= 3;
	SLOW_BIT 		= 2;
	LEFT_BIT 		= 1;
	RIGHT_BIT		= 0;
	cur_direction 	= 0;
}

Motor::~Motor(){
	pthread_mutex_destroy(&init_mtx);
}

Motor* Motor::get_instance(void){
	if(instance_ == NULL){
		pthread_mutex_lock(&init_mtx);
		if(instance_ == NULL){
			instance_ = new Motor();
		}
		pthread_mutex_unlock(&init_mtx);
	}
	return instance_;
}
void Motor::start(void){
	set_bit(BASE,get_cur_direction());
	clear_bit(BASE,STOP_BIT);
}

void Motor::stop(void){
	set_bit(BASE,STOP_BIT);
}

void Motor::go_fast(void){
	clear_bit(BASE,SLOW_BIT);
}

void Motor::go_slow(void){
	set_bit(BASE,SLOW_BIT);
}

void Motor::go_left(void){
	clear_set(BASE,RIGHT_BIT,LEFT_BIT);
}

void Motor::go_right(void){
	clear_set(BASE,LEFT_BIT,RIGHT_BIT);
}

bool Motor::is_slow(void){
	return is_set(BASE,SLOW_BIT);
}

bool Motor::is_fast(void){
	return !is_set(BASE,SLOW_BIT);
}

bool Motor::is_stopped(void){
	return is_set(BASE,STOP_BIT);
}

bool Motor::is_running(void){
	return !is_set(BASE,STOP_BIT);
}

uint8_t Motor::get_cur_direction(){
	return cur_direction==0?RIGHT_BIT:LEFT_BIT;
}




