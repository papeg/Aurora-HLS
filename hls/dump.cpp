/*
 * Copyright 2022 Xilinx, Inc.
 *           2023-2024 Gerrit Pape (papeg@mail.upb.de)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hls_stream.h>
#include <ap_int.h>
#include <ap_axi_sdata.h>

#ifndef DATA_WIDTH_BYTES
#define DATA_WIDTH_BYTES 64
#endif

#define DATA_WIDTH (DATA_WIDTH_BYTES * 8)


extern "C"
{
    void dump (hls::stream<ap_axiu<DATA_WIDTH, 0, 0, 0>>& data_input,
                    ap_uint<DATA_WIDTH> *data_output,
                    unsigned int byte_size,
                    unsigned int iterations,
                    bool ack_enable,
                    hls::stream<ap_axiu<1, 0, 0, 0>>& ack_stream)
    {
    iterations:
        for (unsigned int n = 0; n < iterations; n++) {
        read:
            for (int i = 0; i < (byte_size / DATA_WIDTH_BYTES); i++) {
#pragma HLS PIPELINE II = 1
                ap_axiu<DATA_WIDTH, 0, 0, 0> temp = data_input.read();
                data_output[i] = temp.data;
            }
            if (ack_enable) {
                ap_axiu<1, 0, 0, 0> ack;
                ack_stream.write(ack);
            }
        }
}
}


