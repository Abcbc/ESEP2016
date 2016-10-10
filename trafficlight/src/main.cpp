/* 
 * @file    main.cpp
 * @author  Lukas Drews, Julian Magierski, Daniel Nisch, Melvyn Linke
 * @version 0.0
 * @desc    temporary startup class
 */

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include "Trafficlight.h"
#include "lib/HWaccess.h"

using namespace std;

int main(int argc, char *argv[]) {

    cout << "test" << endl;

    Trafficlight tlC(5);
    tlC.setLights(true, true, true);
    sleep(2);
    tlC.setLights(false, false, false);
    sleep(2);
    tlC.setLights(false, false, true);
    sleep(2);

    return EXIT_SUCCESS;
}
