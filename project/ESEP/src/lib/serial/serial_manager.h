/*
 * Serial_Manager.h
 *	Der Serial Manager kapselt den Zugang zur seriellen Schnittstelle.
 *	Die Klasse bietet die Moeglichkeit an zwei Serielle Verbindungen herzustellen.
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
	 * Precondition: Data darf nicht NULL sein.
	 * Postcondition: Rueckgabewert 0, Paket wurde gesendet. -1 Fehler.
	 * Paket wird an GEME2 gesendet wenn Quelle GEME1
	 */
	int send_to_system2(const int puk_id);
	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Data darf nicht NULL sein.
	 * Postcondition: Rueckgabewert 0, Paket wurde and GEME3 gesendet. -1 Fehler.
	 */
	int send_to_system3(const int puk_id);

	/**
	 * Precondition:
	 * Postcondition: True wenn GEME2 anonsten false f�r GEME1 oder GEME3.
	 */
	bool is_GEME_2();

	/**
	 * Aus einem FIFO wird eine Puk Id geholt.
	 * Wenn keine Puk Id vorhanden ist wird -1 zurueck gegeben.
	 */
	int get_puk_id();

	/*
	 * Returns the pointer of the instance
	 * @return
	 */
	static Serial_Manager* get_instance(const bool sys_middle);

	private:
	/**
	 * Precondition: -
	 * Postcondition: Wenn sys_middle true ist dann werden
	 * zwei Serial fuer den mittleren GEME2 konfiguriert. Dann ist
	 * geme_2 == true. Wenn sys_middle == false dann default Serial und geme_2 == false;
	 */
	Serial_Manager(const bool sys_middle);
	virtual ~Serial_Manager();
	Serial_Manager(const Serial_Manager& other);
	Serial_Manager& operator=(const Serial_Manager& other);

	static Serial_Manager* instance_;
	static pthread_mutex_t init_mtx;

    Serial* serial1;
	// Serial 2
	Serial* serial2;
	// True wenn GEME 2
	bool geme_2;
};

#endif /* SERIAL_MANAGER_H_ */
