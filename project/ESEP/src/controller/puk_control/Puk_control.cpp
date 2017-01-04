/*
 * Puk_control.cpp
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#include <iostream>
#include <algorithm>
#include "controller/puk_control/puk_fsm_dummy.h"
#include "controller/event_table.h"
#include "lib/serial/serial_manager.h"
#include "config.h"

Puk_control* Puk_control::instance_ = NULL;
pthread_mutex_t Puk_control::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Puk_control::Puk_control() :
		dispatcher_(Dispatcher::getInstance()), belt_is_free(true), statePtr(
				&startState), systemType(SYSTEM_NUMBER) {
	dispatcher_ = Dispatcher::getInstance();
	dispatcher_->addListener(this, LIGHT_BARRIER_ENTRY_CLOSE_E_ID);
	dispatcher_->addListener(this, SEND_REQUEST_E_ID);
	dispatcher_->addListener(this, NEW_PUK_E_ID);

	// events for puks
	dispatcher_->addListener(this, LIGHT_BARRIER_ENTRY_OPEN_E_ID);
	dispatcher_->addListener(this, LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
	dispatcher_->addListener(this, LIGHT_BARRIER_EXIT_CLOSE_E_ID);
	dispatcher_->addListener(this, LIGHT_BARRIER_RAMP_CLOSE_E_ID);
	dispatcher_->addListener(this, HEIGHT_SENSOR_MEASURE_START_E_ID);
	dispatcher_->addListener(this, HEIGHT_SENSOR_MEASURE_FINISHED_E_ID);
	dispatcher_->addListener(this, SEND_OK_E_ID);
	dispatcher_->addListener(this, IDENTIFIED_PUK_E_ID);
	dispatcher_->addListener(this, ERR_UNDEFINED_PUK_E_ID);
	dispatcher_->addListener(this, TIMER_EXIT_OUT_E_ID);

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

// TODO: Nothing is getting deleted
void Puk_control::delete_puk(Puk_fsm_dummy* p) {
	std::cout << "Deleting Puk" << std::endl;
	puk_list_.erase(std::remove(puk_list_.begin(), puk_list_.end(), p),
			puk_list_.end());
	SEND_REQUEST();
}

void Puk_control::send_puk(Puk_fsm_dummy* p) {
	std::cout << "Sending Puk to Serial Manager" << std::endl;
}

void Puk_control::create_puk(int pukType) {
	std::cout << "Create new Puk object" << std::endl;
	puk_list_.push_back(new Puk_fsm_dummy(pukType));
	puk_list_.back()->start();
}

bool Puk_control::sequenz_group(int pukType) {
	return statePtr->check(pukType);
}

int Puk_control::puk_count() {
	return puk_list_.size();
}

void Puk_control::try_event(bool (*ptToSignal)(void)) {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_entry_open()) {
			break;
		}
	}
}

void Puk_control::LIGHT_BARRIER_ENTRY_CLOSE() {
	if (systemType == 1) {
		create_puk(42);
	}
}

void Puk_control::SEND_REQUEST() {
	if (systemType != 1) {
		cout << "number of puks: " << puk_list_.size() << endl;
		if (puk_list_.empty()) { // puk_list_.empty()
			MsgSendPulse(CON_ID, PRIO, CODE, SEND_REQUEST_OK_E_ID);
		}
	}
}

void Puk_control::NEW_PUK() {
	Serial_Manager* sm = Serial_Manager::get_instance();
	cout << sm->get_puk_id() << endl;
	create_puk(sm->get_puk_id());
}

void Puk_control::LIGHT_BARRIER_ENTRY_OPEN() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_entry_open()) {
			break;
		}
	}
}

void Puk_control::LIGHT_BARRIER_SWITCH_CLOSE() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_switch_close()) {
			break;
		}
	}
}

void Puk_control::LIGHT_BARRIER_EXIT_CLOSE() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_exit_close()) {
			break;
		}
	}
}

void Puk_control::LIGHT_BARRIER_RAMP_CLOSE() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_ramp_close()) {
			break;
		}
	}
}

void Puk_control::HEIGHT_SENSOR_MEASURE_START() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->height_sensor_measure_start()) {
			break;
		}
	}
}

void Puk_control::HEIGHT_SENSOR_MEASURE_FINISHED() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->height_sensor_measure_finished()) {
			break;
		}
	}
}

void Puk_control::SEND_OK() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->send_ok()) {
			break;
		}
	}
}

void Puk_control::IDENTIFIED_PUK() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->identified_puk()) {
			break;
		}
	}
}

void Puk_control::ERR_UNDEFINED_PUK() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->err_undefined_puk()) {
			break;
		}
	}
}

void Puk_control::TIMER_EXIT_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_exit_out()) {
			break;
		}
	}
}

