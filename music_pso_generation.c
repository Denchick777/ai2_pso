//
// Created by Denis Chernikov on 13.11.2017.
//

#include <stddef.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include "pso.h"
#include "random.h"
#include "music_theory.h"
#include "music_pso_generation.h"

const int ACC_DIMENSIONS = 16;
const int ACC_CHORD_SIZE = 3;
const int MEL_DIMENSIONS = 32;

const int ALLOWED_MIN = 48;
const int ALLOWED_MAX = 96;
const int ROOT_MAX = 84;

Tonality __MUS_PSO_TONALITY;
int *__MUS_PSO_ROOTS = NULL;
int **__MUS_PSO_ACCOMPANIMENT = NULL;
int __CUR_CHORD_INDEX = -1;

void setTonalityForPSO(Tonality t) {
    __MUS_PSO_TONALITY = t;
}

/* -------------------- Accompaniment PSO generation functions -------------------- */

int **generateAccompaniment() {
    if (__MUS_PSO_TONALITY.tonic < 60 || __MUS_PSO_TONALITY.tonic > 71) {
        return NULL;
    }

    int i;

    // PSO configuration generation
    ConfigPSO conf = ConfigPSO_init();
    conf.particleCreator = &accompanimentParticleCreator;
    conf.fitnessFunction = &accompanimentFitnessFunction;
    conf.dimensions = ACC_DIMENSIONS;
    conf.swarmSize = 300;
    conf.iterThreshold = 5000;
    conf.funcMin = MIDI_MIN;
    conf.funcMax = MIDI_MAX;
    conf.c0 = 1.0;
    conf.c1 = 35.3;
    conf.c2 = 5.15;
    conf.inertInit = 1.0;
    conf.inertThreshold = 0.0;
    conf.inertStep = 0.999;

    double *res = executePSO(&conf);

    __MUS_PSO_ROOTS = (int *) malloc(sizeof(int) * ACC_DIMENSIONS);
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        __MUS_PSO_ROOTS[i] = (int) round(res[i]);
        printf("%d %s\n",
               __MUS_PSO_ROOTS[i],
               (__MUS_PSO_ROOTS[i] - (__MUS_PSO_TONALITY.tonic % 12)) % 12 == 0 ||
               (__MUS_PSO_ROOTS[i] - (__MUS_PSO_TONALITY.tonic % 12)) % 12 == 5 ||
               (__MUS_PSO_ROOTS[i] - (__MUS_PSO_TONALITY.tonic % 12)) % 12 == 7 ? "true" : "false");  // TODO remove
    }
    free(res);

    __MUS_PSO_ACCOMPANIMENT = (int **) malloc(sizeof(int *) * ACC_DIMENSIONS);
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        __CUR_CHORD_INDEX = i;
        __MUS_PSO_ACCOMPANIMENT[i] = generateChord();
    }
    free(__MUS_PSO_ROOTS);

    return __MUS_PSO_ACCOMPANIMENT;
}

double *accompanimentParticleCreator() {
    double *res = malloc(sizeof(double) * ACC_DIMENSIONS);
    int i;
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        res[i] = getRandomDoubleBetween(MIDI_MIN, MIDI_MAX);  // TODO
//        res[i] = getRandomDoubleBetween(ALLOWED_MIN, ALLOWED_MAX);
    }
    return res;
}

