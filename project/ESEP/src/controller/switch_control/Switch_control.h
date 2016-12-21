/*
 * Switch_control.h
 *
 *  Created on: 13.12.2016
 *      Author: aby520
 */

#ifndef SWITCH_CONTROL_H_
#define SWITCH_CONTROL_H_

#include <iostream>
#include <cstring>
#include <sys/neutrino.h>
#include "src/lib/hal/hal_component.h"
#include "src/lib/hal/puk_switch.h"
#include "src/controller/event_table.h"

#define CON 3
using namespace std;
class Switch_control {
private:

	Puk_switch* puk_switch;

	struct State {
		virtual void entry(Switch_control* s) {
		}
		virtual void switch_open(Switch_control* S) {
		}
		virtual void timer_switch_out(Switch_control* s) {
		}
		virtual void error_switch(Switch_control* s) {
		}
		virtual void error_switch_ok(Switch_control* s) {
		}
	}*statePtr;

	struct States: public State {
		virtual void error_switch(Switch_control* s){
			new (this) Error;
		}
	};

	struct Error: public State {
		virtual void entry(Switch_control* s) {
			cout << "Switch Error" << endl;
		}
		virtual void error_switch_ok(Switch_control* s) {
			void* history = s->getStateFromHistory_();
			memcpy(this, &history, 4);
		}
	};

	struct Close: public States {
		virtual void entry(Switch_control* s) {
			cout << "Close switch" << endl;
			s->puk_switch->close();
			s->setHistory_(this);
		}
		virtual void switch_open(Switch_control* s) {
			new (this) Open;
		}
	};

	struct Open: public States {
		virtual void entry(Switch_control* s) {
			cout << "Open switch" << endl;
			s->puk_switch->open();
			s->setHistory_(this);
			MsgSendPulse(CON, -1, 5, TIMER_SWITCH_E_ID);
		}
		virtual void timer_switch_out(Switch_control* s) {
			new (this) Close;
		}
	};

	void* history_;

	void setHistory_(State* ptr) {
		history_ = *((void**) ptr);
	}

	void* getStateFromHistory_() {
		return history_;
	}

	Open startState;

public:
	Switch_control() : puk_switch(Puk_switch::get_instance()),statePtr(&startState), history_(0){}

	void switch_open() {
		statePtr->switch_open(this);
		statePtr->entry(this);
	}
	void timer_switch_out() {
		statePtr->timer_switch_out(this);
		statePtr->entry(this);
	}
	void error_switch() {
		statePtr->error_switch(this);
		statePtr->entry(this);
	}
	void error_switch_ok() {
		statePtr->error_switch_ok(this);
		//statePtr->entry(this);
	}

};

#endif /* SWITCH_CONTROL_H_ */
