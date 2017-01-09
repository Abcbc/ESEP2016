/*
 * Puk_control.cpp
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#include <iostream>
#include <algorithm>
#include <stdint.h>
#include "controller/puk_control/puk_fsm_dummy.h"
#include "controller/event_table.h"
#include "lib/serial/serial_manager.h"
#include "config.h"

Puk_control* Puk_control::instance_ = NULL;
pthread_mutex_t Puk_control::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Puk_control::Puk_control() :
		dispatcher_(Dispatcher::getInstance()), belt_is_free(true), entry_timer_id(
				-1), transmit_timer_id(-1), timer(Tick_timer::get_instance()), accept_new_puk(
				true), statePtr(&startState), systemType(SYSTEM_NUMBER) {
	dispatcher_ = Dispatcher::getInstance();
	dispatcher_->addListener(this, LIGHT_BARRIER_ENTRY_CLOSE_E_ID);
	dispatcher_->addListener(this, SEND_REQUEST_E_ID);
	dispatcher_->addListener(this, NEW_PUK_E_ID);
	dispatcher_->addListener(this, TIMER_NEW_PUK_OUT_E_ID);

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
	dispatcher_->addListener(this, ERR_OK_E_ID);
	dispatcher_->addListener(this, TIMER_EXIT_OUT_E_ID);
	dispatcher_->addListener(this, TIMER_GROUP_OUT_E_ID);
	dispatcher_->addListener(this, BUTTON_START_E_ID);
	dispatcher_->addListener(this, TIMER_HEIGHT_MEASURE_DURATION_OUT_E_ID);
	dispatcher_->addListener(this, TIMER_HEIGHT_MEASURE_TO_RAMP_OUT_E_ID);
	dispatcher_->addListener(this, TIMER_HEIGHT_MEASURE_TO_SWITCH_OUT_E_ID);
	dispatcher_->addListener(this, TIMER_SWITCH_TO_EXIT_OUT_E_ID);
	dispatcher_->addListener(this, TIMER_TRANSMIT_OUT_E_ID);

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

void Puk_control::create_puk(uint16_t pukType, uint16_t pukId) {
	std::cout << "Create new Puk object" << std::endl;
	puk_list_.push_back(new Puk_fsm_dummy(pukType, pukId));
	if (SYSTEM_NUMBER != 3) {
		puk_list_.back()->start();
	} else {
		puk_list_.back()->start_group();
	}

}

bool Puk_control::sequenz_group(int pukType) {
	return statePtr->check(pukType);
}

int Puk_control::puk_count() {
	return puk_list_.size();
}

uint16_t Puk_control::create_puk_id() {
	static uint16_t count = 0;
	return count++;
}

uint16_t Puk_control::get_puk_id(int serial_data) {
	return (uint16_t) serial_data >> 16;
}

uint16_t Puk_control::get_puk_type(int serial_data) {
	return (uint16_t) serial_data;
}

void Puk_control::try_event(bool (*ptToSignal)(void)) {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_entry_open()) {
			break;
		}
	}
}

void Puk_control::TIMER_SWITCH_TO_EXIT_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_switch_to_exit_out()) {
			break;
		}
	}
}

void Puk_control::TIMER_HEIGHT_MEASURE_TO_SWITCH_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_height_measure_to_switch_out()) {
			break;
		}
	}
}

void Puk_control::TIMER_HEIGHT_MEASURE_TO_RAMP_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_height_measure_to_ramp_out()) {
			break;
		}
	}
}

void Puk_control::TIMER_HEIGHT_MEASURE_DURATION_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_height_measurement_duration_out()) {
			break;
		}
	}
}

void Puk_control::TIMER_TRANSMIT_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_transmit_out()) {
			break;
		}
	}
}

void Puk_control::LIGHT_BARRIER_ENTRY_CLOSE() {
	if (SYSTEM_NUMBER == 1) {
		if (timer->get_duration(entry_timer_id) == -1) {
			create_puk(-1, create_puk_id());
			entry_timer_id = timer->start_timer(NEW_PUK_DURATION);
		} else {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_PUK_TOO_EARLY_E_ID);
		}
	} else {
		timer->stop_timer(transmit_timer_id);
	}
}

void Puk_control::SEND_REQUEST() {
	if (SYSTEM_NUMBER == 2) {
		cout << "number of puks: " << puk_list_.size() << endl;
		if (puk_list_.empty()) { // puk_list_.empty()
			cout << "System 2: isEmty for SEND_REQUEST_OK_E_ID: "
					<< puk_list_.empty() << endl;
			MsgSendPulse(CON_ID, PRIO, CODE, SEND_REQUEST_OK_E_ID);
		}
	} else if (SYSTEM_NUMBER == 3) {
		if (accept_new_puk) {
			MsgSendPulse(CON_ID, PRIO, CODE, SEND_REQUEST_OK_E_ID);
		}
	}
}

void Puk_control::NEW_PUK() {
	Serial_Manager* sm = Serial_Manager::get_instance();
	cout << sm->get_puk_id() << endl;
	int s_data = sm->get_puk_id();
	uint16_t id = get_puk_id(s_data);
	uint16_t type = get_puk_type(s_data);
	if (SYSTEM_NUMBER != 1) {
		transmit_timer_id = timer->start_timer(TANSMIT_DURATION);
	}
	if (SYSTEM_NUMBER == 2) {
		create_puk(type, id);
	} else if (SYSTEM_NUMBER == 3) {
		cout << "Puk bool: " << puk_list_.empty() << endl;
		if (puk_list_.empty() || puk_list_.back()->is_state_3()) {
			create_puk(type, id);
		} else if (accept_new_puk) {
			for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
					it != puk_list_.end(); ++it) {
				if ((*it)->getState()->new_puk()) {
					break;
				}
			}
		}
	}
}

void Puk_control::TIMER_NEW_PUK_OUT() {
// Ignore
}

void Puk_control::LIGHT_BARRIER_ENTRY_OPEN() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_entry_open()) {
			break;
		}
	}
	if (SYSTEM_NUMBER == 1) {
		MsgSendPulse(CON_ID, PRIO, CODE, ERR_PUK_TOO_EARLY_OK_E_ID);
	}
//	MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_ERR_E_ID);
//	MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_NEW_PUK_OUT_E_ID);
}

void Puk_control::LIGHT_BARRIER_SWITCH_CLOSE() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_switch_close()) {
			return;
		}
	}
	if (SYSTEM_NUMBER != 3) {
		MsgSendPulse(CON_ID, PRIO, CODE, ERR_TO_MANY_PUK_E_ID);
	}
}

void Puk_control::LIGHT_BARRIER_EXIT_CLOSE() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_exit_close()) {
			return;
		}
	}
	if (SYSTEM_NUMBER != 3) {
		MsgSendPulse(CON_ID, PRIO, CODE, ERR_TO_MANY_PUK_E_ID);
	}
}

void Puk_control::LIGHT_BARRIER_RAMP_CLOSE() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->light_barrier_ramp_close()) {
			return;
		}
	}
	if (SYSTEM_NUMBER != 3) {
		MsgSendPulse(CON_ID, PRIO, CODE, ERR_TO_MANY_PUK_E_ID);
	}
}

void Puk_control::HEIGHT_SENSOR_MEASURE_START() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->height_sensor_measure_start()) {
			return;
		}
	}
	if (SYSTEM_NUMBER != 3) {
		MsgSendPulse(CON_ID, PRIO, CODE, ERR_TO_MANY_PUK_E_ID);
	}
}

void Puk_control::HEIGHT_SENSOR_MEASURE_FINISHED() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->height_sensor_measure_finished()) {
			return;
		}
	}
}

void Puk_control::SEND_OK() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->send_ok()) {
			return;
		}
	}
}

void Puk_control::IDENTIFIED_PUK() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->identified_puk()) {
			return;
		}
	}
}

void Puk_control::ERR_UNDEFINED_PUK() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->err_undefined_puk()) {
			return;
		}
	}
}

void Puk_control::ERR_OK() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->err_ok()) {
			return;
		}
	}
}

void Puk_control::TIMER_EXIT_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_exit_out()) {
			return;
		}
	}
}

void Puk_control::TIMER_GROUP_OUT() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->timer_group_out()) {
			return;
		}
	}
}

void Puk_control::BUTTON_START() {
	for (std::vector<Puk_fsm_dummy*>::iterator it = puk_list_.begin();
			it != puk_list_.end(); ++it) {
		if ((*it)->getState()->button_start()) {
			return;
		}
	}
}

