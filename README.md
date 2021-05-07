# beamformer
Lab B of ACA2021, CSIE, NTU

This repository includes modified source code from the article [Beamforming Acceleration](https://developer.xilinx.com/en/articles/beamforming-acceleration.html).

A presentation was conducted on 2021/5/11. The slides are available [here](https://docs.google.com/presentation/d/1082F2w-9es7I9r-RD_cF-dyrVSSd6v9SpDDj6bjOBNo).

## HLS

This folder includes 5 versions of optimizations.

`beamformer.cpp` is the original version without optimization, and `miller_bfr_decimal_tb.c` is its testbench. 4 optimizations are added sequentially to examine the improvements in latency and the resource usage. Please refer to the slides for more details.

## Vitis

This folder includes a partially working example of the same function on Vitis platform.

`src` contains all the required source codes, including `xcl2` from [SDAccel_Examples](https://github.com/Xilinx/SDAccel_Examples/tree/master/libs/xcl2). 

`reports` contains the auto-generated reports by Vitis.

`memo.txt` includes some commands and notes for troubleshooting.

### Unsolved issues

The SW emulation passed with only minor warnings, but HW emulation failed. The `LIBRARY_PATH` solution mentioned in [this thread](https://forums.xilinx.com/t5/Vitis-Acceleration-SDAccel-SDSoC/recipe-for-target-binary-container-1-xclbin-failed-make-binary/td-p/1137439) is tried, and the added environment variable is confirmed in logs, but HW simulation still fails.