/*
 * Serial.cpp
 *
 *  Created on: 28.10.2016
 *      Author: abw181
 */
#include "serial.h"
#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

Serial::Serial(int serial_nr) {
	string str;
	// Com siehe auch Serial Componenten Diagram
	int com = 0;
	// Zuerst wird das Serial Device bestimmt
	if (serial_nr == 1) {
		str = "/dev/ser1";
		com = 1;
	} else if (serial_nr == 2) {
		com = 2;
		str = "/dev/ser2";
	} else {
		exit(-1);
	}
	strcpy(dev_, str.c_str());
	fdesc_ = open(dev_, O_RDWR);
	if (this->fdesc_ == -1) {
		exit(-1);
	}
	// Dann wird der Empfaenger und Sender definiert
	transmit = new Serial_Transmit(fdesc_);
	receive = new Serial_Receive(fdesc_, com);
	receive->start(NULL);
	// Die Serial Connection wird Konfiguriert
	this->configuration();
}

Serial::~Serial() {
	delete transmit;
	delete receive;
	if (close(this->fdesc_) < 0) {
		exit(-1);
	}
}

int Serial::send(const int puk_id) {
	return transmit->transmit_puk(puk_id);
}

int Serial::send_estop() {
	return transmit->transmit_estop();
}

int Serial::send_reset() {
	return transmit->transmit_reset();
}

int Serial::get_puk_id() {
	return receive->get_puk_id();
}

void Serial::configuration() {
	struct termios ts;
	tcflush(this->fdesc_, TCIOFLUSH);
	tcgetattr(this->fdesc_, &ts);
	cfsetispeed(&ts, B19200);
	cfsetospeed(&ts, B19200);
	ts.c_cflag &= ~CSIZE;
	ts.c_cflag &= ~CSTOPB;
	ts.c_cflag &= ~PARENB;
	ts.c_cflag |= CS8;
	ts.c_cflag |= CREAD;
	ts.c_cflag |= CLOCAL;
	tcsetattr(this->fdesc_, TCSANOW, &ts);
}

