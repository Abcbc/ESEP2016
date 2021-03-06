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
#include <stdio.h>
#include "src/controller/event_table.h"
#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/dispatcher.cpp"

using namespace std;

class Serial_Transmit: public State {
	public:
	Serial_Transmit(int fdesc_number);
	~Serial_Transmit();

	/*
	 * Sendet Puk ID zum Nachbarsystem
	 */
	int transmit_puk(const int puk_id);
	/*
	 * Sende Estop
	 */
	void ESTOP_THIS();

	/*
	 * Sende Reset
	 */
	void BUTTON_RESET();

	/*
	 * Sende ESTOP_RELEASED auf ein Nachbarsystem
	 */
	void ESTOP_RELEASED_THIS();

	/*
	 * Sende Button Start auf ein Nachbarsystem
	 */
	void BUTTON_START();

	/*
	 * Sende SEND_WANT auf ein Nachbarsystem
	 */
	void send_want();

	/*
	 * Sende SEND_REQUEST_OK auf ein Nachbarsystem
	 */
	void send_request_ok();

	private:
	Serial_Transmit(const Serial_Transmit& other);
	Serial_Transmit& operator=(const Serial_Transmit& other);
	/**
	 * Warte darauf das Paket bestaetigt wird.
	 */
	int fdesc_;
	// Mutex um Senden zu schuetzen
    static pthread_mutex_t mtx_;

};

#endif /* SERIAL_TRANSMIT_H_ */
