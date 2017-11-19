//
// Created by Denis Chernikov on 12.11.2017.
//

#include "tonality.h"
#include "music_theory.h"

Tonality Tonality_init(int tonic, int isMajor) {
    Tonality res = {C_FIRST + (tonic % OCTAVE_SIZE), isMajor ? true : false};
    return res;
}

_Bool Tonality_isMajor(Tonality *t) {
    return t->mode;
}

_Bool Tonality_isMinor(Tonality *t) {
    return !Tonality_isMajor(t);
}

_Bool Tonality_isNoteTonic(Tonality *t, int note) {
    return Tonality_getDegreeOfNote(t, note) == 1 ? true : false;
}

int Tonality_getDegreeOfNote(Tonality *t, int note) {
    if (note < MIDI_MIN || note > MIDI_MAX) {
        return 0;
    }
    int n = C_FIRST % OCTAVE_SIZE + note % OCTAVE_SIZE - t->tonic % OCTAVE_SIZE;
    if (n < 0) {
        n += OCTAVE_SIZE;
    }
    switch (n) {
        case 0:
            return 1;
        case 2:
            return 2;
        case 3:
            return t->mode ? -1 : 3;
        case 4:
            return t->mode ? 3 : -1;
        case 5:
            return 4;
        case 7:
            return 5;
        case 8:
            return t->mode ? -1 : 6;
        case 9:
            return t->mode ? 6 : -1;
        case 10:
            return t->mode ? -1 : 7;
        case 11:
            return t->mode ? 7 : -1;
        default:
            return -1;
    }
}

int Tonality_getNoteByDegree(Tonality *t, int deg) {
    if (deg < 1 || deg > 7) {
        return -1;
    }
    switch (deg) {
        case 1:
            return t->tonic;
        case 2:
            return t->tonic + 2;
        case 3:
            return t->tonic + (t->mode ? 4 : 3);
        case 4:
            return t->tonic + 5;
        case 5:
            return t->tonic + 7;
        case 6:
            return t->tonic + (t->mode ? 9 : 8);
        case 7:
            return t->tonic + (t->mode ? 11 : 10);
        default:
            return -1;
    }
}

_Bool Tonality_hasNote(Tonality *t, int note) {
    return Tonality_getDegreeOfNote(t, note) != -1 ? true : false;
}

_Bool Tonality_isNumHasDegreeFromList(Tonality *t, int num, int degc, int *degrees) {
    int deg_num = Tonality_getDegreeOfNote(t, num);
    int i;
    for (i = 0; i < degc; ++i) {
        if (deg_num == degrees[i]) {
            return true;
        }
    }
    return false;
}
