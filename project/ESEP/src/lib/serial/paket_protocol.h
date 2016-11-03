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

	enum PAKET_TYP{DATA, EGO, ESTOP};

	typedef struct {
	size_t paket_size;
	PAKET_TYP paket_typ;
	} Header;

	typedef struct {
		uint8_t data;
	}Data;

#endif /* PAKET_PROTOCOL_H_ */
