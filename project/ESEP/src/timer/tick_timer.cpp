/*
 * timer.cpp
 *
 *  Created on: 01.12.2016
 *      Author: abl395
 */

#include <vector>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include "src/timer/tick_timer.h"
#include "src/lib/hal/motor.h"

#define ID_DOES_NOT_EXIST -1
#define MOTOR_FAST 24
#define MOTOR_SLOW 10

struct intern_timer{
	uint8_t id;
	uint32_t duration;
};

std::vector<intern_timer> timer_vector;

int cid = 0;
int con = 0;
int cid_dispatcher = 0;
Motor* motor;

Tick_timer* Tick_timer::instance_ = NULL;
pthread_mutex_t Tick_timer::init_mtx = PTHREAD_MUTEX_INITIALIZER;

void Tick_timer::start_timer(uint32_t duration, uint8_t id){
	struct intern_timer new_timer = {id, duration};
	timer_vector.push_back(new_timer);
}

uint32_t Tick_timer::stop_timer(uint8_t id){
	int idx = find_timer(id);
	if(idx == -1){
		return ID_DOES_NOT_EXIST;
	}
	uint16_t duration = timer_vector[idx].duration;
	timer_vector.erase(timer_vector.begin() + idx);
	return duration;
}

uint32_t Tick_timer::get_duration(uint8_t id){
	int idx = find_timer(id);
	if(idx == -1){
		return ID_DOES_NOT_EXIST;
	}
	return timer_vector[idx].duration;
}

int Tick_timer::find_timer(uint8_t id){
	for(uint32_t i = 0; i < timer_vector.size(); i++){
		if(timer_vector[i].id == id){
			return i;
		}
	}
	return ID_DOES_NOT_EXIST;
}

void Tick_timer::execute(void*){
	struct _pulse     pulse;
	timer_t           timerId;
	struct itimerspec timerSpec;
	struct sigevent   timerEvent;

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
			cout << "ThreadCtl() failed." << endl;
	}

	// pulse message
	if( (cid  = ChannelCreate(0)) == -1){
		cout << "ChannelCreate() failed"<< endl;
	}
	if( (con = ConnectAttach(0, 0, cid, 0, 0)) == -1){
		cout << "ConnectAttach() failed"<< endl;
	}
    SIGEV_PULSE_INIT (&timerEvent, con, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    if (timer_create (CLOCK_REALTIME, &timerEvent, &timerId) == -1) {
    	cout << "timer_create() failed"<< endl;
    }
    cout << "timer cid: "<< cid << endl;

    // init timer
	timerSpec.it_value.tv_sec = 0;
	timerSpec.it_value.tv_nsec = 10000000;
	timerSpec.it_interval.tv_sec = 0;
	timerSpec.it_interval.tv_nsec = 10000000;
	timer_settime (timerId, 0, &timerSpec, NULL);

	uint32_t value;
	uint16_t step;
	while(1){
	    MsgReceivePulse(cid, &pulse, sizeof (pulse), NULL);
	    if(motor -> is_running()){
	    	step = motor -> is_fast() ? MOTOR_FAST : MOTOR_SLOW;
			for(uint32_t i = 0; i < timer_vector.size(); i++){
				timer_vector[i].duration -= step;
				if(timer_vector[i].duration <= 0){
					cout << "timer run out" << cid_dispatcher << endl;
					value = (timer_vector[i].duration << 8) + timer_vector[i].id;
					cout << MsgSendPulse(cid_dispatcher, -1, 5, value) << endl;
					stop_timer(timer_vector[i].id);
				}
			}
	    }
	}
}

void Tick_timer::shutdown(){
	ConnectDetach(con);
	ChannelDestroy(cid);
}

Tick_timer* Tick_timer::get_instance(int cid) {
	cid_dispatcher = cid;
	if (instance_ == NULL) {
 		pthread_mutex_lock(&init_mtx);
 		if (instance_ == NULL) {
 			instance_ = new Tick_timer();
 		}
 		pthread_mutex_unlock(&init_mtx);
 	}
	return instance_;
}

Tick_timer::Tick_timer() {
	motor = Motor::get_instance();
}

Tick_timer::~Tick_timer(){
	// ignore
}





