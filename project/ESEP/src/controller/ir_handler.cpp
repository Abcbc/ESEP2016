#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <iomanip>
#include <pthread.h>
#include <iostream>
#include "lib/Lock.h"
#include "lib/HAWThread.h"
#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include "lib/HWaccess.h"
#include "src/controller/ir_handler.h"
#include "src/controller/event_table.h"
#include "src/lib/hal/hal_component.h"
#include <sys/time.h>

using namespace std;

struct sigevent isrEvent;

#define FILTER_MASK 0b1111000011011011
#define ON_BIT 0h10000

#define ENRTY 1
#define HEIGHT_MEASUREMENT 2
#define PUK_SWITCH 8
#define EXIT 128
#define START 4096
#define STOP 8192
#define RESET 16384
#define E_STOP 32768
#define RAMP 64

uint32_t source_init;
uint32_t old_changes = 0;
uint32_t old_changes2 = 0;
uint64_t change_time = 0;
uint64_t change_time2 = 0;
const uint64_t DEBOUNCE_TIME = 140;

Ir_handler* Ir_handler::instance_ = NULL;
pthread_mutex_t Ir_handler::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Ir_handler::Ir_handler() {
	c_id = 0;
}

const struct sigevent* ISR_DIO(void* arg, int id) {
	uint32_t source = ((in8(PORT_C) << 8) + (in8(PORT_B))) & FILTER_MASK;
	uint32_t changes;
	uint32_t e_id = -1;
	static uint32_t source_old = 0b1010000000000000;
	changes = source ^ source_old;
	source_old = source;

	bool debounce = true;
	uint64_t debounce_change_time = 0;
	if(old_changes == changes){
		debounce_change_time = change_time;
	}else if(old_changes2 == changes){
		debounce_change_time = change_time2;
	}
	if(debounce_change_time != 0){
		struct timeval tp;
		gettimeofday(&tp, NULL);
		uint64_t current_time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		if(current_time < (change_time + DEBOUNCE_TIME)){
			debounce = false;
		}
	}
	if (changes > 0 && debounce) {
		switch (changes) {
			case ENRTY:
				if ((source & ENRTY) > 0) {
					e_id = LIGHT_BARRIER_ENTRY_OPEN_E_ID;
				} else {
					e_id = LIGHT_BARRIER_ENTRY_CLOSE_E_ID;
				}
				break;

			case HEIGHT_MEASUREMENT:
				if ((source & ENRTY) > 0) {
					e_id = LIGHT_BARRIER_HEIGHT_OPEN_E_ID;
				} else {
					e_id = LIGHT_BARRIER_HEIGHT_CLOSE_E_ID;
				}
				break;

			case PUK_SWITCH:
				if ((source & PUK_SWITCH) > 0) {
					e_id = LIGHT_BARRIER_SWITCH_OPEN_E_ID;
				} else {
					e_id = LIGHT_BARRIER_SWITCH_CLOSE_E_ID;
				}
				break;

			case EXIT:
				if ((source & EXIT) > 0) {
					e_id = LIGHT_BARRIER_EXIT_OPEN_E_ID;
				} else {
					e_id = LIGHT_BARRIER_EXIT_CLOSE_E_ID;
				}

				break;
			case RAMP:
				if ((source & RAMP) > 0) {
					e_id = LIGHT_BARRIER_RAMP_OPEN_E_ID;
				} else {
					e_id = LIGHT_BARRIER_RAMP_CLOSE_E_ID;
				}

				break;
			case START: //TODO FILTER MULTIPLE ON/OFF
				if ((source & START) > 0) {
					e_id = BUTTON_START_E_ID;
				} else {
//					e_id = BUTTON_START_RELEASED_E_ID;
				}

				break;
			case STOP:
				if ((source & STOP) > 0) {
					e_id = BUTTON_STOP_E_ID;
				} else {
//					e_id = BUTTON_STOP_PRESSED_E_ID;
				}

				break;
			case RESET:
				if ((source & RESET) > 0) {
					e_id = BUTTON_RESET_E_ID;
				} else {
//					e_id = BUTTON_RESET_RELEASED_E_ID;
				}

				break;
			case E_STOP:
				if ((source & E_STOP) > 0) {
					e_id = ESTOP_RELEASED_THIS_E_ID;
				} else {
					e_id = ESTOP_THIS_E_ID;
				}

				break;
		}

		struct timeval tp;
		gettimeofday(&tp, NULL);
		change_time2 = change_time;
		change_time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		old_changes2 = old_changes;
		old_changes = changes;
	}
	struct sigevent* event = (struct sigevent*) arg;
	out8(0x30F, 0);

	event->__sigev_un2.__st.__sigev_code = 0;
	event->sigev_value.sival_int = e_id;
	return event;
}

int Ir_handler::connect(int c_id) {

	out8(0x30F, 0);
	out8(0x30B, 0b11111001);

	source_init = ((in8(PORT_C) << 8) + (in8(PORT_B))) & FILTER_MASK;
	SIGEV_PULSE_INIT(&isrEvent, c_id, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
	int isrId = InterruptAttach(11, ISR_DIO, &isrEvent, sizeof(isrEvent), 0);
	if (isrId == -1) {
		return -1; // TODO:Real Error code
	}
	return 0;
}

Ir_handler* Ir_handler::get_instance(void) {
	if (instance_ == NULL) {
		pthread_mutex_lock(&init_mtx);
		if (instance_ == NULL) {
			instance_ = new Ir_handler();
		}
		pthread_mutex_unlock(&init_mtx);
	}
	return instance_;
}
