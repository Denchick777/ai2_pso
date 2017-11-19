//
// Created by Denis Chernikov on 12.11.2017.
//

#ifndef AI2_MUSIC_THEORY_H
#define AI2_MUSIC_THEORY_H

#include "tonality.h"

const int C_FIRST, OCTAVE_SIZE, MIDI_MIN, MIDI_MAX;
const double ONE_BY_ZERO;

/**
 * Get pair of closest degrees (by specified numbers) from the given tonality to the 'num'.
 * NOTE: You're advised to use 'free()' function after function's result will be used.
 *
 * @param num Number to search around
 * @param t Tonality
 * @param degc Amount of degrees in array
 * @param degrees List of allowed degrees for nearest searching
 * @return Array containing a pair of numbers - lower and upper closest; if num is degree note itself: [0] == [1]
 */
double *findClosestNotesUsingDegrees(double num, Tonality *t, int degc, int *degrees);

#endif //AI2_MUSIC_THEORY_H
