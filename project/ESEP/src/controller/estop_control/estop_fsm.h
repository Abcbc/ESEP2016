/*
 * estop_fsm.h
 *
 *  Created on: 06.01.2017
 *      Author: abw181
 */

#ifndef ESTOP_FSM_H_
#define ESTOP_FSM_H_

#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/Dispatcher.cpp"
#include "src/controller/event_table.h"
#include "estop_state_checker.h"
#include <iostream>

#define CON_ID 3
#define PRIO -1
#define CODE 5
// Zeigt Debug Msg an 1. Aus = 0
#define SHOW_DEBUG_MESSAGES 1

class Estop_fsm;
struct EstopData {
	EstopData(int estop_system, Estop_state_checker* state_checker, Estop_fsm* estop_f) :
            	estop_nr(estop_system), estate_state_check(state_checker), estop_fsm(estop_f) {

    }
    int estop_nr;
    Estop_state_checker* estate_state_check;
    Estop_fsm* estop_fsm;
};

using namespace std;

class Estop_fsm: public State {
private:
	struct MyState {
		virtual void idle() {
		}
		virtual void full_stop() {
		}
		virtual void estop_released() {
		}
		virtual void reset() {
		}
		EstopData* data;
	}*statePtr;

	struct Idle: public MyState {
		Idle() { // Entry
		}
		virtual void idle() {
		}
		virtual void full_stop() {
				new (this) Full_stop;
		}
	};

	struct Full_stop: public MyState {
		Full_stop() { // Entry
		}
		virtual void estop_released() {
				new (this) Estop_released;
		}
	};

	struct Estop_released: public MyState {
		Estop_released() { // Entry
		}
		virtual void reset() {
				new (this) Reset;
		}
	};

	struct Reset: public MyState {
		Reset() { // Entry
		}
		virtual void idle() {
				new (this) Idle;
		}
	};

	Idle stateMember;
	EstopData contextdata;

public:
	Estop_fsm(int estop_system_nr, Estop_state_checker* estate_state): statePtr(&stateMember),
	contextdata(estop_system_nr, estate_state ,this){
		statePtr->data = &contextdata;
		statePtr->idle();
		Dispatcher *d = Dispatcher::getInstance();
		d->addListener(this, ESTOP_RELEASED_THIS_E_ID);
		d->addListener(this, ESTOP_RELEASED_SYSTEM2_E_ID);
		d->addListener(this, ESTOP_RELEASED_SYSTEM3_E_ID);
		d->addListener(this, BUTTON_RESET_E_ID);
		d->addListener(this, BUTTON_RESET_E_ID);
		d->addListener(this, BUTTON_RESET_SYSTEM2_E_ID);
		d->addListener(this, BUTTON_RESET_SYSTEM3_E_ID);
		d->addListener(this, BUTTON_START_E_ID);
		d->addListener(this, BUTTON_START_INCOMMING_E_ID);
		d->addListener(this, ESTOP_THIS_E_ID);
		d->addListener(this, ESTOP_SYSTEM2_E_ID);
		d->addListener(this, ESTOP_SYSTEM3_E_ID);
	}
   ~Estop_fsm();

   struct MyState* getState() {
       return statePtr;
   }

    void ESTOP_THIS() {
	    // TODO Transition
	}
    void ESTOP_SYSTEM2() {
	}
    void ESTOP_SYSTEM3() {
	}
    void ESTOP_RELEASED_THIS() {
	}
    void ESTOP_RELEASED_SYSTEM2() {
	}
    void ESTOP_RELEASED_SYSTEM3() {
	}
    void BUTTON_RESET() {
	}
    void BUTTON_RESET_SYSTEM2() {
	}
    void BUTTON_RESET_SYSTEM3() {
	}
    void BUTTON_START() {
	}
    void BUTTON_INCOMMING() {
	}
};


#endif /* ESTOP_FSM_H_ */
