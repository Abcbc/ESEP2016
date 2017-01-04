/*
 * Puk_control.h
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#ifndef PUK_CONTROL_H_
#define PUK_CONTROL_H_

#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "lib/HAWThread.h"
#include "lib/dispatcher/Dispatcher.cpp"
#include "lib/dispatcher/State.cpp"
#include "config.h"

using namespace thread;
using namespace std;
class Puk_fsm_dummy;

class Puk_control: public State, public HAWThread {
private:
	static Puk_control* instance_;
	static pthread_mutex_t init_mtx;

	vector<Puk_fsm_dummy *> puk_list_;
	Dispatcher* dispatcher_;
	bool belt_is_free;

	Puk_control();

	struct myState {
		virtual bool check(int pukType) {
			return false;
		}
	}*statePtr;

	struct Idle: public myState {
		virtual bool check(int pukType) {
			// TODO: korrekte Werte für puks eintragen 40 = oben ohne metall
			cout << "Sequence Idle: " << pukType << endl;
			if (pukType == puk_hole) {
				new (this) Ohne_metall_1;
				return true;
			} else {
				return false;
			}
		}
	};

	struct Ohne_metall_1: public myState {
		virtual bool check(int pukType) {
			cout << "Sequence Ohne_metall_1: " << pukType << endl;
			// TODO: korrekte Werte für puks eintragen 40 = oben ohne metall
			if (pukType == puk_hole) {
				new (this) Ohne_metall_2;
				return true;
			} else {
				return false;
			}
		}
	};

	struct Ohne_metall_2: public myState {
		virtual bool check(int pukType) {
			cout << "Sequence Ohne_metall_2: " << pukType << endl;
			// TODO: korrekte Werte für puks eintragen 41 = oben mit metall
			if (pukType == puk_iron_core) {
				new (this) Idle;
				return true;
			} else {
				return false;
			}
		}
	};

	Idle startState;

	void execute(void*) {
		while (1) {
			usleep(10000);
		}
	}
	virtual void shutdown() {

	}

	void try_event(bool (*ptToSignal)());

public:
	const int systemType;

	~Puk_control();

	static Puk_control* get_instance();

	void delete_puk(Puk_fsm_dummy *p);
	void send_puk(Puk_fsm_dummy *p);
	void create_puk(int pukType);
	bool sequenz_group(int pukType);
	int puk_count();

// Events for puk control
	virtual void LIGHT_BARRIER_ENTRY_CLOSE();
	virtual void SEND_REQUEST();
	virtual void NEW_PUK();

// Events for puk fsm
	virtual void LIGHT_BARRIER_ENTRY_OPEN();
	virtual void LIGHT_BARRIER_SWITCH_CLOSE();
	virtual void LIGHT_BARRIER_EXIT_CLOSE();
	virtual void LIGHT_BARRIER_RAMP_CLOSE();
	virtual void HEIGHT_SENSOR_MEASURE_START();
	virtual void HEIGHT_SENSOR_MEASURE_FINISHED();
	virtual void SEND_OK();
	virtual void IDENTIFIED_PUK();
	virtual void TIMER_EXIT_OUT();
	virtual void ERR_UNDEFINED_PUK();
};

#endif /* PUK_CONTROL_H_ */
