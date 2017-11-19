//
// Created by Denis Chernikov on 12.11.2017.
//

#include <math.h>
#include <malloc.h>
#include "music_theory.h"

const int C_FIRST = 60;
const int OCTAVE_SIZE = 12;
const int MIDI_MIN = 0;
const int MIDI_MAX = 127;
const double ONE_BY_ZERO = 1.0 / 0.0;

double *findClosestNotesUsingDegrees(double num, Tonality *t, int degc, int *degrees) {
    double *res = malloc(sizeof(double) * 2);
    double num_round = round(num);

    res[0] = num_round;
    res[1] = num_round;
    if (Tonality_isNumHasDegreeFromList(t, (int) num, degc, degrees)) {
        return res;
    }

    double num_low = floor(num);
    while (true) {
        if (Tonality_isNumHasDegreeFromList(t, (int) num_low, degc, degrees)) {
            break;
        }
        --num_low;
        if (num_low < MIDI_MIN) {
            num_low = -ONE_BY_ZERO;
            break;
        }
    }
    double num_high = ceil(num);
    while (true) {
        if (Tonality_isNumHasDegreeFromList(t, (int) num_high, degc, degrees)) {
            break;
        }
        ++num_high;
        if (num_high > MIDI_MAX) {
            num_high = ONE_BY_ZERO;
            break;
        }
    }
    res[0] = num_low;
    res[1] = num_high;

    return res;
}
