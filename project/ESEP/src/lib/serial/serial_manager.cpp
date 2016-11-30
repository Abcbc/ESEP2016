/**
* HAW SR2 Embedded System Engineering WS 2016
* serial_manager.cpp
* @author Julian Magierski
* Copyright (C) 2016 Julian Magierski
* This software is licensed with GNU license
* see LICENSE.txt for details
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

int Serial_Manager::send(const Data *data) {
	return serial1->send(data);
}

int Serial_Manager::send2(const Data *data) {
	if (is_GEME_2()) {
		return serial2->send(data);
	} else {
		return -1;
	}
}

int Serial_Manager::send_estop() {
	if (serial1->send_estop() == -1) {
		return -1;
	}
	return serial2->send_estop();
}

int Serial_Manager::send_reset() {
	if(serial1->send_reset() == -1) {
		return -1;
	}
	return serial2->send_reset();
}

bool Serial_Manager::is_GEME_2() {
	return geme_2;
}
