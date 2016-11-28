/**
* HAW SR2 Embedded System Engineering WS 2016
* serial.h
* Stellt eine Verbundung zu einem Nachbarsystem da ueber
* Serielle Schnittstelle.
* @author Julian Magierski
* Copyright (C) 2016 Julian Magierski
* This software is licensed with GNU license
* see LICENSE.txt for details
*/

#ifndef SERIAL_H_
#define SERIAL_H_

#include <iostream>
#include <pthread.h>
#include <string.h>
#include "serial_transmit.h"
#include "serial_receive.h"

using namespace std;

class Serial {
public:
		/**
		* Precondition: serial_nr muss '1' oder '2 sein'
		* Postcondition: -
		* @param int serial_nr Nummer der Verbindung
		* @return Ctor
		*/
		Serial(const int serial_nr);
		 ~Serial();

		/**
		* Precondition: -
		* Postcondition: -
		* @param data Puk Data struct welches versendet wird
		* @return 0 bei Erfolg sonst Wert < 1
		*/
		int send(const Data *data);
		
		/**
		* Precondition: -
		* Postcondition: ESTOP wurde an Nachbarsystem versendet.
		* @return 0 bei Erfolg sonst Wert < 1
		*/
		int send_estop();
		
		/**
		* Precondition: -
		* Postcondition: RESET wurde an Nachbarsystem versendet.
		* @return 0 bei Erfolg sonst Wert < 1
		*/
		int send_reset();

private:
		Serial& operator=(const Serial& other);
		Serial(const Serial& other);
		Serial();
		
		/**
		 * Precondition: -
		 * Postcondition: Serial device wurde konfiguriert
		 */
		void configuration();

		// Pfad des Serial device
		char dev_[10];
		// file discriptor number
		int fdesc_;
		Serial_Transmit* transmit;
		Serial_Receive* receive;
		// Sequenznummer fuer Serial Verbindung
		uint8_t sequenznummer;
		// Gibt an ob der ESTOP aktic ist. Sich die Sortieranlage also im Error Zustand befindet
		bool estop_on;
};

#endif /* SERIAL_H_ */
