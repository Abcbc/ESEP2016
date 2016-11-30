/**
* HAW SR2 Embedded System Engineering WS 2016
* serial_manager.h
* Der Serial Manager kapselt den Zugang zur seriellen Schnittstelle.
* Die Klasse stellt zwei serielle Verbindungen her zu den Nachbarsystemen.
* Siehe auch Dokumentation im RDD zu Serieller Schnittstelle. 
* @author Julian Magierski
* Copyright (C) 2016 Julian Magierski
* This software is licensed with GNU license
* see LICENSE.txt for details
*/

#ifndef SERIAL_MANAGER_H_
#define SERIAL_MANAGER_H_

#include <iostream>
#include "serial.h"

using namespace std;

class Serial_Manager{
	public:
	
	/**
	 * Precondition: Data darf nicht NULL sein.
	 * Postcondition: Rueckgabewert 0, Paket wurde gesendet. -1 Fehler.
	 * Paket wird an GEME1 gesendet wenn Quelle GEME2 ist ansonsten an default
	 * Nachbarsystem. Default heisst Versendung ueber com1.
	 * @param data Puk Data zu senden
	 * @return int 0 Wenn ok sonst Wert kleiner 0 
	 */
	int send(const Data* data);
	
	/**
	 * Precondition: GEME muss gleich GEME 2 sein.
	 * Data darf nicht NULL sein.
	 * Postcondition: Rueckgabewert 0, Paket wurde and GEME3 gesendet. -1 Fehler.
	 * @param data Puk Data zu senden
	 * @return int 0 Wenn ok sonst Wert kleiner 0 
	 */
	int send2(const Data* data);

	/**
	 * Postcondition: Estop wurde an alle GEME versendet.
	 * Postcondition: Rueckgabewert 0, Estop Paket wurde gesendet. -1 Fehler.
	 * @return int 0 Wenn ok sonst Wert kleiner 0 
	 */
	int send_estop();

	/**
	 * Postcondition: Fortsetzung nach Estopp wurde an alle GEME versendet.
	 * Postcondition: Rueckgabewert 0, Estop Pakete wurden gesendet. -1 Fehler.
	 * @return int 0 Wenn ok sonst Wert kleiner 0 
	 */
	int send_reset();

	/**
	 * Precondition: -
	 * Postcondition: True wenn GEME2 anonsten false fuer GEME1 oder GEME3.
	 * @return bool true wenn GEME2
	 */
	bool is_GEME_2();

	/**
	 * Returns the pointer of the instance
	 * Precondition: -
	 * Postcondition: Wenn sys_middle geme_2 == true. Sonst geme_2 == false.
	 * Rueckgabe des statisches Pointer auf diese Instanz
	 * @param bool sys_middle geme_2 == true. Wenn sys_middle == false dann default Serial
	 * @return statischer Pointer auf diese Instanz
	 */
	static Serial_Manager* get_instance(const bool sys_middle);

	private:
	/**
	 * Precondition: -
	 * Postcondition: Wenn sys_middle true ist dann werden
	 * zwei Serial fuer den mittleren GEME2 konfiguriert. Dann ist
	 * geme_2 == true. Wenn sys_middle == false dann default Serial und geme_2 == false;
	 * @param bool sys_middle geme_2 == true. Wenn sys_middle == false dann default Serial
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
