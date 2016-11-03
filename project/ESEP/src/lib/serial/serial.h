/*
 * Serial.h
 *
 *  Created on: 28.10.2016
 *      Author: abw181
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <iostream>
#include "paket_protocol.h"
#include <pthread.h>

using namespace std;

class Serial {
public:
		Serial();
		/*
		 * Precondition: serial_nr muss '1' oder '2 sein'
		 */
		Serial(const int serial_nr);
		virtual ~Serial();
		/*
		 * Sendet struc
		 */
		int send(const Data *data);
		/*
		 * Sende estop
		 */
		int send_estop();
		/*
		 * Sende: Band soll laufen
		 */
		int send_ego();

		/**
		 * Wartet auf struc
		 */
		void receive();

private:
		Serial(const Serial& other);
		Serial& operator=(const Serial& other);

		/**
		 * Precondition: -
		 * Postcondition: Serial device wurde konfiguriert
		 */
		void configuration();

		void config_thread();

		// Pfad des Serial device
		const char* dev_;
		int fdesc_;
		pthread_t  receive_thread;
		bool thread_run;

		static void* helper(void *ptr);
};


#endif /* SERIAL_H_ */
