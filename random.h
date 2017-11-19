//
// Created by Denis Chernikov on 12.11.2017.
//

#ifndef AI2_RANDOM_H
#define AI2_RANDOM_H

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Indicates if the random seed has been set already.
 */
_Bool __myRandom_seedSet;

/**
 * Get random int in range [min, max].
 *
 * @param min Minimum number
 * @param max Maximum number
 * @return Random integer between given numbers
 */
int getRandomInt(int min, int max);

/**
 * Get random double in range [0, 1].
 *
 * @return Random double between 0 and 1
 */
double getRandomDouble();

/**
 * Get random double in range [min, max].
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return Random double between given numbers
 */
double getRandomDoubleBetween(double min, double max);

#endif //AI2_RANDOM_H
