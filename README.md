# Audio Programming Coursework 2
Completed as part of the final year *Audio Programming* module on the [Tonmeister](https://www.tonmeister.uk/) course at the University of Surrey.

## Command-line finite impulse reponse filter.

This program will apply a low-pass filter to audio from an input wav file and write the result to a specified output wav file.

The required argument format is:
<input file path> <output file path> <cut-off frequency>

The input and output paths should NOT contain whitespace. The frequency should be an integer in Hz. The frequency range supported is from 1Hz up to just less than half the sample frequency of the selected input file.

Optional arguments can be included *before* the required arguments. Details of their use can be accessed from the CLI by running the program without any arguments.
