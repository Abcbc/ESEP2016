#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>


int main(int argc, char *argv[]) {
	std::cout << "Welcome to the QNX Momentics IDE" << std::endl;

	uint16_t ioControlAddress_;
	uint16_t registerAddress_;

	uint8_t ioControlBitmask_;
	uint8_t greenBitmask_;
	uint8_t yellowBitmask_;
	uint8_t redBitmask_;

	ioControlAddress_ = 0x303;
	registerAddress_ = 0x300;

	ioControlBitmask_ = 0b10001010;
	greenBitmask_ = 0b00100000;
	yellowBitmask_ = 0b01000000;
	redBitmask_ = 0b10000000;

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		cout << "Can't get Hardware access, therefore can't do anything."
				<< endl;
	}

	out8(registerAddress_, in8(registerAddress_) & ~0b11111111);
	out8(registerAddress_, in8(registerAddress_) | 0b0000001);



	/* IO Register definieren. */
	for (int i = 0; i<100;i++){


		out8(registerAddress_, in8(registerAddress_) | 0b00010000);

		out8(registerAddress_, in8(registerAddress_) | greenBitmask_);

		usleep(150000);

		out8(registerAddress_, in8(registerAddress_) | yellowBitmask_);

		usleep(150000);

		out8(registerAddress_, in8(registerAddress_) | redBitmask_);

		usleep(150000);

		out8(registerAddress_, in8(registerAddress_) & ~greenBitmask_);



		out8(registerAddress_, in8(registerAddress_) & ~yellowBitmask_);



		out8(registerAddress_, in8(registerAddress_) & ~redBitmask_);



		out8(registerAddress_, in8(registerAddress_) & ~0b00010000);

		usleep(350000);






	}


	out8(ioControlAddress_, ioControlBitmask_);

	out8(registerAddress_, in8(registerAddress_) | 0b0000001);

	usleep(2000000);

	out8(registerAddress_, in8(registerAddress_) | 0b0001000);

	return EXIT_SUCCESS;
}
