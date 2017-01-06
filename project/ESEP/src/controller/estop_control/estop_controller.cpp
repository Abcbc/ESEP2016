/*
 * estop_controller.cpp
 *
 *  Created on: 06.01.2017
 *      Author: abw181
 */

#include "estop_controller.h"

Estop_controller* Estop_controller::instance_ = NULL;
pthread_mutex_t Estop_controller::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Estop_controller* Estop_controller::get_instance() {
	if(instance_ == NULL){
		pthread_mutex_lock(&init_mtx);
		if(instance_ == NULL){
			instance_ = new Estop_controller();
		}
		pthread_mutex_unlock(&init_mtx);
	}
	return instance_;
}

Estop_controller::Estop_controller() {
	estop_state_checker = new Estop_state_checker();
    estop_this = new Estop_fsm(1, estop_state_checker);
    estop_system2 = new Estop_fsm(2,estop_state_checker);
    estop_system3 = new Estop_fsm(3,estop_state_checker);
}

Estop_controller::~Estop_controller() {

}
