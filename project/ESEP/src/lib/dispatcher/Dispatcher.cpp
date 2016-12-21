/*
 * @file    Dispatcher.cpp
 * @author  Simon Brummer / Andre KÃ¶pke
 * @desc    Dispatcher der Idee vom Tutoriumscode.
 */
#ifndef DISPATCHER_CPP_
#define DISPATCHER_CPP_

#include "event_methods.h"
#include "src/controller/event_table.h"
#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <pthread.h>
#include "lib/HAWThread.h"
#include <pthread.h>

using namespace std;
using namespace thread;


// typedef: Method from Class "Event_methods"
typedef void (Event_methods::*method_t)(void);

class Dispatcher : public HAWThread {

private:
	map<int, method_t> _methods;
	map<int, vector<Event_methods *> > _listeners;
	int _cid, _con;
	pthread_mutex_t  _init_mtx;


private:


	Dispatcher() {
		_init_mtx = PTHREAD_MUTEX_INITIALIZER;

		// Hardwarezugriff für Mulithreading benötigt
		if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
			//TODO throw exception
		}


		if (( _cid = ChannelCreate(0)) == -1){
		    cout << "ChannelCreate() failed." << endl;
		}

		if ((_con = ConnectAttach(0, 0, _cid, 0, 0)) == -1){
		    cout << "ConnectAttach() failed." << endl;
		}

		// Add Method pointer to Call Method Array
		//LIGHT BARRIERS
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_ENTRY_OPEN_E_ID, &Event_methods::LIGHT_BARRIER_ENTRY_OPEN));
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_ENTRY_CLOSE_E_ID, &Event_methods::LIGHT_BARRIER_ENTRY_CLOSE));
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_HEIGHT_OPEN_E_ID, &Event_methods::LIGHT_BARRIER_HEIGHT_OPEN));
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_HEIGHT_CLOSE_E_ID, &Event_methods::LIGHT_BARRIER_HEIGHT_CLOSE));
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_SWITCH_OPEN_E_ID, &Event_methods::LIGHT_BARRIER_SWITCH_OPEN));
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_SWITCH_CLOSE_E_ID, &Event_methods::LIGHT_BARRIER_SWITCH_CLOSE));
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_EXIT_OPEN_E_ID, &Event_methods::LIGHT_BARRIER_EXIT_OPEN));
		_methods.insert(std::pair<int, method_t>(LIGHT_BARRIER_EXIT_CLOSE_E_ID, &Event_methods::LIGHT_BARRIER_EXIT_CLOSE));

		//MOTOR
		_methods.insert(std::pair<int, method_t>(MOTOR_STOP_E_ID, &Event_methods::MOTOR_STOP));
		_methods.insert(std::pair<int, method_t>(MOTOR_START_E_ID, &Event_methods::MOTOR_START));
		_methods.insert(std::pair<int, method_t>(MOTOR_FAST_E_ID, &Event_methods::MOTOR_FAST));
		_methods.insert(std::pair<int, method_t>(MOTOR_SLOW_E_ID, &Event_methods::MOTOR_SLOW));
		_methods.insert(std::pair<int, method_t>(MOTOR_LEFT_E_ID, &Event_methods::MOTOR_LEFT));
		_methods.insert(std::pair<int, method_t>(MOTOR_RIGHT_E_ID, &Event_methods::MOTOR_RIGHT));

		//BUTTONS
		_methods.insert(std::pair<int, method_t>(BUTTON_START_PRESSED_E_ID, &Event_methods::BUTTON_START_PRESSED));
		_methods.insert(std::pair<int, method_t>(BUTTON_START_RELEASED_E_ID, &Event_methods::BUTTON_START_RELEASED));
		_methods.insert(std::pair<int, method_t>(BUTTON_STOP_PRESSED_E_ID, &Event_methods::BUTTON_STOP_PRESSED));
		_methods.insert(std::pair<int, method_t>(BUTTON_STOP_RELEASED_E_ID, &Event_methods::BUTTON_STOP_RELEASED));
		_methods.insert(std::pair<int, method_t>(BUTTON_RESET_PRESSED_E_ID, &Event_methods::BUTTON_RESET_PRESSED));
		_methods.insert(std::pair<int, method_t>(BUTTON_RESET_RELEASED_E_ID, &Event_methods::BUTTON_RESET_RELEASED));
		_methods.insert(std::pair<int, method_t>(BUTTON_E_STOP_PRESSED_E_ID, &Event_methods::BUTTON_E_STOP_PRESSED));
		_methods.insert(std::pair<int, method_t>(BUTTON_E_STOP_RELEASED_E_ID, &Event_methods::BUTTON_E_STOP_RELEASED));

		//HOEHENMESSUNG
		_methods.insert(std::pair<int, method_t>( HEIGHT_SENSOR_MEASURE_START_E_ID, &Event_methods::HEIGHT_SENSOR_MEASURE_START));
		_methods.insert(std::pair<int, method_t>( HEIGHT_SENSOR_MEASURE_FINISHED_E_ID, &Event_methods::HEIGHT_SENSOR_MEASURE_FINISHED));
		_methods.insert(std::pair<int, method_t>( IDENTIFIED_PUK_E_ID, &Event_methods::IDENTIFIED_PUK));
		_methods.insert(std::pair<int, method_t>( IDENTIFIED_REGISTER_E_ID, &Event_methods::IDENTIFIED_REGISTER));
		_methods.insert(std::pair<int, method_t>( IDENTIFIED_FALSE_PUK_E_ID, &Event_methods::IDENTIFIED_FALSE));


		//TIMER
		_methods.insert(std::pair<int, method_t>( TIMER_RUNOUT_E_ID, &Event_methods::TIMER_RUNOUT));
		_methods.insert(std::pair<int, method_t>( TIMER_ENTRY_E_ID, &Event_methods::TIMER_ENTRY));
		_methods.insert(std::pair<int, method_t>( TIMER_ENTRY_OUT_E_ID, &Event_methods::TIMER_ENTRY_OUT));
		_methods.insert(std::pair<int, method_t>( TIMER_GROUP_E_ID, &Event_methods::TIMER_GROUP));
		_methods.insert(std::pair<int, method_t>( TIMER_GROUP_OUT_E_ID, &Event_methods::TIMER_GROUP_OUT));
		_methods.insert(std::pair<int, method_t>( TIMER_MEASURE_E_ID, &Event_methods::TIMER_MEASURE));
		_methods.insert(std::pair<int, method_t>( TIMER_MEASURE_OUT_E_ID, &Event_methods::TIMER_MEASURE_OUT));
		_methods.insert(std::pair<int, method_t>( TIMER_EXIT_E_ID, &Event_methods::TIMER_EXIT));
		_methods.insert(std::pair<int, method_t>( TIMER_EXIT_OUT_E_ID, &Event_methods::TIMER_EXIT_OUT));
		_methods.insert(std::pair<int, method_t>( TIMER_SWITCH_E_ID, &Event_methods::TIMER_SWITCH));
		_methods.insert(std::pair<int, method_t>( TIMER_SWITCH_OUT_E_ID, &Event_methods::TIMER_SWITCH_OUT));
		_methods.insert(std::pair<int, method_t>( TIMER_SWITCH_LONG_E_ID, &Event_methods::TIMER_SWITCH_LONG));
		_methods.insert(std::pair<int, method_t>( TIMER_SWITCH_LONG_OUT_E_ID, &Event_methods::TIMER_SWITCH_LONG_OUT));


		//SERIAL
		_methods.insert(std::pair<int, method_t>( SEND_WANT_E_ID, &Event_methods::SEND_WANT));
		_methods.insert(std::pair<int, method_t>( RDY_TAKING_E_ID, &Event_methods::RDY_TAKING));
		_methods.insert(std::pair<int, method_t>( RDY_TAKING_OK_E_ID, &Event_methods::RDY_TAKING_OK));

		//SLIDE
		_methods.insert(std::pair<int, method_t>( SLIDE_FULL_E_ID, &Event_methods::SLIDE_FULL));
		_methods.insert(std::pair<int, method_t>( SLIDE_NOT_FULL_E_ID, &Event_methods::SLIDE_NOT_FULL));

		//PUK-SWITCH
		_methods.insert(std::pair<int, method_t>( PUK_SWITCH_OPEN_E_ID, &Event_methods::PUK_SWITCH_OPEN));
		_methods.insert(std::pair<int, method_t>( PUK_SWITCH_CLOSE_E_ID, &Event_methods::PUK_SWITCH_CLOSE));


		cout << "Dispatcher constructed on Channel: " << _cid << " Con: " << _con << endl;
	}

	void execute(void*){
		waitPulse();
	}
	virtual void shutdown(){

	}


	virtual ~Dispatcher() {}

	Dispatcher(const Dispatcher &other);
	Dispatcher &operator=(const Dispatcher &other);

