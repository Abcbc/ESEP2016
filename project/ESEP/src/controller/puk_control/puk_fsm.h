/*
 * puk_fsm.h
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#ifndef PUK_FSM_H_
#define PUK_FSM_H_

#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include "Puk_fsm_data.h"
#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/Dispatcher.cpp"
#include "src/controller/event_table.h"
#include "src/controller/puk_control/Puk_control.h"

#define CON_ID 3
#define PRIO -1
#define CODE 5

class Puk_fsm;
struct Data {
	Data(int d, Puk_control* pc, Puk_fsm* pf, Dispatcher* dis) :
			pukType(d), puk_control(pc), puk_fsm(pf), dispatcher(dis) {

	}
	int pukType;
	Puk_control* puk_control;
	Puk_fsm* puk_fsm;
	Dispatcher* dispatcher;
};

class Puk_fsm {
private:

	struct MyState {
		virtual void action_do() {
		}
		virtual void exit() {
		}
		virtual void light_barrier_entry_close() {
		}
		virtual void light_barrier_entry_open() {
		}
		virtual void light_barrier_switch_open() {
		}
		virtual void light_barrier_switch_close() {
		}
		virtual void light_barrier_exit_open() {
		}
		virtual void light_barrier_exit_close() {
		}
		virtual void h_s_measure_start() {
		}
		virtual void timer_entry_out() {
		}
		virtual void timer_group_out() {
		}
		virtual void timer_measure_out() {
		}
		virtual void timer_exit_out() {
		}
		virtual void identified_register() {
		}
		virtual void identified_puk() {
		}
		virtual void switch_close() {
		}
		virtual void send_ok() {
		}
		virtual void rdy_taking_ok() {
		}
		virtual void err_undefined_puk_ok() {
		}
		virtual void err_lost_puk_ok() {
		}
		virtual void err_too_many_puk_ok() {
		}

		Data* data;
	}*statePtr;

	struct Waiting_for_go: public MyState {
		Waiting_for_go() { // Entry
			// TODO: register for LIGHT_BARRIER_ENTRY_CLOSE
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_ENTRY_CLOSE_E_ID);
		}
		virtual void light_barrier_entry_close() {
			int systemType = data->puk_control->systemType;
			if ((systemType == 1) || (systemType == 2)) {
				new (this) Sequenz_CTRL;
			} else if (systemType == 3) {
				new (this) Go;
			}
		}
		virtual void action_do() {
			// TODO: Update Puk Data on System 1
			// TODO: Update data on system 2|3
			int systemType = data->puk_control->systemType;
			if (systemType == 1) {
				data->puk_control->create_puk(data->puk_fsm);
			} else if ((systemType == 3) || (systemType == 2)) {
				data->puk_control->create_puk(data->puk_fsm);
			}
		}
	};

	struct Sequenz_CTRL: public MyState {
		Sequenz_CTRL() {
			// Get Sequenz from sequenz ctrl
			bool sequenz_correct = data->puk_control->sequenz_group();
			if (sequenz_correct) {
				new (this) Go_Group;
			} else {
				new (this) Go;
			}
		}
	};

	struct Go_Group: public MyState {
		Go_Group() {
			MsgSendPulse(CON_ID, PRIO, CODE, TIMER_GROUP_E_ID);
			// TODO: Register for TIMER_GROUP_OUT
			data->puk_control->register_for_event(data->puk_fsm,
					TIMER_GROUP_OUT_E_ID);
		}
		virtual void exit() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_FAST_E_ID);
		}
	};

	struct Go: public MyState {
		Go() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_FAST_E_ID);
			// TODO: register for light barrier entry open
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_ENTRY_OPEN_E_ID);
		}
		virtual void light_barrier_entry_open() {
			new (this) Height_Sensor_Measure_Register;
		}
	};

	struct Height_Sensor_Measure_Register: public MyState {
		Height_Sensor_Measure_Register() {
			// TODO: register for H_S_MEASURE_START
			data->puk_control->register_for_event(data->puk_fsm,
					HEIGHT_SENSOR_MEASURE_START_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, TIMER_ENTRY_E_ID);
			// TODO: register for TIMER_ENTRY_OUT
			data->puk_control->register_for_event(data->puk_fsm,
					TIMER_ENTRY_OUT_E_ID);
		}
		virtual void h_s_measure_start() {
			new (this) Height_Sensor_Measure_Active;
		}
		virtual void timer_entry_out() {
			// TODO: unregister H_S_MEASURE_START
			data->puk_control->unregister_for_event(data->puk_fsm,
					HEIGHT_SENSOR_MEASURE_START_E_ID);
			new (this) Err_Lost_Puk;
		}
		virtual void timer_group_out() {
			if (data->puk_control->systemType == 3) {
				new (this) Group_Stop;
			}
		}
	};

	struct Group_Stop: public MyState {
		Group_Stop() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_E_ID);
		}
		virtual void timer_entry_out() {
			// TODO: unregister H_S_MEASURE_START
			data->puk_control->unregister_for_event(data->puk_fsm,
					HEIGHT_SENSOR_MEASURE_START_E_ID);
		}
		virtual void h_s_measure_start() {
			new (this) Height_Sensor_Measure_Active;
		}
	};

	struct Height_Sensor_Measure_Active: public MyState {
		Height_Sensor_Measure_Active() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_SLOW_E_ID);
		}
		virtual void action_do() {
			// TODO: [NO_TIMER_ENTRY_OUT] Check if Puk is in tolerance of timer. If not throw error.
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_E_ID);
			new (this) Err_Too_Many_Chance_HM;
		}
		virtual void timer_entry_out() {
			new (this) Expect_Puk;
		}
	};

	struct Expect_Puk: public MyState {
		virtual void actio_do() {
			// TODO: register IDENTIFIED_PUK
			data->puk_control->register_for_event(data->puk_fsm,
					IDENTIFIED_PUK_E_ID);
			// TODO: register IDENTIFIED_FALSE_PUK
			data->puk_control->register_for_event(data->puk_fsm,
					IDENTIFIED_FALSE_PUK_E_ID);
		}
		virtual void identified_register() {
			// TODO: update puk_type
			new (this) Err_Undefined_Puk;
		}
		virtual void identified_puk() {
			bool sequenz_correct = data->puk_control->sequenz_group();
			if (sequenz_correct) {
				// TODO: update PUK_DATA
				new (this) To_Switch;
			} else {
				// TODO: update PUK_DATA
				new (this) Sortout;
			}
		}
	};

	struct To_Switch: public MyState {
		To_Switch() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_FAST_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, TIMER_MEASURE_E_ID);
		}
		virtual void action_do() {
			// TODO: register for L_B_S_Close
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
			// TODO: register for TIMER_MEASURE_OUT
			data->puk_control->register_for_event(data->puk_fsm,
					TIMER_MEASURE_OUT_E_ID);
		}
		virtual void timer_measure_out() {
			new (this) Err_Lost_Puk;
		}
		virtual void light_barrier_switch_close() {
			new (this) Switch_ok;
		}
	};

	struct Switch_ok: public MyState {
		virtual void action_do() {
			// TODO: [NO_TIMER_MEASURE_OUT] id TIMER_MEASURE_OUT is out of tolerance thorw error
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_E_ID);
			new (this) Err_Too_Many_Puk_Chance_SO;
		}
		virtual void timer_measure_out() {
			MsgSendPulse(CON_ID, PRIO, CODE, TIMER_EXIT_E_ID);
			new (this) In_Switch;
		}
	};

	struct In_Switch: public MyState {
		In_Switch() {
			MsgSendPulse(CON_ID, PRIO, CODE, PUK_SWITCH_OPEN_E_ID);
			// TODO: register for SWITCH_CLOSE
			data->puk_control->register_for_event(data->puk_fsm,
					PUK_SWITCH_CLOSE_E_ID);
		}
		virtual void action_do() {
			int systemType = data->puk_control->systemType;
			if ((systemType == 1) || (systemType == 2)) {
				MsgSendPulse(CON_ID, PRIO, CODE, TIMER_SWITCH_E_ID);
			} else if (systemType == 3) {
				MsgSendPulse(CON_ID, PRIO, CODE, TIMER_SWITCH_LONG_E_ID);
			}
		}
		virtual void switch_close() {
			new (this) Outgoing_Register;
		}
	};

	struct Outgoing_Register: public MyState {
		Outgoing_Register() {
			// TODO: register for TIMER_EXIT_OUT
			data->puk_control->register_for_event(data->puk_fsm,
					TIMER_EXIT_OUT_E_ID);
			// TODO: register for L_B_EXIT_CLOSE
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_EXIT_CLOSE_E_ID);
		}
		virtual void timer_exit_out() {
			new (this) Err_Lost_Puk;
		}
		virtual void light_barrier_exit_close() {
			new (this) Outgoing_Active;
		}
		virtual void rdy_taking_ok() {
			if (data->puk_control->systemType == 3) {
				// TODO: unregister for TIMER_EXIT_OUT
				data->puk_control->register_for_event(data->puk_fsm,
						TIMER_EXIT_OUT_E_ID);
				// TODO: unregrister for L_B_EXIT_CLOSE
				data->puk_control->register_for_event(data->puk_fsm,
						LIGHT_BARRIER_EXIT_CLOSE_E_ID);
				new (this) Kill_Puk;
			}
		}
	};

	struct Outgoing_Active: public MyState {
		virtual void action_do() {
			// TODO: [NO_TIMER_EXIT_OUT] if TIMER_EXIT_OUT is out of tolerance throw error
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_E_ID);
			new (this) Err_To_Many_Puk_Chance_OA;
		}
		virtual void timer_exit_out() {
			int systemType = data->puk_control->systemType;
			if ((systemType == 1) || (systemType == 2)) {
				new (this) Out_Check;
			} else if (systemType == 3) {
				new (this) Ready_for_taking;
			}
		}
		virtual void rdy_taking_ok() {
			if (data->puk_control->systemType == 3) {
				// TODO: unregister for TIMER_EXIT_OUT
				data->puk_control->register_for_event(data->puk_fsm,
						TIMER_EXIT_OUT_E_ID);
				// TODO: unregister L_B_EXIT_C
				data->puk_control->register_for_event(data->puk_fsm,
						LIGHT_BARRIER_EXIT_CLOSE_E_ID);
				new (this) Kill_Puk;
			}
		}
	};

	struct Out_Check: public MyState {
		Out_Check() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_E_ID);
		}
		virtual void action_do() {
			// TODO: register for L_B_EXIT_CLOSE
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_EXIT_CLOSE_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, SEND_WANT_E_ID);
		}
		virtual void send_ok() {
			// TODO: Welchen Guard hat die Transition? Mehr States!
			// If L_B_EXIT_CLOSE
			new (this) Go_Out;
			// else
			// unregister L_B_EXIT_CLOSE
			new (this) Err_Lost_Puk;
		}
	};

	struct Go_Out: public MyState {
		Go_Out() {
			// TODO: register L_B_EXIT_OPEN
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_EXIT_OPEN_E_ID);
			// TODO: send PUK_DATA via puk_contoler
		}
		virtual void exit() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_FAST_E_ID);
		}
		virtual void light_barrier_exit_open() {
			new (this) Kill_Puk;
		}
	};

	struct Ready_for_taking: public MyState {
		Ready_for_taking() {
			MsgSendPulse(CON_ID, PRIO, CODE, RDY_TAKING_E_ID);
		}
		virtual void action_do() {
			// TODO: Soll das Signal immer wieder gesendet werden
			MsgSendPulse(CON_ID, PRIO, CODE, RDY_TAKING_OK_E_ID);
			// TODO: Wann soll die Transition passieren?
			new (this) Kill_Puk;
		}
	};

	struct Sortout: public MyState {
		Sortout() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_FAST_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, TIMER_MEASURE_E_ID);
		}
		virtual void action_do() {
			// TODO: register for LIGHT_BARRIER_SWITCH_CLOSE
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
			// TODO: register for TIMER_MEASURE_OUT
			data->puk_control->register_for_event(data->puk_fsm,
					TIMER_MEASURE_OUT_E_ID);
		}
		virtual void timer_measure_out() {
			// TODO: unregister for TIMER_MEASURE_OUT
			data->puk_control->unregister_for_event(data->puk_fsm,
					TIMER_MEASURE_OUT_E_ID);
			new (this) Err_Lost_Puk;
		}
		virtual void light_barrier_switch_close() {
			new (this) Switch_Sleep_Acive;
		}
	};

	struct Switch_Sleep_Acive: public MyState {
		virtual void action_do() {
			// TODO: [NO_TIMER_MEASURE_OUT] if timer is outside of tolerance throw error
			new (this) Err_Too_Many_Puk_Chance_SS;
		}
		virtual void timer_measure_out() {
			new (this) On_Ramp;
		}
	};

	struct On_Ramp: public MyState {
		On_Ramp() {
			// TODO: register for L_B_S_OPEN
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_SWITCH_OPEN_E_ID);
			// TODO: register for SLIDE_NOT_FULL
			data->puk_control->register_for_event(data->puk_fsm,
					SLIDE_NOT_FULL_E_ID);
			// TODO register for SLIDE_FULL
			data->puk_control->register_for_event(data->puk_fsm,
					SLIDE_FULL_E_ID);
		}
		virtual void action_do() {
			// TODO: [SLIDE_NOT_FULL] if SLIDE is full throw error
			// TODO: unregister for SLIDE_NOT_FULL
			data->puk_control->unregister_for_event(data->puk_fsm,
					SLIDE_NOT_FULL_E_ID);
			new (this) Err_Slide_Full;
		}
		virtual void light_barrier_switch_open() {
			// TODO: [SLIDE_NOT_FULL] if SLIDE is is not full kill puk
			// TODO: unregister for SLIDE_FULL
			data->puk_control->unregister_for_event(data->puk_fsm,
					SLIDE_FULL_E_ID);
			new (this) Kill_Puk;
		}
	};

	struct Kill_Puk: public MyState {
		Kill_Puk() {
			// TODO: update PUK_CONTROL (Destructor of puk_fsm?)
		}
	};

	struct Err_Slide_Full: public MyState {
		Err_Slide_Full() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_SLIDE_FULL_E_ID);
		}
		virtual void action_do() {
			// TODO: register for ERR_SLIDE_FULL_OK
			data->puk_control->register_for_event(data->puk_fsm,
					ERR_SLIDE_FULL_OK_E_ID);
		}
		virtual void err_slide_full_ok() {
			new (this) Kill_Puk;
		}
	};

	struct Err_Undefined_Puk: public MyState {
		Err_Undefined_Puk() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_UNDEFINED_PUK_E_ID);
		}
		virtual void action_do() {
			// TODO: register for ERR_UNDEFINED_PUK_OK
			data->puk_control->register_for_event(data->puk_fsm,
					ERR_UNDEFINED_PUK_E_ID);
		}
		virtual void err_undefined_puk_ok() {
			new (this) Kill_Puk;
		}
	};

	struct Err_Lost_Puk: public MyState {
		Err_Lost_Puk() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_LOST_PUK_E_ID);
		}
		virtual void action_do() {
			// TODO: register for ERR_LOST_PUK_OK
			data->puk_control->register_for_event(data->puk_fsm,
					ERR_LOST_PUK_OK_E_ID);
		}
		virtual void err_lost_puk_ok() {
			new (this) Kill_Puk;
		}
	};

	struct Err_To_Many_Puk: public MyState {
		Err_To_Many_Puk() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_E_ID);
		}
		virtual void action_do() {
			// TODO: register for ERR_TOO_MANY_PUK_OK
			data->puk_control->register_for_event(data->puk_fsm,
					ERR_TOO_MANY_PUK_OK_E_ID);
		}
		virtual void err_too_many_puk_ok() {
			new (this) Kill_Puk;
		}
	};

	struct Err_To_Many_Puk_Chance_OA: public MyState {
		Err_To_Many_Puk_Chance_OA() {
			// TODO: register ERR_TOO_MANY_PUKS_OK
			data->puk_control->register_for_event(data->puk_fsm,
					ERR_TOO_MANY_PUK_OK_E_ID);
			// TODO: register L_B_EXIT_CLOSE
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_EXIT_CLOSE_E_ID);
		}
		virtual void action_do() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_OK_E_ID);
		}
		virtual void light_barrier_exit_close() {
			new (this) Outgoing_Active;
		}
		virtual void timer_exit_out() {
			// TODO: unregister L_B_EXIT_CLOSE
			data->puk_control->unregister_for_event(data->puk_fsm,
					LIGHT_BARRIER_EXIT_CLOSE_E_ID);
		}
	};

	struct Err_Too_Many_Puk_Chance_SO: public MyState {
		Err_Too_Many_Puk_Chance_SO() {
			// TODO: register ERR_TOO_MANY_PUK_OK
			data->puk_control->register_for_event(data->puk_fsm,
					ERR_TOO_MANY_PUK_E_ID);
			// TODO: register L_B_SWITCH_CLOSE
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
		}
		virtual void action_do() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_OK_E_ID);
		}
		virtual void light_barrier_switch_close() {
			new (this) Switch_ok;
		}
		virtual void timer_measure_out() {
			// TODO: unregister L_B_SWICH_CLOSE
			data->puk_control->unregister_for_event(data->puk_fsm,
					LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
		}
	};

	struct Err_Too_Many_Puk_Chance_SS: public MyState {
		Err_Too_Many_Puk_Chance_SS() {
			// TODO: register for ERR_T_M_P_OK
			data->puk_control->register_for_event(data->puk_fsm,
					ERR_TOO_MANY_PUK_OK_E_ID);
			// TODO: register for L_B_S_C
			data->puk_control->register_for_event(data->puk_fsm,
					LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
		}
		virtual void action_do() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_OK_E_ID);
		}
		virtual void light_barrier_switch_close() {
			new (this) Switch_Sleep_Acive;
		}
		virtual void timer_measure_out() {
			// TODO: unregister L_B_S_C
			new (this) Err_Lost_Puk;
		}
	};

	struct Err_Too_Many_Chance_HM: public MyState {
		Err_Too_Many_Chance_HM() {
			// TODO: register ERR_TOO_MANY_PUK_OK
			// TODO: register H_S_MEASURE_START
		}
		virtual void action_do() {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_TOO_MANY_PUK_OK_E_ID);
		}
		virtual void exit() {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_FAST_E_ID);
		}
		virtual void timer_entry_out() {
			// TODO: unregister H_S_MEASURE_START
			new (this) Err_Lost_Puk;
		}
		virtual void h_s_measure_start() {
			new (this) Height_Sensor_Measure_Active;
		}
		virtual void timer_group_out() {
			if (data->puk_control->systemType) {
				new (this) Group_Stop;
			}
		}
	};

	Waiting_for_go stateMember;
	Data contextdata;

public:
	Puk_fsm(Puk_control* pc, Dispatcher* dis) :
			statePtr(&stateMember), contextdata(0, pc, this, dis) {
		statePtr->data = &contextdata;
	}

};

#endif /* PUK_FSM_H_ */
