#ifndef BLINK_GREEN_H_
#define BLINK_GREEN_H_

#include <stdint.h>
#include "lib/HAWThread.h"
#include "hal_component.h"

using namespace thread;

class Blink_green : public HAWThread, public Hal_component {
    public:
        Blink_green(uint32_t hz_green);
        virtual ~Blink_green();
        Blink_green(const Blink_green& b);

    private:
        Blink_green& operator=(Blink_green& b);
        virtual void execute(void*);
        virtual void shutdown();

        uint32_t hz_green_ ;
        uint16_t BASE;
        uint8_t GREEN;
};

#endif
