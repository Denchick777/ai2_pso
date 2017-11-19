#include <stdio.h>
#include <conio.h>

#include "random.h"
#include "tonality.h"
#include "music_pso_generation.h"
#include "midi_rec.h"

int main(int argc, char **argv) {
    int tonic = getRandomInt(60, 71);
    int mode = getRandomInt(0, 1);  // is major or not
    printf("Generated tonality: tonic = %d, mode = %s\n", tonic, mode ? "Major" : "Minor");

    setTonalityForPSO(Tonality_init(tonic, mode));

    int **accompaniment = generateAccompaniment();
    int *melody = generateMelody();
    writeMIDI("mus.mid", melody, accompaniment);

    int i;
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        free(accompaniment[i]);
    }
    free(accompaniment);
    free(melody);

    printf("\nPress any key to exit...");
    _getch();  // Wait user's input to not exit immediately
    return 0;

//    int *root = malloc(sizeof(int));
//    *root = 60;
//    __MUS_PSO_ROOTS = root;
//    __CUR_CHORD_INDEX = 0;
//    double p[3] = {60, 64, 67};
//    setTonalityForPSO(Tonality_init(60, 1));
//    chordFitnessFunction(p);
//    return 0;
}
