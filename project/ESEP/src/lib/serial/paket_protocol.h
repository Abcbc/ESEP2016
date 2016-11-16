/*
 * Paket_Protocol.h
 *
 *  Created on: 01.11.2016
 *      Author: abw181
 */

#ifndef PAKET_PROTOCOL_H_
#define PAKT_PROTOCOL_H_

	#include <iostream>
	#include <stdint.h>

<<<<<<< HEAD
	enum PAKET_TYP{DATA, EGO, ESTOP};

=======
	/**
	 * Der Typ des Pakets
	 */
	enum PAKET_TYP{DATA, EGO, ESTOP};

	/**
	 * Der HEader wird zuerst �bertragen.
	 * paket_size: git die Gr��e des n�chsten Pakets an
	 * PAKET_TYP: gibt den Typ des n�csten Pakets an
	 */
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	typedef struct {
	size_t paket_size;
	PAKET_TYP paket_typ;
	} Header;

<<<<<<< HEAD
	typedef struct {
		uint8_t data;
=======
	/**
	 * Payload
	 */
	typedef struct {
		char data[256];
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	}Data;

#endif /* PAKET_PROTOCOL_H_ */
