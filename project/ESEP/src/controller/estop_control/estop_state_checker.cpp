/*
 * estop_state_checker.cpp
 *
 *  Created on: 06.01.2017
 *      Author: abw181
 */

#include "estop_state_checker.h"

	Estop_state_checker::Estop_state_checker() {
      estop_this = true;
      estop_system2 = true;
      estop_system3 = true;
	}

	bool Estop_state_checker::transition_ok(){
		return estop_this && estop_system2 && estop_system3;
	}

void Estop_state_checker::set_state_status(int estop_system, bool ok) {
	switch (estop_system) {
	case 1:
		estop_this = ok;
		break;
	case 2:
		estop_system2 = ok;
		break;
	case 3:
		estop_system3 = ok;
		break;
	default:
		if (SHOW_DEBUG_MESSAGES) {
			cerr << "False Estop System Nr. \n";
		}
		break;
	}
}
