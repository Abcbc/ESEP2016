/*
 * serial_receive.cpp
 *
 *  Created on: 11 Nov 2016
 *      Author: Julian Magierski
 */
#include "serial_receive.h"
#include "paket_protocol.h"

Serial_Receive::Serial_Receive(int fdesc_number, uint8_t* sequenznr, bool* estop) {
	fdesc_ = fdesc_number;
	sequenznummer = sequenznr;
	estop_on = estop;
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
			case ACK:
				cout << "RECEIVE: ACK, SQZ: " << header.sequenznummer << endl;
				*sequenznummer = header.sequenznummer;
				break;
			case ESTOP:
				transmit_ack(header.sequenznummer);
				cout << "RECEIVE: ESTOP, SQZ: " << header.sequenznummer << endl;
				if (!(*estop_on)) {
					*estop_on = true;
					create_event_estop();
				} else {
					cout << "ESTOP is already activ" << endl;
				}
				break;
			case RESET:
				transmit_ack(header.sequenznummer);
				cout << "RECEIVE: EGO, SQZ: " << header.sequenznummer << endl;
				if (*estop_on) {
					*estop_on = false;
					create_event_reset();
				} else {
					cout << "RESET is already activ" << endl;
				}
				break;
			case DATA:
				transmit_ack(header.sequenznummer);
				cout << "RECEIVE: Payload Paket Size:" << header.paket_size
						<< endl;
				if (readcond(this->fdesc_, &data, header.paket_size,
						header.paket_size, 0, 0) > 0) {
					transmit_ack((header.sequenznummer + 1) % MAX_SQZ);
					uint8_t seq = (header.sequenznummer + 1) % MAX_SQZ;
					cout << "RECEIVE: Erhalten DATA Ja, SQZ: " << seq << endl;
					cout << "RECEIVE DATA: " << data.data << endl;
					create_event_puk_data(&data);
				} else {
					cout << "RECEIVE: Erhalten DATA Nein" << endl;
				}
			}
		}
	}
}


int Serial_Receive::transmit_ack(int ack_sqz) {
	Header header;
	header.paket_typ = ACK;
	header.paket_size = 0;
	header.sequenznummer = ack_sqz;
	write(this->fdesc_, &header, sizeof(header));
	return 0;
}

void Serial_Receive::create_event_puk_data(void* data) {
	Data puk_data = *static_cast<Data*>(data);
	cout << "Puk data: " << puk_data.data << endl;
}

void Serial_Receive::create_event_reset() {
	cout << "Create Event RESET" << endl;
}

void Serial_Receive::create_event_estop() {
	cout << "Create Event ESTOP" << endl;
}

void Serial_Receive::shutdown() {
	thread_run = false;
}
