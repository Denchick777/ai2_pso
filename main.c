#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "random.h"
#include "tonality.h"
#include "music_pso_generation.h"
#include "midi_rec.h"

int main(int argc, char **argv) {
    clock_t startTime = clock();  // Get the time of execution start

    int tonic = getRandomInt(60, 71);
    int mode = getRandomInt(0, 1);  // is major (1) or not (0)
    setTonalityForPSO(Tonality_init(tonic, mode));

    int **accompaniment = generateAccompaniment(true);
    int *melody = generateMelody(true);

    printf("\nGenerated tonality: tonic = %d, mode = %s\n", tonic, mode ? "Major" : "Minor");

    writeMIDI("mus.tmp", melody, accompaniment);

    int i;
    for (i = 0; i < ACC_DIMENSIONS; ++i) {
        free(accompaniment[i]);
    }
    free(accompaniment);
    free(melody);

    clock_t endTime = clock();  // Count the length of program execution time
    printf("\nTime of program execution: %f sec.\n", (double) (endTime - startTime) / CLOCKS_PER_SEC);

    printf("\nPress any key to exit...");
    _getch();  // Wait user's input to not exit immediately
    return 0;
}
