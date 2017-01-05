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
#include "config.h"

#define CON_ID 3
#define PRIO -1
#define CODE 5

#define HEIHT_MEASUREMENT_TOLERANCE 2000
#define SWITCH_TO_TOLERANCE 600

struct MyData {
    MyData(int d, Puk_control* pc, Puk_fsm_dummy* pf, Dispatcher* dis,
            Tick_timer* t) :
            pukType(d), puk_control(pc), puk_fsm_dummy(pf), dispatcher(dis), tick_timer(
                    t), timer_id(-1) {

    }
    int pukType;
    Puk_control* puk_control;
    Puk_fsm_dummy* puk_fsm_dummy;
    Dispatcher* dispatcher;
    Tick_timer* tick_timer;
    int timer_id;
};
using namespace std;
class Puk_fsm_dummy: public State {
private:

    struct MyState {
        virtual bool start() {
            return false;
        }
        virtual bool start_group() {
            return false;
        }
        virtual bool is_state_3(){
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
        virtual bool light_barrier_ramp_close() {
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
        virtual bool err_undefined_puk() {
            return false;
        }
        virtual bool send_ok() {
            return false;
        }
        virtual bool new_puk() {
            return false;
        }
        virtual bool timer_exit_out() {
            return false;
        }
        virtual bool timer_group_out(){
            return false;
        }
        virtual bool button_start(){
            return false;
        }
        MyData* data;
    }*statePtr;

    struct Start: public MyState {
        Start() {
            cout << "State: start" << endl;
        }
        virtual bool start() {
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_NORMAL_E_ID);
            return true;
        }
        virtual bool start_group() {
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_NORMAL_E_ID);
            data->timer_id = data->tick_timer->start_timer(SHORT_DURATION);
            data->puk_control->accept_new_puk = false;
            new (this) Puk_1;
            return true;
        }
        virtual bool light_barrier_entry_open() {
            data->timer_id = data->tick_timer->start_timer(
                    HEIGHT_MEASURE_DURATION);
            new (this) Active;
            return true;
        }
    };

    struct Active: public MyState {
        Active() {
            cout << "State: Active" << endl;
        }
        virtual bool height_sensor_measure_start() {
            int duration = data->tick_timer->stop_timer(data->timer_id);
            cout << "Timer duration: " << duration << endl;
            if (duration < HEIHT_MEASUREMENT_TOLERANCE)
            {
                new (this) In_Height_Measurement;
            } else
            {
                new (this) Error;
            }
            return true;
        }
    };

    struct In_Height_Measurement: public MyState {
        In_Height_Measurement() {
            cout << "State: In Height Measurement" << endl;
        }

        virtual bool identified_puk() {

            data->pukType = Height_Measurement::get_type();

            bool sequence = data->puk_control->sequenz_group(data->pukType);

            cout << "PukType: " << data->pukType << endl;

            if (sequence)
            {
                data->timer_id = data->tick_timer->start_timer(
                        HEIGHT_MEASURE_TO_SWITCH_DURATION);
                MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
                new (this) To_Switch;
                return true;
            } else if (data->pukType > 32)
            {
                data->timer_id = data->tick_timer->start_timer(
                        HEIGHT_MEASURE_TO_RAMP_DURATION);
                new (this) Sortout;
                return true;
            }

            if (SYSTEM_NUMBER == 1)
            {
                if (data->pukType < 16)
                {
                    data->timer_id = data->tick_timer->start_timer(
                            HEIGHT_MEASURE_TO_RAMP_DURATION);
                    new (this) Sortout;
                } else
                {
                    data->timer_id = data->tick_timer->start_timer(
                            HEIGHT_MEASURE_TO_SWITCH_DURATION);
                    MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
                    new (this) To_Switch;
                }
                return true;
            } else if (SYSTEM_NUMBER == 2)
            {
                if (data->pukType > 15)
                {
                    data->timer_id = data->tick_timer->start_timer(
                            HEIGHT_MEASURE_TO_RAMP_DURATION);
                    new (this) Sortout;
                } else
                {
                    data->timer_id = data->tick_timer->start_timer(
                            HEIGHT_MEASURE_TO_SWITCH_DURATION);
                    MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
                    new (this) To_Switch;
                }
                return true;
            }
            return true;
        }

        virtual bool err_undefined_puk() {
            new (this) Error;
            return true;
        }
    };

    struct Sortout: public MyState {
        Sortout() {
            cout << "State: Sortout" << endl;
        }
        virtual bool light_barrier_ramp_close() {
            data->tick_timer->stop_timer(data->timer_id);
            new (this) Kill_Puk;
            return true;
        }
    };

    struct To_Switch: public MyState {
        To_Switch() {
            cout << "State: To_Switch" << endl;
        }
        virtual bool light_barrier_switch_close() {
            data->tick_timer->stop_timer(data->timer_id);
            data->timer_id = data->tick_timer->start_timer(
                    SWITCH_TO_EXIT_DURATION);
            new (this) To_Exit;
            return true;
        }
    };

