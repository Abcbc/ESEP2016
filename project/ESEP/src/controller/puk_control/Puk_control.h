/*
 * Puk_control.h
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#ifndef PUK_CONTROL_H_
#define PUK_CONTROL_H_

#include <list>
#include "src/lib/dispatcher/Dispatcher.cpp"
#include "src/lib/dispatcher/State.cpp"


using namespace std;
class Puk_fsm;

class Puk_control : public State {
private:
	static Puk_control* instance_;
	static pthread_mutex_t init_mtx;

	list<Puk_fsm *> puk_list_;
	Dispatcher* dispatcher_;
	
	Puk_control();

	struct myState {
		virtual bool check(int pukType){return false;}
	}*statePtr;
	
	struct Idle : public myState {
		virtual bool check(int pukType){
			// TODO: korrekte Werte für puks eintragen 40 = oben ohne metall
			if (pukType == 40){
				new (this) Ohne_metall_1;
				return true;
			} else {
				return false;
			}
		}
	};
	
	struct Ohne_metall_1 : public myState{
		virtual bool check(int pukType){
			// TODO: korrekte Werte für puks eintragen 40 = oben ohne metall
			if (pukType == 40){
				new (this) Ohne_metall_2;
				return true;
			} else {
				return false;
			}
		}
	};
	
	struct Ohne_metall_2 : public myState{
		virtual bool check(int pukType){
			// TODO: korrekte Werte für puks eintragen 41 = oben ohne metall
			if (pukType == 41){
				new (this) Idle;
				return true;
			} else {
				return false;
			}
		}
	};
	
	Idle startState;
	
	
public:
	const int systemType = 1;

	~Puk_control();

	static Puk_control* get_instance();
	
	void delete_puk(Puk_fsm *p);
	void send_puk(Puk_fsm *p);
	void create_puk(int pukType);
	bool sequenz_group(int pukType);
	
	virtual void LIGHT_BARRIER_ENTRY_CLOSE();
};

#endif /* PUK_CONTROL_H_ */
