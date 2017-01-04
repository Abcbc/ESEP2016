/*
 * puk_fsm_dummy.h
 *
 *  Created on: 19.12.2016
 *      Author: aby520
 */

#ifndef PUK_FSM_DUMMY_H_
#define PUK_FSM_DUMMY_H_

#include <iostream>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include "Puk_fsm_data.h"
#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/Dispatcher.cpp"
#include "src/controller/event_table.h"
#include "src/controller/puk_control/Puk_control.h"
#include "src/lib/serial/serial_manager.h"
#include "timer/tick_timer.h"
#include "controller/height_measurement/height_measurement.h"

#define CON_ID 3
#define PRIO -1
#define CODE 5

struct MyData {
	MyData(int d, Puk_control* pc, Puk_fsm_dummy* pf, Dispatcher* dis) :
			pukType(d), puk_control(pc), puk_fsm_dummy(pf), dispatcher(dis) {

	}
	int pukType;
	Puk_control* puk_control;
	Puk_fsm_dummy* puk_fsm_dummy;
	Dispatcher* dispatcher;
};
using namespace std;
class Puk_fsm_dummy: public State {
private:

	struct MyState {
		virtual bool start() {
			return false;
		}
		virtual bool light_barrier_entry_open() {
			return false;
		}
		virtual bool light_barrier_switch_close() {
			return false;
		}
		virtual bool light_barrier_exit_close() {
			return false;
		}
		virtual bool height_sensor_measure_start() {
			return false;
		}
		virtual bool height_sensor_measure_finished() {
			return false;
		}
		virtual bool identified_puk() {
			return false;
		}
		virtual bool send_ok() {
			return false;
		}
		virtual bool timer_exit_out() {
			return false;
		}
		MyData* data;
	}*statePtr;

	struct Start: public MyState {
		Start() {
			cout << "State: start" << endl;
		}
		virtual bool start() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_NORMAL_E_ID);
			return true;
		}
		virtual bool light_barrier_entry_open() {
			new (this) Active;
			return true;
		}
	};

	struct Active: public MyState {
		Active() {
			cout << "State: Active" << endl;
		}
		virtual bool height_sensor_measure_start() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_SLOW_E_ID);
			new (this) Measure_Height;
			return true;
		}
	};

	struct Measure_Height: public MyState {
		Measure_Height() {
			cout << "in Height Measurement" << endl;
		}
		virtual bool identified_puk() {
			cout << "Height Measurement finished" << endl;
			data->pukType = Height_Measurement::get_instance()->get_type();
			MsgSendPulse(CON_ID, PRIO, CODE, TIMER_MEASURE_OUT_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
			new (this) Exit;
			return true;
		}
	};

	struct Switch: public MyState {

		virtual bool light_barrier_switch_close() {
						MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
			new (this) Exit;
			return true;
		}
	};

	struct Exit: public MyState {
		Exit() {
			cout << "State: exit" << endl;
		}
		virtual bool light_barrier_exit_close() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_CLOSE_E_ID);
			if (data->puk_control->systemType == 2) {
				data->puk_control->delete_puk(data->puk_fsm_dummy);
			} else {
				new (this) Transmit;
			}
			return true;
		}
	};

	struct Transmit: public MyState {
		Transmit() {
			cout << "State: Transmit" << endl;
			MsgSendPulse(CON_ID, PRIO, CODE, SEND_WANT_E_ID);
		}
		virtual bool send_ok() {
			cout << "Send ok" << endl;
			// Send pukType via serial
			Serial_Manager* sm = Serial_Manager::get_instance();
			sm->send_to_system2(data->pukType);
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_E_ID);
			Tick_timer* t = Tick_timer::get_instance();
			t->start_timer(EXIT_DURATION);
			return true;
		}
		virtual bool timer_exit_out() {
			cout << "Puk count: " <<  data->puk_control->puk_count() << endl;
			if(data->puk_control->puk_count()  == 0){
				MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_IDLE_E_ID);
			}
			data->puk_control->delete_puk(data->puk_fsm_dummy);
			return true;
		}
	};

	Start stateMember;
	MyData contextdata;

public:
	Puk_fsm_dummy(int Type);
	virtual ~Puk_fsm_dummy();

	struct MyState* getState(){
		return statePtr;
	}

	virtual void start() {
		statePtr->start();
	}
	virtual void LIGHT_BARRIER_ENTRY_OPEN() {
		statePtr->light_barrier_entry_open();
	}
	virtual void LIGHT_BARRIER_SWITCH_CLOSE() {
		statePtr->light_barrier_switch_close();
	}
	virtual void LIGHT_BARRIER_EXIT_CLOSE() {
		statePtr->light_barrier_exit_close();
	}
	virtual void HEIGHT_SENSOR_MEASURE_START() {
		statePtr->height_sensor_measure_start();
	}
	virtual void HEIGHT_SENSOR_MEASURE_FINISHED() {
		statePtr->height_sensor_measure_finished();
	}
	virtual void SEND_OK() {
		statePtr->send_ok();
	}
	virtual void IDENTIFIED_PUK() {
		statePtr->identified_puk();
	}
	virtual void TIMER_EXIT_OUT() {
		statePtr->timer_exit_out();
	}
};

#endif /* PUK_FSM_DUMMY_H_ */
