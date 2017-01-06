/*
 * estop_controller.h
 *
 *  Created on: 06.01.2017
 *      Author: abw181
 */

#ifndef ESTOP_CONTROLLER_H_
#define ESTOP_CONTROLLER_H_

#include <iostream>
#include "estop_fsm.h"
#include "estop_state_checker.h"

class Estop_controller {
public:
	/*
	 * Returns the pointer of the instance
	 * @return
	 */
	static Estop_controller* get_instance();


private:
	Estop_controller();
	virtual ~Estop_controller();
	Estop_controller(const Estop_controller& other);
	Estop_controller& operator=(const Estop_controller& other);
	static Estop_controller* instance_;
	static pthread_mutex_t init_mtx;

    Estop_state_checker* estop_state_checker;

    Estop_fsm* estop_this;
    Estop_fsm* estop_system2;
    Estop_fsm* estop_system3;

};


#endif /* ESTOP_CONTROLLER_H_ */
