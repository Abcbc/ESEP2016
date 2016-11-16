/*
 * Serial_Manager.cpp
 *
 *  Created on: 28.10.2016
 *      Author: abw181
 */
#include "serial_manager.h"
#include "serial.h"


Serial_Manager::Serial_Manager(const bool sys_middle){
	geme_2 = sys_middle;
	serial1 = new Serial(1);
	serial2 = new Serial(2);
}

Serial_Manager::~Serial_Manager() {
	delete serial1;
	delete serial2;
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
