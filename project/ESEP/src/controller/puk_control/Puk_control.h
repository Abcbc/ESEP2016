/*
 * Puk_control.h
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#ifndef PUK_CONTROL_H_
#define PUK_CONTROL_H_



class Puk_fsm;
class Puk_control {
private:


public:
    const int systemType;

    Puk_control(int systemType): systemType(systemType){}
	~Puk_control(){}

	void register_for_event(Puk_fsm *p, int event_id){}
	void unregister_for_event(Puk_fsm *p, int event_id){}
	bool sequenz_group();

};

#endif /* PUK_CONTROL_H_ */
