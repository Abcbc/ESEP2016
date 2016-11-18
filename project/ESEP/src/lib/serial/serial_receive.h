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

using namespace std;
using namespace thread;

class Serial_Receive: public HAWThread {
public:
	Serial_Receive(int fdesc_number, uint8_t* sequenznr, bool* estop);
	virtual ~Serial_Receive();

private:
	Serial_Receive(const Serial_Receive& other);
	Serial_Receive& operator=(const Serial_Receive& other);
	int fdesc_;
	bool* estop_on;
	bool thread_run;
	uint8_t* sequenznummer;
    static const int MAX_SQZ = 256;
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
