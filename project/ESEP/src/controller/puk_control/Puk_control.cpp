/*
 * Puk_control.cpp
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#include "src/controller/puk_control/puk_fsm.h"
#include <algorithm>

Puk_control* Puk_control::instance_ = NULL;
pthread_mutex_t Puk_control::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Puk_control::Puk_control() : statePtr(&startState){
	dispatcher_ = Dispacther::getInstance();
}

Puk_control::~Puk_control() {
	// TODO Auto-generated destructor stub
}

Puk_control* Puk_control::get_instance(){
	pthread_mutex_lock(&init_mtx);
	if(instance_ == NULL){
		// TODO: get systemType
		instance_ = new Puk_control();
	}
	pthread_mutex_unlock(&init_mtx);
	return instance_;
}

void Puk_control::delete_puk(Puk_fsm* p) {
	std::cout << "Deleting Puk" << std::endl;
	puk_list_.erase(std::find(puk_list_.begin(), puk_list_.end(), p).first());
	}

void Puk_control::send_puk(Puk_fsm* p) {
	std::cout << "Sending Puk to Serial Manager" << std::endl;
}

void Puk_control::create_puk(int pukType) {
	std::cout << "Create new Puk object" << std::endl;
	puk_list_.push_back(new Puk_fsm(this, dispatcher_, pukType));
}

bool Puk_control::sequenz_group(int pukType) {
	return statePtr->check();
}

virtual void LIGHT_BARIER_ENTRY_CLOSE(){
	
	if (systemType == 1){
		create_puk(0);
	} else {
		// Get Puk data from serial Manager
		int data = -1;
		create_puk(data);
	}
}
