/*
 * serial_receive.cpp
 *
 *  Created on: 11 Nov 2016
 *      Author: Julian Magierski
 */
#include "serial_receive.h"
#include "paket_protocol.h"

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
			case REQUEST_OK:
				create_event_send_ok();
				break;
				// Send Request wurde empfangen
			case REQUEST:
				create_event_send_request();
				break;
				// Released wurde empfangen
			case RELEASED:
				create_event_released();
				break;
				// Button Start wurde empfangen
			case START:
				create_event_button_start();
				break;
			}
			// Beim empfangen ist ein Fehler aufgetreten.
		} else {
			perror("Error Serial, Receive Header");
		}
	}
}

void Serial_Receive::create_event_new_puk() {
	if (SHOW_DEBUG_MESSAGE) {
		cerr << "create event NEW_PUK\n";
	}
	if (MsgSendPulse(3, -1, 100, NEW_PUK_E_ID) < 0) {
		perror("Error Receive NEW_PUK from Serial com: " + com);
	}
}

void Serial_Receive::create_event_released() {
	if (SHOW_DEBUG_MESSAGE) {
		cerr << "create event RELEASED\n";
	}
	if (com == 1) {
		if (MsgSendPulse(3, -1, 101, ESTOP_RELEASED_SYSTEM3_E_ID) < 0) {
			perror("Error Receive RELEASED from Serial com: " + com);
		}
	} else {
		if (MsgSendPulse(3, -1, 102, ESTOP_RELEASED_SYSTEM3_E_ID) < 0) {
			perror("Error Receive RELEASED from Serial com: " + com);
		}
	}
}

void Serial_Receive::create_event_button_start() {
	if (SHOW_DEBUG_MESSAGE) {
		cerr << "create event BUTTON_START_INCOMMING\n";
	}
	if (MsgSendPulse(3, -1, 103, BUTTON_START_INCOMMING_E_ID) < 0) {
		perror("Error Receive BUTTON_START from Serial com: " + com);
	}
}

void Serial_Receive::create_event_send_ok() {
	if ( SHOW_DEBUG_MESSAGE ) {
		cerr << "create event SEND_OK\n";
	}
	if (MsgSendPulse(3, -1, 104, SEND_OK_E_ID) < 0) {
		perror("Error Receive SEND_OK from Serial com: " + com);
	}
}

void Serial_Receive::create_event_send_request() {
	if ( SHOW_DEBUG_MESSAGE ) {
		cerr << "create event SEND_REQUEST\n";
	}
	if (MsgSendPulse(3, -1, 105, SEND_REQUEST_E_ID) < 0) {
		perror("Error Receive SEND_REQUEST from Serial com: " + com);
	}
}

void Serial_Receive::create_event_reset() {
	if (SHOW_DEBUG_MESSAGE) {
		cerr << "Create Event RESET\n";
	}
	if (com == 1) {
		if (MsgSendPulse(3, -1, 101, ESTOP_RESET_SYSTEM2_E_ID) < 0) {
			perror("Error Receive RESET from Serial com: " + com);
		}
	} else {
		if (MsgSendPulse(3, -1, 102, ESTOP_RESET_SYSTEM3_E_ID) < 0) {
			perror("Error Receive RESET from Serial com: " + com);
		}
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
	if (com == 1) {
			if (MsgSendPulse(3, -1, 101, ESTOP_SYSTEM2_E_ID) < 0) {
				perror("Error Receive ESTOP from Serial com: " + com);
			}
		} else {
			if (MsgSendPulse(3, -1, 102, ESTOP_SYSTEM3_E_ID) < 0) {
				perror("Error Receive ESTOP from Serial com: " + com);
			}
		}
}

void Serial_Receive::shutdown() {
	thread_run = false;
}
