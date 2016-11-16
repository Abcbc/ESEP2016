 /*
 *  Created on: 11.11.2016
 *      Author: Julian Magierski
 *      Version: 0.1
 */
#include "serial_transmit.h"

Serial_Transmit::Serial_Transmit(int fdesc_number, int* sequenznr,
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
	cout << "Sequenznummer Receive: " << *sequenznummer  << endl;
	int result;
	do {
	 result = write(this->fdesc_, &header, sizeof(header));
		cout << "While Receive" << *sequenznummer  << endl;
		usleep(QSEC);
	} while (*sequenznummer != sqz_transmit);
	sqz_transmit = (sqz_transmit + 1) % MAX_SQZ;
	do {
	result =	write(this->fdesc_, data, header.paket_size);
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





