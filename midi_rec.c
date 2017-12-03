//
// Created by Denis Chernikov on 13.11.2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "music_pso_generation.h"
#include "midi_rec.h"

void writeMIDI(char *filename, int *melody, int **accompaniment) {
    int i, j;

    FILE *fd = fopen(filename, "w+");

    printf("\nMelody: [");
    fprintf(fd, "[");
    for (i = 0; i < MEL_DIMENSIONS; ++i) {
        if (i != 0) {
            printf(", ");
            fprintf(fd, ", ");
        }
        printf("%d", melody[i]);
        fprintf(fd, "%d", melody[i]);
    }
    printf("]\n\nAccompaniment: [");
    fprintf(fd, "]\n[");
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        if (i != 0) {
            printf(", ");
            fprintf(fd, ", ");
        }
        printf("[");
        fprintf(fd, "[");
        for (j = 0; j < ACC_CHORD_SIZE; ++j) {
            if (j != 0) {
                printf(", ");
                fprintf(fd, ", ");
            }
            printf("%d", accompaniment[i][j]);
            fprintf(fd, "%d", accompaniment[i][j]);
        }
        printf("]");
        fprintf(fd, "]");
    }
    printf("]\n");
    fprintf(fd, "]");

    fclose(fd);

    system("python midi_recording.py");
}
