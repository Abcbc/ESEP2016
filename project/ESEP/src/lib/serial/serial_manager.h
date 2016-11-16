/*
 * Serial_Manager.h
 *	Der Serial Manager kapselt den Zugang zur seriellen Schnittstelle.
<<<<<<< HEAD
 *	Die Klasse bietet die Möglichkeit an eine oder zwei Serielle Verbindungen zu
=======
 *	Die Klasse bietet die Mï¿½glichkeit an eine oder zwei Serielle Verbindungen zu
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
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
<<<<<<< HEAD
	 * zwei Serial für den mittleren GEME2 konfiguriert. Dann ist
=======
	 * zwei Serial fï¿½r den mittleren GEME2 konfiguriert. Dann ist
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 * geme_2 == true. sys_middle == false default Serial und geme_2 == false;
	 */
	Serial_Manager(const bool sys_middle);

	virtual ~Serial_Manager();

	/**
	 * Precondition: Data darf nicht NULL sein.
<<<<<<< HEAD
	 * Postcondition: Rückgabewert 0, Paket wurde gesendet. -1 Fehler.
=======
	 * Postcondition: Rï¿½ckgabewert 0, Paket wurde gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 * Paket wird an GEME1 gesendet wenn Quelle GEME2 ist ansonsten an default
	 * Nachbarsystem.
	 */
	int send(Data* data);
	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Data darf nicht NULL sein.
<<<<<<< HEAD
	 * Postcondition: Rückgabewert 0, Paket wurde and GEME3 gesendet. -1 Fehler.
=======
	 * Postcondition: Rï¿½ckgabewert 0, Paket wurde and GEME3 gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	int send2(Data* data);

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Estop wurde an alle GEME versendet.
<<<<<<< HEAD
	 * Rückgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
=======
	 * Rï¿½ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	int estop_all();

	/**
	 * Precondition:
	 * Postcondition: Estop wurde an GEME 1 versendet.
<<<<<<< HEAD
	 * Rückgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
=======
	 * Rï¿½ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	int estop();

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Estop wurde anGEME2 versendet.
<<<<<<< HEAD
	 * Rückgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
=======
	 * Rï¿½ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	int estop2();

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Fortsetzung nach Estopp wurde an alle GEME versendet.
<<<<<<< HEAD
	 * Rückgabewert 0, Estop Pakete wurden gesendet. -1 Fehler.
=======
	 * Rï¿½ckgabewert 0, Estop Pakete wurden gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	int estop_run_all();

	/**
	 * Precondition:
	 * Postcondition: Fortsetzung nach Estopp wird an GEME1 oder
	 * default Serial versendet.
<<<<<<< HEAD
	 * Rückgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
=======
	 * Rï¿½ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	int estop_run();

	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Postcondition: Estop wird an GEME2 versendet.
<<<<<<< HEAD
	 * Rückgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
=======
	 * Rï¿½ckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	int estop_run2();

	/**
	 * Precondition:
<<<<<<< HEAD
	 * Postcondition: True wenn GEME2 anonsten false für GEME1 oder GEME3.
=======
	 * Postcondition: True wenn GEME2 anonsten false fï¿½r GEME1 oder GEME3.
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	 */
	bool is_GEME_2();

	private:
	Serial_Manager(const Serial_Manager& other);
	Serial_Manager& operator=(const Serial_Manager& other);

<<<<<<< HEAD
    Serial serial1;
	// Serial 2
	Serial serial2;
=======
    Serial* serial1;
	// Serial 2
	Serial* serial2;
>>>>>>> f63716ee56db30d4a73bfb9969a28a7b93b02719
	// True wenn GEME 2
	bool geme_2;
};

#endif /* SERIAL_MANAGER_H_ */
