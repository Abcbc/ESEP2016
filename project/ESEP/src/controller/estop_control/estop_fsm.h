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
		virtual void start(){
		}
		virtual void idle() {
		}
		virtual void full_stop(int system_nr) {
		}
		virtual void estop_released(int system_nr) {
		}
		virtual void reset(int system_nr) {
		}
		EstopData* data;
	}*statePtr;

	struct StartState : public MyState {
		virtual void start(){
			new (this) Idle;
		}
	};

	struct Idle: public MyState {
		Idle() { // Entry
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ESTOP_FSM STATE IDLE \n";
			}
			data->estate_state_check->set_state_status(data->estop_nr, true);
		}
		virtual void full_stop(int system_nr) {
			switch (system_nr) {
			case 1:
				if (data->estop_nr == 1) {
					if (SHOW_DEBUG_MESSAGES) {
						cerr << "ESTOP_FSM STATE FULL STOP ESTOP: 1 \n";
					}
					new (this) Full_stop;
				}
				break;
			case 2:
				if (data->estop_nr == 2) {
					if (SHOW_DEBUG_MESSAGES) {
						cerr << "ESTOP_FSM STATE FULL STOP: ESTOP 2 \n";
					}
					new (this) Full_stop;
				}
				break;
			case 3:
				if (data->estop_nr == 3) {
					if (SHOW_DEBUG_MESSAGES) {
						cerr << "ESTOP_FSM STATE FULL STOP: ESTOP 3 \n";
					}
					new (this) Full_stop;
				}
				break;
			}
		}
	};

	struct Full_stop: public MyState {
		Full_stop() { // Entry
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_ERR_E_ID);
            MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_UNACK_ERROR_E_ID);
			data->estate_state_check->set_state_status(data->estop_nr,false);
		}
		virtual void estop_released(int system_nr) {
		    switch(system_nr) {
		    case 1:
			    if(data->estop_nr == 1) {
			    	if (SHOW_DEBUG_MESSAGES) {
			    		cerr << "ESTOP_FSM STATE ESTOP RELEASED: ESTOP 1 \n";
			    	}
					new (this) Estop_released;
			    }
		    	break;
		    case 2:
			    if(data->estop_nr == 2) {
			    	if (SHOW_DEBUG_MESSAGES) {
			    		cerr << "ESTOP_FSM STATE ESTOP RELEASED: ESTOP 2 \n";
			    	}
					new (this) Estop_released;
			    }
		    	break;
		    case 3:
			    if(data->estop_nr == 3) {
			    	if (SHOW_DEBUG_MESSAGES) {
			    		cerr << "ESTOP_FSM STATE ESTOP RELEASED: ESTOP 3 \n";
			    	}
					new (this) Estop_released;
			    }
		    	break;
		    }
		}
	};

	struct Estop_released: public MyState {
		Estop_released() { // Entry
	    data->estate_state_check->set_state_status(data->estop_nr,false);
		}
		virtual void reset(int system_nr) {
		    switch(system_nr) {
		    case 1:
			    if(data->estop_nr == 1) {
			    	if (SHOW_DEBUG_MESSAGES) {
			    		cerr << "ESTOP_FSM STATE RESET: ESTOP 1 \n";
			    	}
					new (this) Reset;
			    }
		    	break;
		    case 2:
			    if(data->estop_nr == 2) {
			    	if (SHOW_DEBUG_MESSAGES) {
			    		cerr << "ESTOP_FSM STATE RESET: ESTOP 2 \n";
			    	}
					new (this) Reset;
			    }
		    	break;
		    case 3:
			    if(data->estop_nr == 3) {
			    	if (SHOW_DEBUG_MESSAGES) {
			    		cerr << "ESTOP_FSM STATE RESET: ESTOP 3 \n";
			    	}
					new (this) Reset;
			    }
		    	break;
		    }
		}
	};

	struct Reset: public MyState {
		Reset() { // Entry
		data->estate_state_check->set_state_status(data->estop_nr,true);
		}
		virtual void idle() {
			if (data->estate_state_check->transition_ok()) {
		    	if (SHOW_DEBUG_MESSAGES) {
		    		cerr << "ESTOP_FSM STATE GO TO IDLE \n";
		    	}
				MsgSendPulse(CON_ID, PRIO, CODE, ESTOP_OK_E_ID);
				new (this) Idle;
			}
		}
	};

	StartState stateMember;
	EstopData contextdata;

	Estop_state_checker* estop_state_checker;


public:
	Estop_fsm(int estop_system_nr, Estop_state_checker* estate_state): statePtr(&stateMember),
	contextdata(estop_system_nr, estate_state ,this), estop_state_checker(estate_state) {
		statePtr->data = &contextdata;
		statePtr->start();
		Dispatcher *d = Dispatcher::getInstance();
		d->addListener(this, ESTOP_RELEASED_THIS_E_ID);
		d->addListener(this, ESTOP_RELEASED_SYSTEM2_E_ID);
		d->addListener(this, ESTOP_RELEASED_SYSTEM3_E_ID);
		d->addListener(this, BUTTON_RESET_E_ID);
		d->addListener(this, BUTTON_RESET_SYSTEM2_E_ID);
		d->addListener(this, BUTTON_RESET_SYSTEM3_E_ID);
		d->addListener(this, BUTTON_START_E_ID);
		d->addListener(this, BUTTON_START_INCOMING_E_ID);
		d->addListener(this, ESTOP_THIS_E_ID);
		d->addListener(this, ESTOP_SYSTEM2_E_ID);
		d->addListener(this, ESTOP_SYSTEM3_E_ID);
	}

   struct MyState* getState() {
       return statePtr;
   }

	void start(){
		statePtr->start();
	}

    void ESTOP_THIS() {
    	statePtr->full_stop(1);
	}
    void ESTOP_SYSTEM2() {
    	statePtr->full_stop(2);
	}
    void ESTOP_SYSTEM3() {
    	statePtr->full_stop(3);
	}
    void ESTOP_RELEASED_THIS() {
    	statePtr->estop_released(1);
	}
    void ESTOP_RELEASED_SYSTEM2() {
    	statePtr->estop_released(2);
	}
    void ESTOP_RELEASED_SYSTEM3() {
    	statePtr->estop_released(3);
	}
    void BUTTON_RESET() {
    	statePtr->reset(1);
	}
    void BUTTON_RESET_SYSTEM2() {
    	statePtr->reset(2);
	}
    void BUTTON_RESET_SYSTEM3() {
    	statePtr->reset(3);
	}
    void BUTTON_START() {
    	statePtr->idle();
	}
    void BUTTON_START_INCOMING() {
    	statePtr->idle();
	}
};


#endif /* ESTOP_FSM_H_ */
