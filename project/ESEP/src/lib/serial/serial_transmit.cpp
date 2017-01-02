 /*
 *  Created on: 11.11.2016
 *      Author: Julian Magierski
 *      Version: 0.1
 */
#include "serial_transmit.h"

pthread_mutex_t Serial_Transmit::mtx_ = PTHREAD_MUTEX_INITIALIZER;

Serial_Transmit::Serial_Transmit(int fdesc_number) {
	fdesc_ = fdesc_number;
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

int Serial_Transmit::transmit_estop() {
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
		result_write = -1;
	} else {
		result_write = 0;
	}
	pthread_mutex_unlock(&mtx_);
	return result_write;
}

int Serial_Transmit::transmit_reset() {
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
		result_write = -1;
	} else {
		result_write = 0;
	}
	pthread_mutex_unlock(&mtx_);
	return result_write;
}





