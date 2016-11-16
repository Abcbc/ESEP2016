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
#include "lib/HAWThread.h"

using namespace std;
using namespace thread;

class Serial_Receive: public HAWThread {
public:
	Serial_Receive(int fdesc_number, int* sequenznr, bool* estop);
	virtual ~Serial_Receive();

private:
	Serial_Receive(const Serial_Receive& other);
	Serial_Receive& operator=(const Serial_Receive& other);
	int fdesc_;
	bool* estop_on;
	bool thread_run;
	int* sequenznummer;
    static const int MAX_SQZ = 10000;
	/**
	 * Receive struc from Serial Port
	 */
	int transmit_ack(int ack_sqz);
	void create_event_puk_data(void* data);
	void create_event_reset();
	void create_event_estop();

	virtual void execute(void*);
	virtual void shutdown();
};

#endif /* SERIAL_RECEIVE_H_ */
