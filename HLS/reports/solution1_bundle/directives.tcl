############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode s_axilite -bundle control "beamformer"
set_directive_interface "beamformer"
set_directive_interface -mode m_axi -depth 128 -offset slave -bundle gmemi "beamformer" RXi_in1
set_directive_interface -mode m_axi -depth 128 -offset slave -bundle gmemi "beamformer" Bi_out
set_directive_interface -mode m_axi -depth 128 -offset slave -bundle gmemq "beamformer" RXq_in1
set_directive_interface -mode m_axi -depth 128 -offset slave -bundle gmemi "beamformer" Wi_in2
set_directive_interface -mode m_axi -depth 128 -offset slave -bundle gmemq "beamformer" Wq_in2
set_directive_interface -mode m_axi -depth 128 -offset slave -bundle gmemq "beamformer" Bq_out
set_directive_interface -mode s_axilite -bundle control "beamformer" RXi_in1
set_directive_interface -mode s_axilite -bundle control "beamformer" RXq_in1
set_directive_interface -mode s_axilite -bundle control "beamformer" Wi_in2
set_directive_interface -mode s_axilite -bundle control "beamformer" Wq_in2
set_directive_interface -mode s_axilite -bundle control "beamformer" Bi_out
set_directive_interface -mode s_axilite -bundle control "beamformer" Bq_out
