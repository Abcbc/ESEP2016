/*
 * serial_transmit.h
 *
 *  Created on: 11 Nov 2016
 *      Author: Julian Magierski
 */

#ifndef SERIAL_TRANSMIT_H_
#define SERIAL_TRANSMIT_H_

#include <iostream>
#include "paket_protocol.h"
#include <pthread.h>
#include <unistd.h>
#include "lib/HAWThread.h"

using namespace std;

class Serial_Transmit {
	public:
	Serial_Transmit(int fdesc_number, int* sequenznr, bool* estop);
	~Serial_Transmit();

	/*
	 * Sendet struc
	 */
	int transmit(const Data *data);
	/*
	 * Sende estop
	 */
	int transmit_estop();
	/*
	 * Sende: Band soll laufen
	 */
	int transmit_reset();

	private:
	Serial_Transmit(const Serial_Transmit& other);
	Serial_Transmit& operator=(const Serial_Transmit& other);
	/**
	 * Warte darauf das Paket bestaetigt wird.
	 */
	int fdesc_;
	int* sequenznummer;
	int sqz_transmit;
	bool* estop_on;
    pthread_mutex_t mtx_;
    static const int QSEC = 200000;
    static const int MAX_SQZ = 10000;
};

#endif /* SERIAL_TRANSMIT_H_ */
