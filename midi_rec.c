//
// Created by Denis Chernikov on 13.11.2017.
//

#include <stdio.h>
#include "music_pso_generation.h"
#include "midi_rec.h"

void writeMIDI(char *filename, int *melody, int **accompaniment) {
    int i, j;

    printf("Melody: [");
    for (i = 0; i < MEL_DIMENSIONS; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", melody[i]);
    }
    printf("]\nAccompaniment: [");
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("[");
        for (j = 0; j < ACC_CHORD_SIZE; ++j) {
            if (j != 0) {
                printf(", ");
            }
            printf("%d", accompaniment[i][j]);
        }
        printf("]");
    }
    printf("]\n");

    // TODO MIDI
}
