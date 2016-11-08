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

Serial::Serial(const int serial_nr) {
	string str;
	if (serial_nr == 1) {
		str = "/dev/ser1";
	} else if (serial_nr == 2) {
		str = "/dev/ser2";
	} else {
	}
	strcpy(this->dev_, str.c_str());
	this->fdesc_ = open(this->dev_, O_RDWR);
	if (this->fdesc_ == -1) {
		exit(-1);
	}
	this->configuration();
	config_thread();
}

void Serial::config_thread() {
	cout << "CONFIG: DEVICE: " << dev_ << endl;
	thread_run = true;
	pthread_create(&receive_thread, NULL, helper, this);
}

void* Serial::helper(void *ptr) {
	static_cast<Serial*>(ptr)->receive();
	return 0;
}

Serial::~Serial() {
	shutdown();
	if (close(this->fdesc_) < 0) {
		exit(-1);
	}
}

void Serial::receive() {
	Header header;
	while (thread_run) {
		Data data;
		if (readcond(this->fdesc_, &header, sizeof(Header), sizeof(Header), 0, 0) > 0) {
			switch (header.paket_typ) {
			//cout << dev_ << endl;
				case ESTOP:
					break;
				case EGO:
					break;
				case DATA:
					cout << "RECEIVE: Payload Paket Size:" << header.paket_size << endl;
					if (readcond(this->fdesc_, &data, header.paket_size, header.paket_size, 0, 0) > 0) {
						cout << "RECEIVE: Erhalten DATA Ja" << endl;
						cout << "RECEIVE DATA: " << data.data << endl;
					} else {
						cout << "RECEIVE: Erhalten DATA Nein" << endl;
					}
			}
		}
	}
}

int Serial::send(const Data *data) {
	Header header;
	header.paket_typ = DATA;
	header.paket_size = sizeof(*data);
	cout << "SEND: DATA: " << data->data << endl;
//	cout << "SEND: Size of ptr" << sizeof(*data) << endl;
	cout << "SEND: Header Paket Size:" << sizeof(header) << endl;
	cout << "SEND: Payload Paket Size:" << header.paket_size << endl;
	write(this->fdesc_, &header, sizeof(header));
	sleep(1);
	write(this->fdesc_, data, header.paket_size);
	return 0;
}

int Serial::send_estop() {
	Header header;
	header.paket_typ = ESTOP;
	header.paket_size = 1;
	cout << "SEND ESTOP: DEVICE: "<< dev_ << endl;
	//cout << "Header Paket Size:" << sizeof(header) << endl;
	//cout << "Payload Paket Size:" << header.paket_size << endl;
	write(this->fdesc_, &header, sizeof(header));
	return 0;
}

int Serial::send_ego() {
	Header header;
	header.paket_typ = EGO;
	header.paket_size = 1;
	cout << "SEND EGO: DEVICE: "<< dev_ << endl;
	write(this->fdesc_, &header, sizeof(header));
	return 0;
}

int Serial::shutdown() {
	thread_run = false;
	Header header;
	header.paket_size = 1;
	cout << "Shutdown" << endl;
	write(this->fdesc_, &header, sizeof(header));
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

