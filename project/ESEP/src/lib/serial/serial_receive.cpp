/*
 * serial_receive.cpp
 *
 *  Created on: 11 Nov 2016
 *      Author: Julian Magierski
 */
#include "serial_receive.h"
#include "paket_protocol.h"
#include <stdio.h>

Serial_Receive::Serial_Receive(int fdesc_number, int com_number) {
	fdesc_ = fdesc_number;
	com = com_number;
	// Receive Thread soll laufen
	thread_run = true;
}

Serial_Receive::~Serial_Receive() {

}

void Serial_Receive::execute(void*) {
	Header header;
	while (thread_run) {
		Data data;
		if (readcond(this->fdesc_, &header, sizeof(Header), sizeof(Header), 0,
				0) > 0) {
			switch (header.paket_typ) {
			// Ein ESTOP wurde empfangen
			case ESTOP:
				create_event_estop();
				break;
				// Reset wurde empfangen
			case RESET:
				// Erstell ein RESET Event
				create_event_reset();
				break;
				// Puk ID wurde empfangen
			case PUK_ID:
				cerr << "RECEIVE: Payload Paket Size:" << header.paket_size
						<< "\n";
				if (readcond(this->fdesc_, &data, header.paket_size,
						header.paket_size, 0, 0) > 0) {
					if (SHOW_DEBUG_MESSAGE) {
						cerr << "RECEIVE DATA: " << data.puk_id << "\n";
					}
					// Erstell ein NEW_PUK Event
					add_puk_id(data.puk_id);
					create_event_new_puk();
				} else {
					if (SHOW_DEBUG_MESSAGE) {
						cerr << "RECEIVE: Erhalten DATA Nein" << "\n";
					}
				}
				break;
				// Send ok wurde empfangen
			case SEND_OK:
				create_event_send_ok();
				break;
				// Send Request wurde empfangen
			case SEND_REQUEST:
				create_event_send_request();
				break;
			}
			// Beim empfangen ist ein Fehler aufgetreten.
		} else {
			perror("Error Serial, Receive Header");
		}
	}
}

void Serial_Receive::create_event_new_puk() {
	if ( SHOW_DEBUG_MESSAGE ) {
		cerr << "create event NEW_PUK\n";
	}
}

void Serial_Receive::create_event_send_ok() {
	if ( SHOW_DEBUG_MESSAGE ) {
		cerr << "create event SEND_OK\n";
	}
}

void Serial_Receive::create_event_send_request() {
	if ( SHOW_DEBUG_MESSAGE ) {
		cerr << "create event SEND_REQUEST\n";
	}
}

void Serial_Receive::create_event_reset() {
	if ( SHOW_DEBUG_MESSAGE ) {
	cerr << "Create Event RESET\n";
	}
}


int Serial_Receive::get_puk_id() {
	int puk_id;
	if (!puk_fifo.empty()) {
		puk_id = puk_fifo.front();
		puk_fifo.pop();
	} else {
		puk_id = -1;
	}
	return puk_id;
}

void Serial_Receive::add_puk_id(const int puk_id) {
	puk_fifo.push(puk_id);
}

void Serial_Receive::create_event_estop() {
	if ( SHOW_DEBUG_MESSAGE ) {
	cerr << "Create Event ESTOP\n";
	}
}

void Serial_Receive::shutdown() {
	thread_run = false;
}
