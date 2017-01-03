 /*
 *  Created on: 11.11.2016
 *      Author: Julian Magierski
 *      Version: 0.1
 */
#include "serial_transmit.h"

pthread_mutex_t Serial_Transmit::mtx_ = PTHREAD_MUTEX_INITIALIZER;

Serial_Transmit::Serial_Transmit(int fdesc_number) {
	fdesc_ = fdesc_number;
	Dispatcher *d = Dispatcher::getInstance();
	d->addListener(this, SEND_WANT_E_ID);
	d->addListener(this, ESTOP_THIS_E_ID);
	d->addListener(this, ESTOP_RESET_THIS_E_ID);
	d->addListener(this, ESTOP_RELEASED_THIS_E_ID);
	d->addListener(this, BUTTON_START_E_ID);
	d->addListener(this, SEND_REQUEST_OK_E_ID);
}

Serial_Transmit::~Serial_Transmit() {
}

int Serial_Transmit::transmit_puk(const int puk_id) {
	pthread_mutex_lock(&mtx_);
	Header header;
	header.paket_typ = PUK_ID;
	Data data;
	data.puk_id = puk_id;
	header.paket_size = sizeof(data);
	if (SHOW_DEBUG_MESSAGE) {
		cerr << "transmit: DATA: " << data.puk_id << "\n";
		cerr << "transmit: Header Paket Size:" << sizeof(header) << "\n";
		cerr << "transmit: Payload Paket Size:" << header.paket_size << "\n";
	}
	// Zuerst wird der Header gesendet
	int result_write = write(this->fdesc_, &header, sizeof(header));
	if (result_write <= 0) {
		perror("Error Serial, Transmit Header Puk Data");
		pthread_mutex_unlock(&mtx_);
		return -1;
	} else {
		result_write = 0;
	}
	// Dann die Daten
	result_write = write(this->fdesc_, &data, header.paket_size);
	if (result_write <= 0) {
		perror("Error Serial, Transmit Puk Data");
		result_write = -1;
	} else {
		result_write = 0;
	}
	pthread_mutex_unlock(&mtx_);
	return result_write;
}

void Serial_Transmit::ESTOP_RESET_THIS() {
	pthread_mutex_lock(&mtx_);
	if (SHOW_DEBUG_MESSAGE) {
		cerr << "Send Reset\n";
	}
	// Verkapsulung des RESET
	Header header;
	header.paket_typ = RESET;
	header.paket_size = 1;
	// Transmit RESET
	int result_write = write(this->fdesc_, &header, sizeof(header));
	if (result_write <= 0) {
		perror("Error Serial, Transmit RESET");
	}
	pthread_mutex_unlock(&mtx_);
}

void Serial_Transmit::ESTOP_THIS() {
	pthread_mutex_lock(&mtx_);
		if (SHOW_DEBUG_MESSAGE) {
			cerr << "Send ESTOP\n";
		}
		// Verkapsulung des ESTOP
		Header header;
		header.paket_typ = ESTOP;
		header.paket_size = 1;
		// Transmit ESTOP
		int result_write = write(this->fdesc_, &header, sizeof(header));
		if (result_write <= 0) {
			perror("Error Serial, Transmit ESTOP");
		}
		pthread_mutex_unlock(&mtx_);
}

void Serial_Transmit::ESTOP_RELEASED_THIS() {
	pthread_mutex_lock(&mtx_);
		if (SHOW_DEBUG_MESSAGE) {
			cerr << "Send ESTOP_RELEASED_THIS\n";
		}
		// Verkapsulung des ESTOP
		Header header;
		header.paket_typ = RELEASED;
		header.paket_size = 1;
		// Transmit ESTOP
		int result_write = write(this->fdesc_, &header, sizeof(header));
		if (result_write <= 0) {
			perror("Error Serial, Transmit RELEASED");
		}
		pthread_mutex_unlock(&mtx_);
}

void Serial_Transmit::BUTTON_START() {
	Header header;
	header.paket_typ = START;
	header.paket_size = 1;
	// Transmit Send Button Start
	int result_write = write(this->fdesc_, &header, sizeof(header));
	if (result_write <= 0) {
		perror("Error Serial, Transmit START");
	}
}


void Serial_Transmit::SEND_WANT() {
	Header header;
	header.paket_typ = REQUEST;
	header.paket_size = 1;
	// Transmit Send Want
	int result_write = write(this->fdesc_, &header, sizeof(header));
	if (result_write <= 0) {
		perror("Error Serial, Transmit Send Want");
	}
}

void Serial_Transmit::SEND_REQUEST_OK() {
	Header header;
	header.paket_typ = REQUEST_OK;
	header.paket_size = 1;
	// Transmit Send REQUEST_OK_PACKED
	int result_write = write(this->fdesc_, &header, sizeof(header));
	if (result_write <= 0) {
		perror("Error Serial, Transmit Send REQUEST_OK");
	}
}