public:
	static Dispatcher* getInstance(void){
		static Dispatcher instance_;
		return &instance_;
	}

	virtual void addListener(Event_methods *listener, int event) {
		// Add Listener to be called on a specific Event
		pthread_mutex_lock(&_init_mtx);
		_listeners[event].push_back(listener);
		pthread_mutex_unlock(&_init_mtx);
	}

	virtual void remListeners(Event_methods *listener, int event) {
		pthread_mutex_lock(&_init_mtx);

		// Remove Listener from a specific Event
		for (unsigned i = 0; i < _listeners[event].size(); ++i) {
			if (_listeners[event][i] == listener){
				_listeners[event].erase(_listeners[event].begin() + i);
			}
		}

		pthread_mutex_unlock(&_init_mtx);
	}



	virtual void waitPulse()
	{
		struct _pulse pulse;

		while (1) {
			MsgReceivePulse(_cid, &pulse, sizeof (pulse), NULL);

			map<int,method_t>::iterator it = _methods.find(pulse.value.sival_int);
			if(it != _methods.end())
			{
			   getInstance()->callListeners(pulse.value.sival_int);
			}
			else
			{
				//TODO TIMER
			}
		}
	}


	virtual void callListeners(int event) {
		pthread_mutex_lock(&_init_mtx);

		// Call for every registered Listener
		// the Method that corresponds with event.
		for (unsigned i = 0; i < _listeners[event].size(); ++i) {
			(_listeners[event][i]->*_methods[event])();
		}

		pthread_mutex_unlock(&_init_mtx);
	}
};

#endif
