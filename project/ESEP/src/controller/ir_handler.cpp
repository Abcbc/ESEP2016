#include <sys/neutrino.h>
#include <sys/siginfo.h>

sigevent* isr_dio(void* arg,int id){
	struct sigevent* event = (struct sigevent*) arg;
	out8(0x30F, 0);

	switch(){

	case LIGHT_BARRIER_EXIT:

		break;
	case LIGHT_BARRIER_SWITCH:

		break;
	case LIGHT_BARRIER_ENTRY:

		break;
	}
	event->__sigev_un2.__st.__sigev_code = 0;
	event->sigev_value.sival_int = 1;
	return event;
}
int Ir_handler::connect(c_id){
	 out8(0x30F, 0);
	 out8(0x30B, 0b11111101);

	 SIGEV_PULSE_INIT(&isrEvent, c_id, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
	 isrId = InterruptAttach(IRQ, isr_dio, &isrEvent, sizeof(isrEvent), 0);
	 if (isrId == -1) {
	   return -1; // TODO:Real Error code
	 }
	 return 0;
}
