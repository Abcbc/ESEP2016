/*
 * Puk_control.h
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#ifndef PUK_CONTROL_H_
#define PUK_CONTROL_H_

class Puk_control {
public:
	Puk_control();
	virtual ~Puk_control();

	bool sequenz_group();

	int systemType;
};

#endif /* PUK_CONTROL_H_ */
