/**
* HAW SR2 Embedded System Engineering WS 2016
* serial_transmit.cpp
* @author Julian Magierski
* Copyright (C) 2016 Julian Magierski
* This software is licensed with GNU license
* see LICENSE.txt for details
*/

#include "serial_transmit.h"

Serial_Transmit::Serial_Transmit(int fdesc_number, uint8_t* sequenznr,
		bool* estop) {
	estop_on = estop;
	sqz_transmit = 0;
	fdesc_ = fdesc_number;
	sequenznummer = sequenznr;
	if (fdesc_number == 2) {
		*sequenznummer = 42;
	}
	mtx_ = PTHREAD_MUTEX_INITIALIZER;
}

Serial_Transmit::~Serial_Transmit() {
}

int Serial_Transmit::transmit(const Data *data) {
	pthread_mutex_lock(&mtx_);
	Header header;
	header.paket_typ = DATA;
	header.paket_size = sizeof(*data);
	cout << "transmit: DATA: " << data->data << endl;
	cout << "transmit: Header Paket Size:" << sizeof(header) << endl;
	cout << "transmit: Payload Paket Size:" << header.paket_size << endl;
	sqz_transmit = (sqz_transmit + 1) % MAX_SQZ;
	header.sequenznummer = sqz_transmit;
	cout << "HEADER SQZ: " << header.sequenznummer << endl;
	cout << "Sequenznummer Transmit: " << sqz_transmit << endl;
	cout << "Sequenznummer Receive: " << *sequenznummer << endl;
	int result;
	// Zuerst wird der Header gesendet
	do {
		result = write(this->fdesc_, &header, sizeof(header));
		cout << "While Receive" << *sequenznummer << endl;
	// Warte um den Empfaenger Zeit zum Empfangen zu geben und um ein ACK zu senden
		usleep(QSEC);
	// Sollange ACK noch nicht bestaetigt wurde wiederhole Senden
	} while (*sequenznummer != sqz_transmit);
	sqz_transmit = (sqz_transmit + 1) % MAX_SQZ;
	// Send Data
	do {
		result = write(this->fdesc_, data, header.paket_size);
		usleep(QSEC);
		cout << "test_data" << result << endl;
	} while (*sequenznummer != sqz_transmit);
	pthread_mutex_unlock(&mtx_);
	return 0;
}

int Serial_Transmit::transmit_estop() {
	pthread_mutex_lock(&mtx_);
	*estop_on = true;
	cout << "ESTOP STATE: " << *estop_on << endl;
	Header header;
	header.paket_typ = ESTOP;
	header.paket_size = 1;
	sqz_transmit = (sqz_transmit + 1) % MAX_SQZ;
	header.sequenznummer = sqz_transmit;
	cout << "HEADER SQZ ESTOP: " << header.sequenznummer << endl;
	do {
		write(this->fdesc_, &header, sizeof(header));
		usleep(QSEC);
	} while (*sequenznummer != sqz_transmit);
	pthread_mutex_unlock(&mtx_);
	return 0;
}

int Serial_Transmit::transmit_reset() {
	pthread_mutex_lock(&mtx_);
	*estop_on = false;
	cout << "ESTOP STATE: " << *estop_on << endl;
	Header header;
	header.paket_typ = RESET;
	header.paket_size = 1;
	sqz_transmit = (sqz_transmit + 1) % MAX_SQZ;
	header.sequenznummer = sqz_transmit;
	cout << "HEADER SQZ RESET: " << header.sequenznummer << endl;
	do {
		write(this->fdesc_, &header, sizeof(header));
		usleep(QSEC);
	} while (*sequenznummer != sqz_transmit);
	pthread_mutex_unlock(&mtx_);
	return 0;
}





