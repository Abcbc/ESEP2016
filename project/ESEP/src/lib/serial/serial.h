/*
 * Serial.h
 *
 *  Created on: 28.10.2016
 *      Author: abw181
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <iostream>
#include <pthread.h>
#include <string.h>
#include "serial_transmit.h"
#include "serial_receive.h"

using namespace std;

class Serial {
public:
		/*
		 * Precondition: serial_nr muss '1' oder '2 sein'
		 */
		Serial(const int serial_nr);
		 ~Serial();

		/*
		 * Sendet struc
		 */
		int send(const int puk_id);


		int get_puk_id();

		/*
		 * Sende SEND_WANT auf ein Nachbarsystem
		 */
		void send_want();

		/*
		 * Sende SEND_REQUEST_OK auf ein Nachbarsystem
		 */
		void send_request_ok();

private:
		Serial& operator=(const Serial& other);
		Serial(const Serial& other);
		Serial();
		/**
		 * Precondition: -
		 * Postcondition: Serial device wurde konfiguriert
		 */
		void configuration();

		// Pfad des Serial device
		char dev_[10];
		// file discriptor number
		int fdesc_;
		Serial_Transmit* transmit;
		Serial_Receive* receive;
		// Sequenznummer fuer Serial Verbindung
		// Gibt an ob der ESTOP aktic ist. Sich die Sortieranlage also im Error Zustand befindet
};

#endif /* SERIAL_H_ */
