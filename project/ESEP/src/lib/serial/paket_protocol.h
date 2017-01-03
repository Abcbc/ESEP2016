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
	PUK_ID, RESET, ESTOP, RELEASED, START, REQUEST, REQUEST_OK
};

/**
 * Zeigt alle Debug Messages aus oder an.
 * An  = 1
 * Aus = 0
 */
#define SHOW_DEBUG_MESSAGE 1
/**
 * Der Header wird zuerst uebertragen.
 * paket_size: git die Groesse des naehsten Pakets an
 * PAKET_TYP: gibt den Typ des naecsten Pakets an
 */
typedef struct {
	size_t paket_size;
	PAKET_TYP paket_typ;
} Header;

/**
 * Payload
 */
typedef struct {
	uint32_t puk_id;

} Data;

#endif /* PAKET_PROTOCOL_H_ */
