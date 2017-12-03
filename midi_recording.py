import json
from midiutil import MIDIFile
import os

IN_FILENAME = 'mus.tmp'
FILENAME = "mus.mid"

with open(IN_FILENAME) as inpf:
    melody, accompaniment = inpf.readlines()

melody = json.loads(melody)
accompaniment = json.loads(accompaniment)

file = MIDIFile(2, adjust_origin=True)
file.addTempo(0, 0, 120)
file.addTempo(0, 1, 120)

time = 0
for n in melody:
    file.addNote(0, 1, n, time, 1, 127)
    time += 1

time = 0
for c in accompaniment:
    for n in c:
        file.addNote(1, 1, n, time, 2, 100)
    time += 2

file.addNote(0, 1, 0, time + 1, 1, 0)  # End of file

with open(FILENAME, "wb") as output_file:
    file.writeFile(output_file)

os.remove(IN_FILENAME)

os.system("start \"" + __name__ + "\" \"" + FILENAME + "\"")
