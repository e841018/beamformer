#ifndef __BEAMFORMER_H__
#define __BEAMFORMER_H__

#include <stdio.h>
#include <string.h>

// Simple set
// #define CHANNELS 4 //number of channels
// #define BEAMS 2    //number of beams to form
// #define SAMPLES 5  //number of samples per PRI

// Full set
#define CHANNELS 16  //number of channels
#define BEAMS 3      //number of beams to form
#define SAMPLES 2500 //number of samples per PRI

void beamformer(
	const int *RXi_in1, // Read-Only Matrix 1
	const int *RXq_in1, // Read-Only Matrix 1
	const int *Wi_in2,  // Read-Only Matrix 2
	const int *Wq_in2,  // Read-Only Matrix 2
	int *Bi_out,        // Output Result
	int *Bq_out         // Output Result
);

#endif