double accompanimentFitnessFunction(double *part) {
    double fit = 0.0, curRoot, weight, mid, min = MIDI_MAX, max = MIDI_MIN;
    int maxDifference = 25;
    int i, counter, rootRound;
    double *closest;
    int degrees[3] = {1, 4, 5};  // tonic, subdominant, dominant
    int tonicDegree[1] = {1};  // only tonic for degree search

    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        curRoot = part[i];
        rootRound = (int) round(curRoot);

        // 1st condition - range belonging
        weight = 0.0;
        if (curRoot <= ALLOWED_MIN && curRoot > MIDI_MIN) {
            fit += weight * (ALLOWED_MIN - curRoot) / (ALLOWED_MIN - MIDI_MIN);
        } else if (curRoot >= ROOT_MAX && curRoot < MIDI_MAX) {
            fit += weight * (curRoot - ROOT_MAX) / (MIDI_MAX - ROOT_MAX);
        } else if (curRoot <= MIDI_MIN || curRoot >= MIDI_MAX) {
            fit += weight;
        }

        // 2nd condition - no more than 12 between neighbours
        if (i > 0 && fabs(rootRound - part[i - 1]) > 12) {
            weight = 0.0;
            if (curRoot < part[i - 1] && curRoot > MIDI_MIN && part[i - 1] != 12 - MIDI_MIN) {
                fit += weight * (part[i - 1] - 12 - curRoot) / (part[i - 1] - 12 - MIDI_MIN);
            } else if (curRoot > part[i - 1] && curRoot < MIDI_MAX && part[i - 1] != 12 + MIDI_MAX) {
                fit += weight * (curRoot - part[i - 1] + 12) / (MIDI_MAX - part[i - 1] + 12);
            } else {
                fit += weight;
            }
        }

        // 3rd condition - no more than 4 same in a row
        if (i > 3) {
            weight = 0.0;
            counter = 1;
            while (part[i - counter] == rootRound && i - counter >= 0) {
                ++counter;
                if (counter > 4) {
                    fit += weight / (i - 3);
                }
            }
        }

        // 4th condition - is [tonic, subdominant, dominant]
        closest = findClosestNotesUsingDegrees(curRoot, &__MUS_PSO_TONALITY, 3, degrees);
        if (closest[0] != closest[1]) {
            weight = 10.0;
            if (closest[0] != -ONE_BY_ZERO && closest[1] != ONE_BY_ZERO) {
                mid = (closest[0] + closest[1]) / 2;
                fit += weight * (curRoot > mid ? (closest[1] - curRoot) / (closest[1] - mid) :
                                 (curRoot - closest[0]) / (mid - closest[0]));
            } else {
                fit += weight;
            }
        }
        free(closest);

        // 5th condition - difference between maximum and minimum
        if (curRoot < min) {
            min = curRoot;
        }
        if (curRoot > max) {
            max = curRoot;
        }

        // 6th condition - tonics
        weight = 0.0;
        if (i == 0 || i % 4 == 3) {
            closest = findClosestNotesUsingDegrees(curRoot, &__MUS_PSO_TONALITY, 1, tonicDegree);
            if (closest[0] != closest[1]) {
                if (closest[0] != -ONE_BY_ZERO && closest[1] != ONE_BY_ZERO) {
                    mid = (closest[0] + closest[1]) / 2;
                    if (i == 0) {
                        fit += weight * 0.9 *
                               (curRoot > mid ? (closest[1] - curRoot) / (closest[1] - mid) :
                                (curRoot - closest[0]) / (mid - closest[0]));
                    } else if (i % 4 == 3) {
                        if (i == ACC_DIMENSIONS - 1) {
                            fit += weight *
                                   (curRoot > mid ? (closest[1] - curRoot) / (closest[1] - mid) :
                                    (curRoot - closest[0]) / (mid - closest[0]));
                        } else {
                            fit += weight * 0.5 *
                                   (curRoot > mid ? (closest[1] - curRoot) / (closest[1] - mid) :
                                    (curRoot - closest[0]) / (mid - closest[0]));
                        }
                    }
                } else {
                    fit += weight;
                }
            }
            free(closest);
        }
    }
    // 5th condition - continuation
    weight = 0.0;
    if (max - min > maxDifference) {
        fit += weight * (1 - 1 / (max - min - maxDifference + 1));
    }

    return fit;
}

/* -------------------- Chord PSO generation functions -------------------- */

