#ifndef BLINK_RED_H_
#define BLINK_RED_H_

#include <stdint.h>
#include "lib/HAWThread.h"
#include "hal_component.h"

using namespace thread;

class Blink_red: public HAWThread, public Hal_component {
    public:
        Blink_red(uint32_t hz_red);
        virtual ~Blink_red();
        Blink_red(const Blink_red& b);

    private:
        Blink_red& operator=(Blink_red& b);
        virtual void execute(void*);
        virtual void shutdown();

        uint32_t hz_red_ ;
        uint16_t BASE;
        uint8_t RED;
};

#endif
