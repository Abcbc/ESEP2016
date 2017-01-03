/*
 * Puk_control.cpp
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#include <iostream>
#include <algorithm>
#include "src/controller/puk_control/puk_fsm_dummy.h"
#include "src/controller/event_table.h"
#include "src/lib/serial/serial_manager.h"

Puk_control* Puk_control::instance_ = NULL;
pthread_mutex_t Puk_control::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Puk_control::Puk_control() :
		dispatcher_(Dispatcher::getInstance()), belt_is_free(true), statePtr(&startState) {
	dispatcher_ = Dispatcher::getInstance();
	dispatcher_->addListener(this, LIGHT_BARRIER_ENTRY_CLOSE_E_ID);
	std::cout << "Puk_control contructed" << std::endl;
}

Puk_control::~Puk_control() {
}

Puk_control* Puk_control::get_instance() {
	pthread_mutex_lock(&init_mtx);
	if (instance_ == NULL) {
		// TODO: get systemType
		instance_ = new Puk_control();
	}
	pthread_mutex_unlock(&init_mtx);
	return instance_;
}

void Puk_control::delete_puk(Puk_fsm_dummy* p) {
	std::cout << "Deleting Puk" << std::endl;
//	puk_list_.erase(std::find(puk_list_.begin(), puk_list_.end(), p));
	puk_list_.erase(std::remove(puk_list_.begin(), puk_list_.end(), p),
			puk_list_.end());
	if ((systemType != 1) && puk_list_.empty()){
		belt_is_free = true;
	}
}

void Puk_control::send_puk(Puk_fsm_dummy* p) {
	std::cout << "Sending Puk to Serial Manager" << std::endl;
}

void Puk_control::create_puk(int pukType) {
	std::cout << "Create new Puk object" << std::endl;
	puk_list_.push_back(new Puk_fsm_dummy(pukType));
	puk_list_.back()->start();
	if(systemType != 1){
		belt_is_free = false;
	}
}

bool Puk_control::sequenz_group(int pukType) {
	return statePtr->check(pukType);
}

void Puk_control::LIGHT_BARRIER_ENTRY_CLOSE() {
	if (systemType == 1) {
		create_puk(42);
	}
}

void Puk_control::SEND_REQUEST(){
	if (systemType != 1){
		if (belt_is_free){
			MsgSendPulse(CON_ID, PRIO, CODE, SEND_REQUEST_OK_E_ID);
		}
	}
}

void Puk_control::NEW_PUK(){
	Serial_Manager* sm = Serial_Manager::get_instance(false);
	create_puk(sm->get_puk_id());
}



