int *generateChord() {
    if (__CUR_CHORD_INDEX < 0 || __CUR_CHORD_INDEX > ACC_DIMENSIONS) {
        return NULL;
    }

    int i;

    // PSO configuration generation
    ConfigPSO conf = ConfigPSO_init();
    conf.particleCreator = &chordParticleCreator;
    conf.fitnessFunction = &chordFitnessFunction;
    conf.dimensions = ACC_CHORD_SIZE;
    conf.swarmSize = 300;
    conf.iterThreshold = 200;
    conf.funcMin = MIDI_MIN;
    conf.funcMax = MIDI_MAX;
    conf.c0 = 1.0;
    conf.c1 = 3.0;
    conf.c2 = 5.0;
    conf.inertInit = 1.0;
    conf.inertThreshold = 0.0;
    conf.inertStep = 0.999;

    double *chords = executePSO(&conf);

    int *res = (int *) malloc(sizeof(int) * ACC_CHORD_SIZE);
    for (i = 0; i < ACC_CHORD_SIZE; ++i) {
        res[i] = (int) round(chords[i]);
    }
    free(chords);

    return res;
}

double *chordParticleCreator() {
    double *res = malloc(sizeof(double) * ACC_CHORD_SIZE);
    int i;
    for (i = 0; i < ACC_CHORD_SIZE; ++i) {
        res[i] = getRandomDoubleBetween(MIDI_MIN, MIDI_MAX);
    }
    return res;
}

double chordFitnessFunction(double *part) {
    double fit = 0.0, weight = 1.0, curNote;
    int i, degree, target;
    for (i = 0; i < ACC_CHORD_SIZE; ++i) {
        curNote = part[i];
        if (curNote < MIDI_MIN || curNote > MIDI_MAX) {
            fit += weight;
        } else {
            degree = Tonality_getDegreeOfNote(&__MUS_PSO_TONALITY, __MUS_PSO_ROOTS[__CUR_CHORD_INDEX]) + 2 * i;
            target = Tonality_getNoteByDegree(&__MUS_PSO_TONALITY, degree) -
                     C_FIRST + __MUS_PSO_ROOTS[__CUR_CHORD_INDEX];
            fit += weight * (ACC_CHORD_SIZE - i) * (target - curNote) /
                   (target - (curNote > target ? MIDI_MAX : MIDI_MIN));
        }
    }
    return fit;
}

/* -------------------- Melody PSO generation functions -------------------- */

int *generateMelody() {
    if (__MUS_PSO_ACCOMPANIMENT == NULL || __MUS_PSO_TONALITY.tonic < 60 || __MUS_PSO_TONALITY.tonic > 71) {
        return NULL;
    }

    int i;

    // PSO configuration generation
    ConfigPSO conf = ConfigPSO_init();
    conf.particleCreator = &melodyParticleCreator;
    conf.fitnessFunction = &melodyFitnessFunction;
    conf.dimensions = MEL_DIMENSIONS;
    conf.swarmSize = 200;
    conf.iterThreshold = 200;
    conf.funcMin = MIDI_MIN;
    conf.funcMax = MIDI_MAX;
    conf.c0 = 1.0;
    conf.c1 = 3.0;
    conf.c2 = 5.0;
    conf.inertInit = 1.0;
    conf.inertThreshold = 0.0;
    conf.inertStep = 0.999;

    double *melody = executePSO(&conf);

    int *res = (int *) malloc(sizeof(int) * MEL_DIMENSIONS);
    for (i = 0; i < MEL_DIMENSIONS; ++i) {
        res[i] = (int) round(melody[i]);
    }

    return res;
}

double *melodyParticleCreator() {
    double *res = malloc(sizeof(double) * MEL_DIMENSIONS);
    int i;
    for (i = 0; i < MEL_DIMENSIONS; ++i) {
        res[i] = getRandomDoubleBetween(MIDI_MIN, MIDI_MAX);
    }
    return res;
}

double melodyFitnessFunction(double *part) {
    double fit = 0.0, curNote;
    int i;
    for (i = 0; i < MEL_DIMENSIONS; ++i) {
        curNote = part[i];
        // TODO
    }
    return fit;
}
