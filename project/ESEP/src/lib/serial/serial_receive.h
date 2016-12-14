/**
* HAW SR2 Embedded System Engineering WS 2016
* serial_receive.h
* Pakete werden hier empfangen die von der
* Seriellen Schnittelle kommen.
* @author Julian Magierski
* Copyright (C) 2016 Julian Magierski
* This software is licensed with GNU license
* see LICENSE.txt for details
*/

#ifndef SERIAL_RECEIVE_H_
#define SERIAL_RECEIVE_H_

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <fcntl.h>
#include "lib/HAWThread.h"

using namespace std;
using namespace thread;

class Serial_Receive: public HAWThread {
public:
	/**
	 * Ctor
	 * @param fdesc_number file discriptor 
	 * @param sequenznr Pointer zu SQN
	 * @return -
	 */
	Serial_Receive(int fdesc_number, uint8_t* sequenznr, bool* estop);
	virtual ~Serial_Receive();

private:
	Serial_Receive(const Serial_Receive& other);
	Serial_Receive& operator=(const Serial_Receive& other);
	// file discriptor
	int fdesc_;
	bool* estop_on;
	// thread soll laufen oder nicht
	bool thread_run;
	uint8_t* sequenznummer;
    static const int MAX_SQZ = 256;
	/**
	 * ACK soll an Nachbarsystem versendet werden.
	 * Precondition: - 
	 * Postcondition: ACK wurde gesendet
	 * @param ack_sqz SQN
	 * @return 0 Erfolg oder Wert kleiner 0 Fehler
	 */
	int transmit_ack(int ack_sqz);
	
	/**
	 * Puk Data soll an Puk_Ctrl uebergeben werden.
	 * Precondition: Argument data ist struct Data
	 * Postcondition: Puk Data wurde an Puk_Ctrl ueberreicht
	 * @param * data Puk Data die weitergereicht werden soll
	 * @return -
	 */
	void create_event_puk_data(void* data);
	
	/**
	 * Pulse Event RESSET soll an den Dispatcher gehen.
	 * Precondition: -
	 * Postcondition: -
	 * @return -
	 */
	void create_event_reset();
	
	/**
	 * Pulse Event ESTOP soll an den Dispatcher gehen.
	 * Precondition: -
	 * Postcondition: -
	 * @return -
	 */
	void create_event_estop();

	/**
	 * Execute Funktion geerbt von HAWThread
	 * Precondition: -
	 * Postcondition: -
	 * @param 
	 * @return -
	 */
	virtual void execute(void*);
	
	/**
	 * shutdown Funktion geerbt von 
	 * soll aufgerufen werden, wenn Thread beendet wird
	 * Precondition: -
	 * Postcondition: -
	 * @param 
	 * @return -
	 */
	virtual void shutdown();
};

#endif /* SERIAL_RECEIVE_H_ */
