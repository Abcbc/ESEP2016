/**
* HAW SR2 Embedded System Engineering WS 2016
* paket_protocol.h
* Hier sind die Pakete definiert die Verschickt und
* empfangen werden ueber die Serielle Schnittstelle.
* @author Julian Magierski
* Copyright (C) 2016 Julian Magierski
* This software is licensed with GNU license
* see LICENSE.txt for details
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
typedef struct Header {
	size_t paket_size;
	PAKET_TYP paket_typ;
	uint8_t sequenznummer;
} __attribute__((packed));;

/**
 * Payload
 */
typedef struct Data {
	char data[256];
} __attribute__((packed));;

#endif /* PAKET_PROTOCOL_H_ */
