//
// Created by Denis Chernikov on 13.11.2017.
//

#ifndef AI2_PSO_MIDI_REC_H
#define AI2_PSO_MIDI_REC_H

/**
 * Generate MIDI file that will have given filename, melody (32 eight notes) and accompaniment (16 quarter triples).
 *
 * @param melody Melody's notes set
 * @param accompaniment Accompaniment's chords (note triples') set
 */
void writeMIDI(char *filename, int *melody, int **accompaniment);

#endif //AI2_PSO_MIDI_REC_H
