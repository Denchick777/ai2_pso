//
// Created by Denis Chernikov on 12.11.2017.
//

#ifndef AI2_TONALITY_H
#define AI2_TONALITY_H

#include <stdbool.h>

/**
 * Represents musical tonality using it's tonic (MIDI NOTE in range [60, 71]) and mode (major or minor).
 */
typedef struct Tonality {
    /**
     * Tonality tonic note (between 60 and 72).
     */
    int tonic;

    /**
     * Tonality mode (1 - major, 0 - minor).
     */
    _Bool mode;
} Tonality;

/**
 * Initialise tonality object.
 *
 * @param tonic Tonality tonic [60, 71]
 * @param isMajor Tonality mode
 * @return Tonality initialised object
 */
Tonality Tonality_init(int tonic, int isMajor);

/**
 * Is given tonality - major?
 *
 * @param t Tonality
 * @return 'true' if major, 'false' otherwise
 */
_Bool Tonality_isMajor(Tonality *t);

/**
 * Is given tonality - minor?
 *
 * @param t Tonality
 * @return 'true' if minor, 'false' otherwise
 */
_Bool Tonality_isMinor(Tonality *t);

/**
 * Is given note - tonic of the given tonality and belongs to the MIDI range?
 *
 * @param t Tonality
 * @param note Given note
 * @return 'true' if tonic, 'false' otherwise
 */
_Bool Tonality_isNoteTonic(Tonality *t, int note);

/**
 * Get degree number of the given note in the specified tonality.
 *
 * @param t Tonality
 * @param note Note to get the degree of
 * @return int [1, 7] - degree; if not belongs to the tonality or MIDI range - return -1
 */
int Tonality_getDegreeOfNote(Tonality *t, int note);

/**
 * Get note of the first octave that represents given degree in the specified tonality.
 *
 * @param t Tonality
 * @param deg Degree to get [1, 7]
 * @return Note from the first octave, if degree is wrong - return -1
 */
int Tonality_getNoteByDegree(Tonality *t, int deg);

/**
 * Is given node belongs to the set of tonality notes of the provided tonality?
 *
 * @param t Tonality
 * @param note Node to check belonging for
 * @return 'true' if belongs, 'false' otherwise
 */
_Bool Tonality_hasNote(Tonality *t, int note);

/**
 * Is given int - one of the provided in a list degrees of the given tonality?
 *
 * @param t Tonality
 * @param num Number to check
 * @param degc Size of degree numbers' array
 * @param degrees Array of degrees to search throw
 * @return 'true' if num is one of degrees, 'false' otherwise
 */
_Bool Tonality_isNumHasDegreeFromList(Tonality *t, int num, int degc, int *degrees);

#endif //AI2_TONALITY_H
