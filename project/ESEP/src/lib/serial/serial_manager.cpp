/*
 * Serial_Manager.cpp
 *
 *  Created on: 28.10.2016
 *      Author: abw181
 */
#include "serial_manager.h"
#include "serial.h"


Serial_Manager::Serial_Manager(const bool sys_middle) {
	geme_2 = sys_middle;
	Serial serial1(1);
	Serial serial2(2);
}

Serial_Manager::~Serial_Manager() {
}

int Serial_Manager::send(Data *data) {
	return serial1.send(data);
}

int Serial_Manager::send2(Data *data) {
	if (is_GEME_2()) {
		return serial2.send(data);
	} else {
		return -1;
	}
}

int Serial_Manager::estop_all() {
	if (serial1.send_estop() == -1) {
		return -1;
	}
	return serial2.send_estop();
}

int Serial_Manager::estop() {
	return serial1.send_estop();
}

int Serial_Manager::estop2() {
	return serial2.send_estop();
}

int Serial_Manager::estop_run_all() {
	if(serial1.send_ego() == -1) {
		return -1;
	}
	return serial2.send_ego();
}

int Serial_Manager::estop_run() {
	return serial1.send_ego();
}

int Serial_Manager::estop_run2() {
	return serial2.send_ego();
}

bool Serial_Manager::is_GEME_2() {
	return geme_2;
}
