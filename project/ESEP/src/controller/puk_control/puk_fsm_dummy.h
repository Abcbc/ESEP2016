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
        virtual void start() {
        }
        virtual void light_barrier_entry_open() {
        }
        virtual void light_barrier_switch_close() {
        }
        virtual void light_barrier_exit_close() {
        }
        virtual void height_sensor_measure_start() {
        }
        virtual void height_sensor_measure_finished(){}
        virtual void send_ok(){}
        MyData* data;
    }*statePtr;

    struct Start: public MyState {
        Start() {
            cout << "State: start" << endl;
        }
        virtual void start() {
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_E_ID);
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_NORMAL_E_ID);
            data->dispatcher->addListener(data->puk_fsm_dummy,
                    LIGHT_BARRIER_ENTRY_OPEN_E_ID);
        }
        virtual void light_barrier_entry_open() {
            data->dispatcher->remListeners(data->puk_fsm_dummy,
                    LIGHT_BARRIER_ENTRY_OPEN_E_ID);
            new (this) Active;
        }
    };

    struct Active: public MyState {
        Active() {
            cout << "State: Active" << endl;
            data->dispatcher->addListener(data->puk_fsm_dummy,
                    HEIGHT_SENSOR_MEASURE_START_E_ID);
        }
        virtual void height_sensor_measure_start() {
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_SLOW_E_ID);
            data->dispatcher->remListeners(data->puk_fsm_dummy,
                                HEIGHT_SENSOR_MEASURE_START_E_ID);
            new (this) Measure_Height;
        }
    };

    struct Measure_Height: public MyState {
        Measure_Height() {
            cout << "in Height Measurement" << endl;
            data->dispatcher->addListener(data->puk_fsm_dummy,
                    HEIGHT_SENSOR_MEASURE_FINISHED_E_ID);
        }
        virtual void height_sensor_measure_finished() {
            cout << "Height Measurement finished" << endl;
            MsgSendPulse(CON_ID, PRIO, CODE, TIMER_MEASURE_OUT_E_ID);
            data->dispatcher->remListeners(data->puk_fsm_dummy,
                    HEIGHT_SENSOR_MEASURE_FINISHED_E_ID);
            MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
            new (this) Exit;
        }
    };

    struct Switch : public MyState {
        Switch(){
            data->dispatcher->addListener(data->puk_fsm_dummy, LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
        }

        virtual void light_barrier_switch_close(){
            data->dispatcher->remListeners(data->puk_fsm_dummy, LIGHT_BARRIER_SWITCH_CLOSE_E_ID);
            MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
            new (this) Exit;
        }
    };


    struct Exit: public MyState {
        Exit() {
            cout << "State: exit" << endl;
            data->dispatcher->addListener(data->puk_fsm_dummy,
                    LIGHT_BARRIER_EXIT_CLOSE_E_ID);
        }
        virtual void light_barrier_exit_close() {
        	data->dispatcher->remListeners(data->puk_fsm_dummy,
        	                    LIGHT_BARRIER_EXIT_CLOSE_E_ID);
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_E_ID);
            MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_CLOSE_E_ID);
            new (this) Transmit;
        }
    };

    struct Transmit: public MyState {
    	Transmit() {
    		cout << "State: Transmit"
    		data->dispatcher->addListener(data->puk_fsm_dummy, SEND_OK_E_ID);
    		MsgSendPulse(CON_ID, PRIO, CODE, SEND_WANT_E_ID);
    	}
    	virtual void send_ok(){
    		data->dispatcher->remListeners(data->puk_fsm_dummy, SEND_OK_E_ID);
    		// Send pukType via serial
    		Serial_Manager* sm = Serial_Manager::get_instance(false);
    		sm->send_to_system2(data->pukType);
    	}
    };

    Start stateMember;
    MyData contextdata;

public:
    Puk_fsm_dummy(int Type);
    virtual ~Puk_fsm_dummy();
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
    virtual void HEIGHT_SENSOR_MEASURE_FINISHED(){
        statePtr->height_sensor_measure_finished();
    }
    virtual void SEND_OK(){
    	statePtr->send_ok();
    }
};

#endif /* PUK_FSM_DUMMY_H_ */
