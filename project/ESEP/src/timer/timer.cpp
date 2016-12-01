/*
 * timer.cpp
 *
 *  Created on: 01.12.2016
 *      Author: abl395
 */

#include <vector>
#include <stdint.h>
#include <sys/neutrino.h>
#include "src/timer/timer.h"

#define ID_DOES_NOT_EXIST -1
#define MOTOR_STOP 0
#define MOTOR_FAST 24
#define MOTOR_SLOW 10

struct intern_timer{
	uint16_t id;
	uint16_t duration;
};

std::vector<intern_timer> timer_vector;

int channel = 0;
int connection = 0;

Timer* Timer::instance_ = NULL;
pthread_mutex_t Timer::init_mtx = PTHREAD_MUTEX_INITIALIZER;

void Timer::start_timer(uint16_t duration, uint16_t id){
	struct intern_timer new_timer = {id, duration};
	timer_vector.push_back(new_timer);
}

uint16_t Timer::stop_timer(uint16_t id){
	int idx = find_timer(id);
	if(idx == -1){
		return ID_DOES_NOT_EXIST;
	}
	uint16_t duration = timer_vector[idx].duration;
	timer_vector.erase(timer_vector.begin() + idx);
	return duration;
}

uint16_t Timer::get_duration(uint16_t id){
	int idx = find_timer(id);
	if(idx == -1){
		return ID_DOES_NOT_EXIST;
	}
	return timer_vector[idx].duration;
}

int Timer::find_timer(uint16_t id){
	for(uint32_t i = 0; i < timer_vector.size(); i++){
		if(timer_vector[i].id == id){
			return i;
		}
	}
	return ID_DOES_NOT_EXIST;
}

void Timer::execute(void*){
	struct _pulse     pulse;
	timer_t           timerId;
	struct itimerspec timerSpec;
	struct sigevent   timerEvent;

	// pulse message
	if( (channel  = ChannelCreate(0)) == -1){
		// TODO
	}
	if( (connection = ConnectAttach(0, 0, channel, 0, 0)) == -1){
		// TODO
	}
	if (timer_create (CLOCK_REALTIME, &timerEvent, &timerId) == -1) {
    	// TODO
	}
    SIGEV_PULSE_INIT (&timerEvent, connection, SIGEV_PULSE_PRIO_INHERIT, 0, 0);

    // init timer
	timerSpec.it_value.tv_sec = 2;
	timerSpec.it_value.tv_nsec = 0;
	timerSpec.it_interval.tv_sec = 2;
	timerSpec.it_interval.tv_nsec = 0;
	timer_settime (timerId, 0, &timerSpec, NULL);

	uint16_t step = MOTOR_STOP;
	while(1){
	    MsgReceivePulse(channel, &pulse, sizeof (pulse), NULL);
	    // TODO get motor speed -> set step
	    for(uint32_t i = 0; i < timer_vector.size(); i++){
	    	timer_vector[i].duration -= step;
	    	if(timer_vector[i].duration == 0){
	    		// TODO send pulse message with id
	    		stop_timer(timer_vector[i].id);
	    	}
	    }
	}
}

void Timer::shutdown(){
	ConnectDetach(connection);
	ChannelDestroy(channel);
}

Timer* Timer::get_instance(void) {
	if (instance_ == NULL) {
 		pthread_mutex_lock(&init_mtx);
 		if (instance_ == NULL) {
 			instance_ = new Timer();
 		}
 		pthread_mutex_unlock(&init_mtx);
 	}
	return instance_;
}

Timer::Timer() {
	// ignore
}

Timer::~Timer(){
	// ignore
}





