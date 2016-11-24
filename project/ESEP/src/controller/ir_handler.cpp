#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <iomanip>
#include <pthread.h>
#include "lib/Lock.h"
#include "lib/HAWThread.h"
#include <iostream>
#include <cstdlib>
#include "lib/HWaccess.h"
#include "src/controller/ir_handler.h"
#include "src/controller/event_table.h"

using namespace std;
struct sigevent isrEvent;

Ir_handler* Ir_handler::instance_ = NULL;
pthread_mutex_t  Ir_handler::init_mtx = PTHREAD_MUTEX_INITIALIZER;

Ir_handler::Ir_handler() {
	c_id = 0;
}


const struct sigevent* ISR_DIO(void* arg, int id) {
    struct sigevent* event = (struct sigevent*) arg;
    out8(0x30F, 0);

    event->__sigev_un2.__st.__sigev_code = 0;
    event->sigev_value.sival_int = 1;
    return event;
}

int Ir_handler::connect(int c_id){

	 out8(0x30F, 0);
	 out8(0x30B, 0b11111101);

	 SIGEV_PULSE_INIT(&isrEvent, c_id, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
	 int isrId = InterruptAttach(11, ISR_DIO, &isrEvent, sizeof(isrEvent), 0);
	 if (isrId == -1) {
	   return -1; // TODO:Real Error code
	 }
	 return 0;
}

Ir_handler* Ir_handler::get_instance(void){
	if(instance_ == NULL){
		pthread_mutex_lock(&init_mtx);
		if(instance_ == NULL){
			instance_ = new Ir_handler();
		}
		pthread_mutex_unlock(&init_mtx);
	}
	return instance_;
}
