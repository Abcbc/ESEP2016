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

/**
 * Der Typ des Pakets
 * DATA = Puk Data Information zur weiterreichung an eine der anderen Sortieranlagen
 * RESET = Reset Taste wurde gedrueckt
 * ESTOP = ESTOP Taste wurde gedrueckt
 * ACK = Bestaetigung eines Paketes
 */
enum PAKET_TYP {
	DATA, RESET, ESTOP, ACK
};

/**
 * Der Header wird zuerst uebertragen.
 * paket_size: git die Groesse des naehsten Pakets an
 * PAKET_TYP: gibt den Typ des naecsten Pakets an
 */
typedef struct {
	size_t paket_size;
	PAKET_TYP paket_typ;
	uint8_t sequenznummer;
} Header;

/**
 * Payload
 */
typedef struct {
	char data[256];

} Data;

#endif /* PAKET_PROTOCOL_H_ */
