//
// Created by Denis Chernikov on 12.11.2017.
//

#include <time.h>
#include <stdlib.h>
#include "random.h"

_Bool __myRandom_seedSet = false;

const double RAND_MAX_F = (double) RAND_MAX;

void __myRandom_setSeed() {
    if (!__myRandom_seedSet) {
        srand(time(NULL));
        __myRandom_seedSet = true;
    }
}

int getRandomInt(const int min, const int max) {
    __myRandom_setSeed();
    return rand() % (max - min + 1) + min;
}

double getRandomDouble() {
    __myRandom_setSeed();
    return rand() / RAND_MAX_F;
}

double getRandomDoubleBetween(double min, double max) {
    return getRandomDouble() * (max - min) + min;
}
