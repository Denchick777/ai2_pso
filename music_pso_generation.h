//
// Created by Denis Chernikov on 13.11.2017.
//

#ifndef AI2_MUSIC_PSO_GENERATION_H
#define AI2_MUSIC_PSO_GENERATION_H

#include "tonality.h"

const int ACC_DIMENSIONS, ACC_CHORD_SIZE, MEL_DIMENSIONS;

const int ALLOWED_MIN, ALLOWED_MAX, ROOT_MAX;

/**
 * Set tonality for all PSO music functions.
 *
 * @param t Tonality
 */
void setTonalityForPSO(Tonality t);

/**
 * Get an accompaniment's set of chords that fits predefined conditions.
 * NOTE: You're advised to use 'free()' function FOR EACH ARRAY LINE AND ARRAY ITSELF
 * after returned accompaniment will be used.
 *
 * @return Array of chords
 */
int **generateAccompaniment();

/**
 * Create particle of accompaniment for PSO algorithm.
 * NOTE: You're advised to use 'free()' function after returned particle will be used.
 *
 * @return Particle values
 */
double *accompanimentParticleCreator();

/**
 * Count fitness for particle of accompaniment for PSO algorithm.
 * NOTE: __MUS_PSO_TONALITY should be defined.
 *
 * @param part Particle to check
 * @return Particle's fitness
 */
double accompanimentFitnessFunction(double *part);

/**
 * Get an accompaniment's chord that fits predefined conditions.
 * NOTE: You're advised to use 'free()' function after returned accompaniment will be used.
 * NOTE: __MUS_PSO_ROOTS should be defined, CUR_CHORD_INDEX should be in range [0, ACC_DIMENSIONS).
 *
 * @return Array of chords
 */
int *generateChord();

/**
 * Create particle of chord for PSO algorithm.
 * NOTE: You're advised to use 'free()' function after returned particle will be used.
 *
 * @return Particle values
 */
double *chordParticleCreator();

/**
 * Count fitness for particle of chord for PSO algorithm.
 * NOTE: __MUS_PSO_TONALITY should be defined.
 *
 * @param part Particle to check
 * @return Particle's fitness
 */
double chordFitnessFunction(double *part);

/**
 * Get a melody's set of notes that fits predefined conditions and generated accompaniment.
 * NOTE: You're advised to use 'free()' function after returned melody will be used.
 *
 * @return Array of notes
 */
int *generateMelody();

/**
 * Create particle of melody for PSO algorithm.
 * NOTE: You're advised to use 'free()' function after returned particle will be used.
 *
 * @return Particle values
 */
double *melodyParticleCreator();

/**
 * Count fitness for particle of melody for PSO algorithm.
 * NOTE: __MUS_PSO_TONALITY and __MUS_PSO_ACCOMPANIMENT should be defined.
 *
 * @return Particle's fitness
 */
double melodyFitnessFunction(double *part);

#endif //AI2_MUSIC_PSO_GENERATION_H