    struct To_Exit: public MyState {
        To_Exit() {
            cout << "State: To_Exit" << endl;
        }
        virtual bool light_barrier_exit_close() {
            int duration = data->tick_timer->stop_timer(data->timer_id);
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_E_ID);
            cout << "Timer duration: " << duration << endl;
            if (duration > SWITCH_TO_TOLERANCE)
            {
                new (this) Error;
            } else
            {
                MsgSendPulse(CON_ID, PRIO, CODE, SEND_WANT_E_ID);
                new (this) Transmit;
            }
            return true;
        }
    };

    struct Transmit: public MyState {
        Transmit() {
            cout << "State: Transmit" << endl;
        }
        virtual bool send_ok() {
            cout << "Send ok" << endl;
            // Send pukType via serial
            Serial_Manager* sm = Serial_Manager::get_instance();
            if (SYSTEM_NUMBER == 1)
            {
                sm->send_to_system2(data->pukType);
            } else if (SYSTEM_NUMBER == 2)
            {
                sm->send_to_system3(data->pukType);
            }
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_E_ID);
            Tick_timer* t = Tick_timer::get_instance();
            t->start_timer(EXIT_DURATION);
            new (this) To_Next_Geme;
            return true;
        }
    };

    struct To_Next_Geme: public MyState {
        To_Next_Geme() {
            cout << "State: To_Next_Geme" << endl;
        }
        virtual bool timer_exit_out() {
            cout << "Puk count: " << data->puk_control->puk_count() << endl;
            if (data->puk_control->puk_count() == 1)
            {
                MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_IDLE_E_ID);
            }
            new (this) Kill_Puk;
            return true;
        }
    };

    struct Error: public MyState {
        Error() {
            cerr << "State: Error" << endl;
            new (this) Kill_Puk;
        }
    };

    struct Kill_Puk: public MyState {
        Kill_Puk() {
            cerr << "State: Kill Puk" << endl;
            data->puk_control->delete_puk(data->puk_fsm_dummy);
        }
    };

    struct Puk_1: public MyState {
        Puk_1() {
            cout << "State: Puk_1" << endl;
        }
        virtual bool timer_group_out() {
            cout << "timer_group_out" << endl;
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_IDLE_E_ID);
            data->puk_control->accept_new_puk = true;
            data->puk_control->SEND_REQUEST();
            return true;
        }
        virtual bool new_puk() {
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_NORMAL_E_ID);
            data->timer_id = data->tick_timer->start_timer(SHORT_DURATION);
            data->puk_control->accept_new_puk = false;
            new (this) Puk_2;
            return true;
        }
    };

    struct Puk_2: public MyState {
        Puk_2() {
            cout << "State: Puk_2" << endl;
        }
        virtual bool timer_group_out() {
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_IDLE_E_ID);
            data->puk_control->accept_new_puk = true;
            data->puk_control->SEND_REQUEST();
            return true;
        }
        virtual bool new_puk() {
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_NORMAL_E_ID);
            data->timer_id = data->tick_timer->start_timer(LONG_DURATION);
            data->puk_control->accept_new_puk = false;
            new (this) Puk_3;
            return true;
        }
    };

    struct Puk_3: public MyState {
            Puk_3() {
                cout << "State: Puk_3" << endl;
            }
            virtual bool timer_group_out() {
                MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_IDLE_E_ID);
                data->puk_control->accept_new_puk = true;
                data->puk_control->SEND_REQUEST();
                return true;
            }
            virtual bool light_barrier_switch_close(){
                static int counter = 0;
                counter++;
                MsgSendPulse(CON_ID, PRIO, CODE, SWITCH_OPEN_E_ID);
                if(counter == 3){
                    counter = 0;
                    new (this) Switch_Open;
                }
                return true;
            }
            virtual bool is_state_3(){
                return true;
            }
        };

    struct Switch_Open: public MyState {
        Switch_Open(){
            cout << "State: Switch_Open" << endl;
        }
        virtual bool light_barrier_exit_close(){
            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_E_ID);
            new (this) Exit_Belt_3;
            return true;
        }
    };

    struct Exit_Belt_3: public MyState {
        Exit_Belt_3(){
            cout << "State: Exit_Belt_3" << endl;
        }
        virtual bool button_start(){

            MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_E_ID);
//            data->puk_control->accept_new_puk = true;
            if(data->puk_control->puk_count() == 0){
                MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_IDLE_E_ID);
            }
            new (this) Kill_Puk;
            return true;
        }
    };

    Start stateMember;
    MyData contextdata;

public:
    Puk_fsm_dummy(int Type);
    virtual ~Puk_fsm_dummy();

    struct MyState* getState() {
        return statePtr;
    }

    virtual void start() {
        statePtr->start();
    }
    virtual void start_group() {
        statePtr->start_group();
    }
    virtual bool is_state_3(){
        return statePtr->is_state_3();
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
