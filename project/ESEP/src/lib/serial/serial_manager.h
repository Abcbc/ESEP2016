/*
 * Serial_Manager.h
 *	Der Serial Manager kapselt den Zugang zur seriellen Schnittstelle.
 *	Die Klasse bietet die M�glichkeit an eine oder zwei Serielle Verbindungen zu
 *	konfigurieren.
 *
 *  Created on: 28.10.2016
 *      Author: Julian Magierski
 *      Version: 0.1
 */

#ifndef SERIAL_MANAGER_H_
#define SERIAL_MANAGER_H_

#include <iostream>
#include "serial.h"

using namespace std;

typedef struct {
	bool set;
} Estop;

class Serial_Manager{
	public:
	/**
	 * Precondition: -
	 * Postcondition: Wenn sys_middle true ist dann werden
	 * zwei Serial f�r den mittleren GEME2 konfiguriert. Dann ist
	 * geme_2 == true. sys_middle == false default Serial und geme_2 == false;
	 */
	Serial_Manager(const bool sys_middle);

	virtual ~Serial_Manager();

	/**
	 * Precondition: Data darf nicht NULL sein.
	 * Postcondition: R�ckgabewert 0, Paket wurde gesendet. -1 Fehler.
	 * Paket wird an GEME1 gesendet wenn Quelle GEME2 ist ansonsten an default
	 * Nachbarsystem.
	 */
	int send(Data* data);
	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Data darf nicht NULL sein.
	 * Postcondition: R�ckgabewert 0, Paket wurde and GEME3 gesendet. -1 Fehler.
	 */
	int send2(Data* data);

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Estop wurde an alle GEME versendet.
	 * R�ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
	 */
	int estop_all();

	/**
	 * Precondition:
	 * Postcondition: Estop wurde an GEME 1 versendet.
	 * R�ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
	 */
	int estop();

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Estop wurde anGEME2 versendet.
	 * R�ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
	 */
	int estop2();

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Fortsetzung nach Estopp wurde an alle GEME versendet.
	 * R�ckgabewert 0, Estop Pakete wurden gesendet. -1 Fehler.
	 */
	int estop_run_all();

	/**
	 * Precondition:
	 * Postcondition: Fortsetzung nach Estopp wird an GEME1 oder
	 * default Serial versendet.
	 * R�ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
	 */
	int estop_run();

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Estop wird an GEME2 versendet.
	 * R�ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
	 */
	int estop_run2();

	/**
	 * Precondition:
	 * Postcondition: True wenn GEME2 anonsten false f�r GEME1 oder GEME3.
	 */
	bool is_GEME_2();

	private:
	Serial_Manager(const Serial_Manager& other);
	Serial_Manager& operator=(const Serial_Manager& other);

    Serial* serial1;
	// Serial 2
	Serial* serial2;
	// True wenn GEME 2
	bool geme_2;
};

#endif /* SERIAL_MANAGER_H_ */
