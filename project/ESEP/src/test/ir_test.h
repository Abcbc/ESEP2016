/*
 * ir_test.h
 *
 *  Created on: 22.11.2016
 *      Author: abl395
 */

#ifndef IR_TEST_H_
#define IR_TEST_H_

class ir_test {
    public:
        ir_test(void);
        virtual ~ir_test();
        void test_ir(void);

    private:
        ir_test(const ir_test& l);
        ir_test& operator=(ir_test& l);
};


#endif /* IR_TEST_H_ */
