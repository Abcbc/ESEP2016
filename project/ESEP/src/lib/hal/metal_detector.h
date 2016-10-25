/*
 * metal_detector.h
 *
 *  Created on: 25.10.2016
 *      Author: abr468
 */

#ifndef METAL_DETECTOR_H_
#define METAL_DETECTOR_H_

class Metal_detector:Hal_component{
	public:
			bool is_metal(void);

	private:
			uint8_t BASE;
};

#endif /* METAL_DETECTOR_H_ */
