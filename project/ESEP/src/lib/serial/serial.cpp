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
#include <termios.h>
#include <stdlib.h>
#include <string.h>

Serial::Serial() {
	this->dev_ = "/dev/ser1";
	this->fdesc_ = open(this->dev_, O_RDWR);
	if (this->fdesc_ == -1) {
		exit(-1);
	}
	this->configuration();
	config_thread();
}

Serial::Serial(const int serial_nr) {
	if (serial_nr == 1) {
		this->dev_ = "/dev/ser1";
	} else {
		this->dev_ = "/dev/ser2";
	}
	this->fdesc_ = open(this->dev_, O_RDWR);
	if (this->fdesc_ == -1) {
		exit(-1);
	}
	this->configuration();
	config_thread();
}


void Serial::config_thread() {
	thread_run = true;
	pthread_create(&receive_thread, NULL, helper, this);
}

void* Serial::helper(void *ptr) {
	static_cast<Serial*>(ptr)->receive();
	return 0;
}


Serial::~Serial() {
	thread_run = false;
	if (close(this->fdesc_) < 0) {
		exit(-1);
	}
}


void Serial::receive() {
	Data data;
	Header header;
	memset(&data, 0 , sizeof(data));
	while (thread_run) {
		if (readcond(this->fdesc_, &header, sizeof(Header), sizeof(Header), 0,
				0) > 0) {
			cout << "Erhalten" << endl;
			switch (header.paket_typ) {
			case ESTOP:
				break;
			case EGO:
				break;
			case DATA:
				cout << "Erhalten DATA" << endl;
				cout << "Payload Paket Size:" << header.paket_size << endl;
				if (readcond(this->fdesc_, &data, sizeof(Data),
						sizeof(Data), 0, 0) > 0) {
					cout << "Erhalten DATA Ja" << endl;
					cout << data.data << endl;
				} else {
					cout << "Erhalten DATA Nein" << endl;
				}
			}
		}
	}
}

int Serial::send(const Data *data) {
	Header header;
	header.paket_typ = DATA;
	header.paket_size = sizeof(*data);
	//cout << "Size of ptr" << sizeof(*data) << endl;
	//cout << "Header Paket Size:" << sizeof(header) << endl;
	//cout << "Payload Paket Size:" << header.paket_size << endl;
	write(this->fdesc_, &header, sizeof(Header));
	sleep(10);
	write(this->fdesc_, data, sizeof(Data));
	return 0;
}

int Serial::send_estop() {
	Header header;
	header.paket_typ = ESTOP;
	header.paket_size = 1;
	cout << "Header Paket Size:" << sizeof(header) << endl;
	cout << "Payload Paket Size:" << header.paket_size << endl;
	write(this->fdesc_, &header, sizeof(header));
	sleep(1);
	return 0;
}

int Serial::send_ego() {
	Header header;
	header.paket_typ = EGO;
	header.paket_size = 1;
	write(this->fdesc_, &header, sizeof(header));
	sleep(1);
	return 0;
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


