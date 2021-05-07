#include <stdio.h>

// Simple set
// #define CHANNELS 4 //number of channels
// #define BEAMS 2    //number of beams to form
// #define SAMPLES 5  //number of samples per PRI

// Full set
#define CHANNELS 16  //number of channels
#define BEAMS 3      //number of beams to form
#define SAMPLES 2500 //number of samples per PRI

typedef int dio_t;

int main() {
    int s, b, c;

    dio_t rx_i[SAMPLES * CHANNELS] = { 0 };
    dio_t rx_q[SAMPLES * CHANNELS] = { 0 };
    for (s = 0; s < SAMPLES; s++) {
        for (c = 0; c < CHANNELS; c++) {
            rx_i[s * CHANNELS + c] = s + c + 1;
            rx_q[s * CHANNELS + c] = s + c + 1;
        }
    }

    dio_t w_i[BEAMS * CHANNELS] = { 0 };
    dio_t w_q[BEAMS * CHANNELS] = { 0 };
    for (b = 0; b < BEAMS; b++) {
        for (c = 0; c < CHANNELS; c++) {
            w_i[b * CHANNELS + c] = b * CHANNELS + c + 1;
            w_q[b * CHANNELS + c] = b * CHANNELS + c + 1;
        }
    }

    dio_t beamso_i_gold[SAMPLES * BEAMS] = { 0 };
    dio_t beamso_q_gold[SAMPLES * BEAMS] = { 0 };
    for (s = 0; s < SAMPLES; s++) {
        for (b = 0; b < BEAMS; b++) {
            for (c = 0; c < CHANNELS; c++) {
                beamso_i_gold[s * BEAMS + b] +=
                    ( rx_i[s * CHANNELS + c] * w_i[b * CHANNELS + c]
                    - rx_q[s * CHANNELS + c] * w_q[b * CHANNELS + c] );
                beamso_q_gold[s * BEAMS + b] +=
                    ( rx_i[s * CHANNELS + c] * w_q[b * CHANNELS + c]
                    + rx_q[s * CHANNELS + c] * w_i[b * CHANNELS + c] );
            }
        }
    }

    dio_t beamso_i[SAMPLES * BEAMS] = { 0 };
    dio_t beamso_q[SAMPLES * BEAMS] = { 0 };
    beamformer(rx_i, rx_q,
               w_i, w_q,
               beamso_i, beamso_q);

    int retval = 0;
    if (SAMPLES <= 10) {
        for (s = 0; s < SAMPLES; s++) {
            printf("\nSample %d\n", s);
            for (b = 0; b < BEAMS; b++) {
                printf(" Beam %d: %d + %d j\n", b, beamso_i[s * BEAMS + b], beamso_q[s * BEAMS + b]);
                if( beamso_i[s * BEAMS + b] != beamso_i_gold[s * BEAMS + b] ||
                    beamso_q[s * BEAMS + b] != beamso_q_gold[s * BEAMS + b] )
                    retval = 1;
            }
        }
    }

    // Compare the results file with the golden results
    if (retval != 0)
        printf("Test failed  !!!\n");
    else
        printf("Test passed !\n");

    // Return 0 if the test passes
    return retval;
}
