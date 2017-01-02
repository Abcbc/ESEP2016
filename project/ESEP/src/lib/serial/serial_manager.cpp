/*
 * Serial_Manager.cpp
 *
 *  Created on: 28.10.2016
 *      Author: abw181
 */
#include "serial_manager.h"

Serial_Manager* Serial_Manager::instance_ = NULL;
pthread_mutex_t Serial_Manager::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Serial_Manager::Serial_Manager(const bool sys_middle){
	geme_2 = sys_middle;
	serial1 = new Serial(1);
	serial2 = new Serial(2);
}

Serial_Manager::~Serial_Manager() {
	delete serial1;
	delete serial2;
	pthread_mutex_destroy(&init_mtx);
}

Serial_Manager* Serial_Manager::get_instance(const bool sys_middle) {
	if(instance_ == NULL){
		pthread_mutex_lock(&init_mtx);
		if(instance_ == NULL){
			instance_ = new Serial_Manager(sys_middle);
		}
		pthread_mutex_unlock(&init_mtx);
	}
	return instance_;
}

int Serial_Manager::send_to_system2(const int puk_id) {
	return serial1->send(puk_id);
}

int Serial_Manager::send_to_system3(const int puk_id) {
	if (is_GEME_2()) {
		return serial2->send(puk_id);
	} else {
		return -1;
	}
}

int Serial_Manager::get_puk_id() {
	return serial1->get_puk_id();
}

bool Serial_Manager::is_GEME_2() {
	return geme_2;
}
