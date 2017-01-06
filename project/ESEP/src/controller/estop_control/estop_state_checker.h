/*
 * estop_state_checker.h
 *
 *  Created on: 06.01.2017
 *      Author: Julian
 */

#ifndef ESTOP_STATE_CHECKER_H_
#define ESTOP_STATE_CHECKER_H_

#include <iostream>
#include <stdio.h>

// Zeigt Debug Msg an 1. Aus = 0
#define SHOW_DEBUG_MESSAGES 1

using namespace std;

class Estop_state_checker {
public:

	/*
	 * Prueft ob eine Transition von State Estop Reset zu Idle ok ist.
	 * Wenn ok true. Ansonsten false.
	 */
    bool transition_ok();

    /**
     * Setzt den Status einer Estop FSM.
     * estop_system gibt das System an
     * ok, wenn true dann Save State. False unsave.
     * Save State, wenn Uebergang von Estop Reset State zu Idle State erlaubt ist
     */
    void set_state_status(int estop_system, bool ok);

	Estop_state_checker();

	~Estop_state_checker();
private:
	Estop_state_checker(const Estop_state_checker& other);
	Estop_state_checker& operator=(const Estop_state_checker& other);

	bool estop_this;

	bool estop_system2;

	bool estop_system3;

};


#endif /* ESTOP_STATE_CHECKER_H_ */
