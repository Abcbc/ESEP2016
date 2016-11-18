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
	Serial_Transmit(int fdesc_number, uint8_t* sequenznr, bool* estop);
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
	uint8_t* sequenznummer;
	uint8_t sqz_transmit;
	bool* estop_on;
	// Mutex um Senden zu schuetzen
    pthread_mutex_t mtx_;
    // Die Anzahl der Microsekunden die gewartet werden sollen bis zum naechsten Senden
    static const int QSEC = 200000;
    //  Die Maximale Groesse der Sequenznummer
    static const int MAX_SQZ = 256;
};

#endif /* SERIAL_TRANSMIT_H_ */
