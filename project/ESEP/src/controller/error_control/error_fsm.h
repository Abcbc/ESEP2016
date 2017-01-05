/*
 * error_fsm.h
 *	Die Error FSM bearbeitet Error Events.
 *  Created on: 01.05.2017
 *      Author: Julian Magierski
 *      Version: 0.1
 */

#ifndef ERROR_FSM_H_
#define ERROR_FSM_H_

#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/Dispatcher.cpp"
#include "src/controller/event_table.h"
#include <iostream>

#define CON_ID 3
#define PRIO -1
#define CODE 5

class Error_fsm: public State {
private:

	struct MyState {
		virtual void ok(Error_fsm* err) {}
		virtual void entry(Error_fsm* err) {}
		virtual void slide_full(Error_fsm* err) {}
		virtual void lost_puk(Error_fsm* err) {}
		virtual void puk_to_many(Error_fsm* err) {}
		virtual void unknown_puk(Error_fsm* err) {}
		virtual void rdy_taking(Error_fsm* err) {}
		virtual void start(Error_fsm* err) {}
		virtual void error_acknowledged(Error_fsm* err) {}
		virtual void estop_active(Error_fsm* err) {}
		MyData* data;
	}*statePtr;

	struct Idle: public Error_fsm {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_NORMAL_E_ID);
			err->setHistory(this);
		}
		virtual void lost_puk(Error_fsm* err) {
			new (this) slide_full;
		}
		virtual void slide_full(Error_fsm* err) {
			new (this) slide_full;
		}
		virtual void puk_to_many(Error_fsm* err) {
			new (this) slide_full;
		}
		virtual void unknown_puk(Error_fsm* err) {
			new (this) slide_full;
		}
		virtual void rdy_taking(Error_fsm* err) {
			new (this) slide_full;
		}
		virtual void start(Error_fsm* err) {
			new (this) slide_full;
		}
	};

	Idle startState;

	Error_fsm(): statePtr(&startState),history_()  {
		Dispatcher *d = Dispatcher::getInstance();
		d->addListener(this, ERR_LOST_PUK_E_ID);
		d->addListener(this, ERR_TO_MANY_PUK_E_ID);
		d->addListener(this, ERR_SLIDE_FULL_E_ID);
		d->addListener(this, ERR_UNDEFINED_PUK_E_ID);
		d->addListener(this, RDY_TAKING_E_ID);
		d->addListener(this, ESTOP_OK_E_ID);
		d->addListener(this, BUTTON_RESET_E_ID);
		d->addListener(this, BUTTON_START_E_ID);
		d->addListener(this, ESTOP_THIS_E_ID);
		d->addListener(this, ESTOP_SYSTEM2_E_ID);
		d->addListener(this, ESTOP_SYSTEM3_E_ID);
	}

	void* history_;

	void setHistory(MyState* ptr) {
		history_ = *((void**) ptr);
	}

	void* getStateFromHistory_() {
		return history_;
	}

	public:

	struct MyState* getState(){
		return statePtr;
	}

	static Error_fsm* get_instance() {
		static Error_fsm instance_;
		return &instance_;
	}

	virtual void start() {
		statePtr->start();
	}

		void ERR_LOST_PUK() {
			statePtr->lost_puk(this);
			statePtr->entry(this);
		}

		void ERR_TO_MANY_PUK() {
			statePtr->puk_to_many(this);
			statePtr->entry(this);
		}

		void ERR_SLIDE_FULL() {
			statePtr->slide_full(this);
			statePtr->entry(this);
		}

		void ERR_UNDEFINED_PUK() {
			statePtr->unknown_puk(this);
			statePtr->entry(this);
		}

		void RDY_TAKING() {
			statePtr->rdy_taking(this);
			statePtr->entry(this);
		}

		void ESTOP_OK() {
			statePtr->estop_active(this);
			statePtr->entry(this);
		}

		void BUTTON_RESET() {
			statePtr->error_acknowledged(this);
			statePtr->entry(this);
		}

		void BUTTON_START() {
			statePtr->start(this);
			statePtr->entry(this);
		}

		void ESTOP_THIS() {
			statePtr->estop_active(this);
			statePtr->entry(this);
		}

		void ESTOP_SYSTEM2() {
			statePtr->estop_active(this);
			statePtr->entry(this);
		}

		void ESTOP_SYSTEM3() {
			statePtr->estop_active(this);
			statePtr->entry(this);
		}

	};

#endif /* ERROR_FSM_H_ */
