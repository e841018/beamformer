/**********
Copyright (c) 2018, Xilinx, Inc.
All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

/*
Kernel Description :

    Matrix multiply
    This example showcases how reordering the loops helps achieve a better
    pipeline initiation interval (II) and better performance.

    Arguments :

        int *in1   (input)     --> Input  Matrix 1
        int *in2   (input)     --> Input  Matrix 2
        int *out_r   (output)    --> Output Matrix
*/
#include "beamformer.h"

// Computes matrix multiply
void beamformer(
    const int *RXi_in1, // Read-Only Matrix 1
    const int *RXq_in1, // Read-Only Matrix 1
    const int *Wi_in2,  // Read-Only Matrix 2
    const int *Wq_in2,  // Read-Only Matrix 2
    int *Bi_out,        // Output Result
    int *Bq_out         // Output Result
) {
    // #pragma HLS INTERFACE m_axi port = RXi_in1 offset = slave bundle = gmem
    // #pragma HLS INTERFACE m_axi port = RXq_in1 offset = slave bundle = gmem
    // #pragma HLS INTERFACE m_axi port = Wi_in2 offset = slave bundle = gmem
    // #pragma HLS INTERFACE m_axi port = Wq_in2 offset = slave bundle = gmem
    // #pragma HLS INTERFACE m_axi port = Bi_out offset = slave bundle = gmem
    // #pragma HLS INTERFACE m_axi port = Bq_out offset = slave bundle = gmem

    // #pragma HLS INTERFACE s_axilite port = RXi_in1 bundle = control
    // #pragma HLS INTERFACE s_axilite port = RXq_in1 bundle = control
    // #pragma HLS INTERFACE s_axilite port = Wi_in2 bundle = control
    // #pragma HLS INTERFACE s_axilite port = Wq_in2 bundle = control
    // #pragma HLS INTERFACE s_axilite port = Bi_out bundle = control
    // #pragma HLS INTERFACE s_axilite port = Bq_out bundle = control
    // #pragma HLS INTERFACE s_axilite port = return bundle = control

    // Local memory to store input and output matrices
    // Local memory is implemented as BRAM memory blocks
    int RXi[SAMPLES][CHANNELS];
    int RXq[SAMPLES][CHANNELS];
    int Wi[BEAMS][CHANNELS];
    int Wq[BEAMS][CHANNELS];
    int Bi[SAMPLES][BEAMS];
    int Bq[SAMPLES][BEAMS];
    int itr, i, j, k;

    // #pragma HLS ARRAY_PARTITION variable = RXi dim = 2 complete
    // #pragma HLS ARRAY_PARTITION variable = RXq dim = 2 complete
    // #pragma HLS ARRAY_PARTITION variable = Wi dim = 2 complete
    // #pragma HLS ARRAY_PARTITION variable = Wq dim = 2 complete

    // Burst read for matrix A
readA:
    for (itr = 0, i = 0, j = 0; itr < SAMPLES * CHANNELS; itr++, j++) {
        // #pragma HLS PIPELINE II=1
        if (j == CHANNELS) {
            j = 0;
            i++;
        }
        RXi[i][j] = RXi_in1[itr];
        RXq[i][j] = RXq_in1[itr];
    }

    // Burst read for matrix B
readB:
    for (itr = 0, i = 0, j = 0; itr < CHANNELS * BEAMS; itr++, j++) {
        // #pragma HLS PIPELINE II=1
        if (j == CHANNELS) {
            j = 0;
            i++;
        }
        Wi[i][j] = Wi_in2[itr];
        Wq[i][j] = Wq_in2[itr];
    }

    // Performs matrix multiply
loop1:
    for (i = 0; i < SAMPLES; i++) {

    loop2:
        for (j = 0; j < BEAMS; j++) {
            // #pragma HLS PIPELINE II=1
            int result_i = 0;
            int result_q = 0;

        loop3:
            for (k = 0; k < CHANNELS; k++) {
                result_i += RXi[i][k] * Wi[j][k] - RXq[i][k] * Wq[j][k];
                result_q += RXi[i][k] * Wq[j][k] + RXq[i][k] * Wi[j][k];
            }
            Bi[i][j] = result_i;
            Bq[i][j] = result_q;
        }
    }

    // Burst write from matrix C
writeC:
    for (itr = 0, i = 0, j = 0; itr < SAMPLES * BEAMS; itr++, j++) {
        if (j == BEAMS) {
            j = 0;
            i++;
        }
        Bi_out[itr] = Bi[i][j];
        Bq_out[itr] = Bq[i][j];
    }
}
