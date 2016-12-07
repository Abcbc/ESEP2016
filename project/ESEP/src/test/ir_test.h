/*
 * ir_test.h
 *
 *  Created on: 22.11.2016
 *      Author: abl395
 */

#ifndef IR_TEST_H_
#define IR_TEST_H_

class IR_Test {
    public:
		IR_Test(void);
        virtual ~IR_Test();
        void test_ir(void);

    private:
        IR_Test(const IR_Test& l);
        IR_Test& operator=(IR_Test& l);
};


#endif /* IR_TEST_H_ */
