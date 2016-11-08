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
#include <string.h>

using namespace std;

class Serial {
public:
		Serial();
		/*
		 * Precondition: serial_nr muss '1' oder '2 sein'
		 */
		 Serial(const int serial_nr);
		 ~Serial();

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
		Serial& operator=(const Serial& other);

		Serial(const Serial& other);
		/**
		 * Precondition: -
		 * Postcondition: Serial device wurde konfiguriert
		 */
		void configuration();

		void config_thread();

		// Pfad des Serial device
		char dev_[10];
		int fdesc_;
		pthread_t  receive_thread;
		bool thread_run;

		static void* helper(void *ptr);

		/**
		 * Shutdown von Pthread.
		 */
		int shutdown();
};


#endif /* SERIAL_H_ */
