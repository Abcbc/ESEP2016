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
<<<<<<< HEAD
=======
#include <string.h>
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719

using namespace std;

class Serial {
public:
		Serial();
		/*
		 * Precondition: serial_nr muss '1' oder '2 sein'
		 */
<<<<<<< HEAD
		Serial(const int serial_nr);
		virtual ~Serial();
=======
		 Serial(const int serial_nr);
		 ~Serial();

>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
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
<<<<<<< HEAD
		Serial(const Serial& other);
		Serial& operator=(const Serial& other);

=======
		Serial& operator=(const Serial& other);

		Serial(const Serial& other);
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
		/**
		 * Precondition: -
		 * Postcondition: Serial device wurde konfiguriert
		 */
		void configuration();

		void config_thread();

		// Pfad des Serial device
<<<<<<< HEAD
		const char* dev_;
=======
		char dev_[10];
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
		int fdesc_;
		pthread_t  receive_thread;
		bool thread_run;

		static void* helper(void *ptr);
<<<<<<< HEAD
=======

		/**
		 * Shutdown von Pthread.
		 */
		int shutdown();
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
};


#endif /* SERIAL_H_ */
