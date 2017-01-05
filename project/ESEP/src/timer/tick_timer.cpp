/*
 * timer.cpp
 *
 *  Created on: 01.12.2016
 *      Author: abl395
 */

#include <vector>
#include <queue>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include "timer/tick_timer.h"
#include "src/lib/hal/motor.h"
#include "controller/event_table.h"
#include "lib/dispatcher/Dispatcher.cpp"

#define ID_DOES_NOT_EXIST -1
#define TICK_SEC 0
#define TICK_NSEC 10000000

struct intern_timer{
	uint32_t id;
	int32_t duration;
	int32_t start_duration;
};

static std::vector<intern_timer> timer_vector;
static std::queue<uint32_t> id_queue;
static int con_dispatcher;

Tick_timer* Tick_timer::instance_ = NULL;
pthread_mutex_t Tick_timer::init_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Tick_timer::vector_access_mtx = PTHREAD_MUTEX_INITIALIZER;

uint32_t Tick_timer::start_timer(int32_t duration){
	pthread_mutex_lock(&vector_access_mtx);
	static int32_t id = 0;
	id++;
	struct intern_timer new_timer = {id, duration, duration};
	timer_vector.push_back(new_timer);
	pthread_mutex_unlock(&vector_access_mtx);
	return id;
}

int32_t Tick_timer::stop_timer(uint32_t id){
	pthread_mutex_lock(&vector_access_mtx);
	int idx = find_timer(id);
	if(idx == -1){
		return ID_DOES_NOT_EXIST;
	}
	uint16_t duration = timer_vector[idx].duration;
	timer_vector.erase(timer_vector.begin() + idx);
	pthread_mutex_unlock(&vector_access_mtx);
	return duration;
}

uint32_t Tick_timer::get_duration(uint32_t id){
	pthread_mutex_lock(&vector_access_mtx);
	int idx = find_timer(id);
	if(idx == -1){
		return ID_DOES_NOT_EXIST;
	}
	pthread_mutex_unlock(&vector_access_mtx);
	return timer_vector[idx].duration;
}

int Tick_timer::find_timer(uint32_t id){
	for(uint32_t i = 0; i < timer_vector.size(); i++){
		if(timer_vector[i].id == id){
			return i;
		}
	}
	return ID_DOES_NOT_EXIST;
}

void Tick_timer::set_speed(uint32_t speed){
	step = speed;
}

uint32_t Tick_timer::get_id(){
	uint32_t id = id_queue.front();
	id_queue.pop();
	return id;
}

void Tick_timer::execute(void*){
	struct _pulse     pulse;
	timer_t           timerId;
	struct itimerspec timerSpec;
	struct sigevent   timerEvent;

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
			cout << "ThreadCtl() failed." << endl;
	}

	// init pulse message
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

    // add listener to disptacher
    Dispatcher* d = Dispatcher::getInstance();
    d->addListener(this, SPEED_NORMAL_E_ID);
    d->addListener(this, SPEED_STOP_E_ID);
    d->addListener(this, SPEED_SLOW_E_ID);

    // init timer
	timerSpec.it_value.tv_sec = TICK_SEC;
	timerSpec.it_value.tv_nsec = TICK_NSEC;
	timerSpec.it_interval.tv_sec = TICK_SEC;
	timerSpec.it_interval.tv_nsec = TICK_NSEC;
	timer_settime (timerId, 0, &timerSpec, NULL);

	while(1){
		// wait for tick
	    MsgReceivePulse(cid, &pulse, sizeof (pulse), NULL);
	    if(step != MOTOR_STOP_VALUE){
	    	pthread_mutex_lock(&vector_access_mtx);
			// calculate rest duration
			for(uint32_t i = 0; i < timer_vector.size(); i++){
				timer_vector[i].duration -= step;
				if(timer_vector[i].duration <= 0){
					cout << "timer run out" << endl;
					// if the timer has run out the id is send in pulse-msg
					switch(timer_vector[i].start_duration){
						case SWITCH_OPEN_DURATION :
							MsgSendPulse(3, -1, 5, TIMER_SWITCH_OUT_E_ID);
							break;
						case SWITCH_OPEN_LONG_DURATION :
						    MsgSendPulse(3, -1, 5, TIMER_SWITCH_OUT_E_ID);
                            break;
						case EXIT_DURATION :
							MsgSendPulse(3, -1, 5, TIMER_EXIT_OUT_E_ID);
							break;
						case SHORT_DURATION :
						    MsgSendPulse(3, -1, 5, TIMER_GROUP_OUT_E_ID);
						    break;
						case LONG_DURATION :
                            MsgSendPulse(3, -1, 5, TIMER_GROUP_OUT_E_ID);
                            break;
						default :
							MsgSendPulse(3, -1, 5, TIMER_RUNOUT_E_ID);
					}
					id_queue.push(timer_vector[i].id);
					timer_vector.erase(timer_vector.begin() + i);
				}
			}
			pthread_mutex_unlock(&vector_access_mtx);
	    }
	}
}

void Tick_timer::shutdown(){
	ConnectDetach(con);
	ChannelDestroy(cid);
}

Tick_timer* Tick_timer::get_instance() {
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
	cid = 0;
	con = 0;
	con_dispatcher = con;
	step = MOTOR_STOP_VALUE;

}

Tick_timer::~Tick_timer(){
	// ignore
}


void Tick_timer::SPEED_NORMAL(){
	step = MOTOR_FAST_VALUE;
}
void Tick_timer::SPEED_STOP(){
	step = MOTOR_STOP_VALUE;
}
void Tick_timer::SPEED_SLOW(){
	step = MOTOR_SLOW_VALUE;
}



