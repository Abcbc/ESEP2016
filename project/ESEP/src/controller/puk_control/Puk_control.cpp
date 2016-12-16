/*
 * Puk_control.cpp
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#include "src/controller/puk_control/puk_fsm.h"

Puk_control::Puk_control(int systemType, Dispatcher* d) :
		dispatcher(d), systemType(systemType) {
	puk_list.push_back(new Puk_fsm(this, dispatcher));
}

Puk_control::~Puk_control() {
	// TODO Auto-generated destructor stub
}

void Puk_control::register_for_event(Puk_fsm *p, int event_id){

}

void Puk_control::unregister_for_event(Puk_fsm *p, int event_id){

}

void Puk_control::delete_puk(Puk_fsm* p) {
	std::cout << "Deleting Puk" << std::endl;
}

void Puk_control::send_puk(Puk_fsm* p) {
	std::cout << "Sending Puk to Serial Manager" << std::endl;
}

void Puk_control::create_puk(Puk_fsm *p) {
	std::cout << "Create Puk" << std::endl;
}

bool Puk_control::sequenz_group() {
	return true;
}
