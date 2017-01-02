/*
 * serial_receive.h
 *
 *  Created on: 11 Nov 2016
 *      Author: Julian Magierski
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
#include <queue>

using namespace std;
using namespace thread;

class Serial_Receive: public HAWThread {
public:
	Serial_Receive(int fdesc_number, int com_number);
	virtual ~Serial_Receive();
	/**
	 * Aus einem FIFO wird eine Puk Id geholt.
	 * Wenn keine Puk Id vorhanden ist wird -1 zurueck gegeben.
	 */
	int get_puk_id();

private:
	Serial_Receive(const Serial_Receive& other);
	Serial_Receive& operator=(const Serial_Receive& other);
	int fdesc_;
	// Der com Eingang des Empfaengers siehe RDD Serial Componenten Diagram
	int com;
	bool thread_run;
	/**
	 * Receive struc from Serial Port
	 */
	void create_event_new_puk();
	void create_event_send_ok();
	void create_event_send_request();
	void create_event_reset();
	void create_event_estop();
	void add_puk_id(const int puk_id);

	virtual void execute(void*);
	virtual void shutdown();
	queue<int> puk_fifo;
};

#endif /* SERIAL_RECEIVE_H_ */
