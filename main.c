#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "random.h"
#include "tonality.h"
#include "music_pso_generation.h"
#include "midi_rec.h"

int main(int argc, char **argv) {
    clock_t startTime = clock();

    int tonic = getRandomInt(60, 71);
    int mode = getRandomInt(0, 1);  // is major or not
    setTonalityForPSO(Tonality_init(tonic, mode));

    int **accompaniment = generateAccompaniment(true);
    int *melody = generateMelody(true);

    printf("\nGenerated tonality: tonic = %d, mode = %s\n", tonic, mode ? "Major" : "Minor");

    writeMIDI("mus.mid", melody, accompaniment);

    int i;
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        free(accompaniment[i]);
    }
    free(accompaniment);
    free(melody);

    clock_t endTime = clock();
    printf("\nTime of program execution: %f\n", (double) (endTime - startTime) / CLOCKS_PER_SEC);

    printf("\nPress any key to exit...");
    _getch();  // Wait user's input to not exit immediately
    return 0;

//    int *root = malloc(sizeof(int));
//    *root = 77;
//    __MUS_PSO_ROOTS = root;
//    __CUR_CHORD_INDEX = 0;
//    double p[3] = {63, 67, 70};
//    double p[16] = {64.285335,
//                    64.295563,
//                    59.077286,
//                    64.264329,
//                    64.442367,
//                    57.227228,
//                    64.272571,
//                    76.086004,
//                    76.243112,
//                    76.344085,
//                    76.252339,
//                    76.288817,
//                    69.100494,
//                    71.277332,
//                    64.000000,
//                    52.500000};
//    setTonalityForPSO(Tonality_init(68, 1));
//    chordFitnessFunction(p);
//    accompanimentFitnessFunction(p);
//    return 0;
}
