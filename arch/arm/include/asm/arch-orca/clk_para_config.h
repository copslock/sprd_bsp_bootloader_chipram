/******************************************************************************
 ** File Name:                                                                *
 ** Author:                                                                   *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic input and output operations   *
 **                 on hardware, it can be treated as a hardware abstract     *
 **                 layer interface.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/12/2010                                                                *
 ******************************************************************************/

#ifndef _CLK_PARA_CONFIG_H_
#define _CLK_PARA_CONFIG_H_

#ifdef __cplusplus
"C"
{
#endif

typedef struct{
		volatile char name[8];
		volatile unsigned int value;
}CLK_VOL_UNIT;

typedef struct{
		volatile char name[8];
		volatile unsigned char lvl_count;
		volatile unsigned char lvl;
		volatile unsigned char lvl_count_max;
		volatile unsigned char reserved0;
		volatile unsigned int clk_list[8];
}CLK_LIST_UNIT;

typedef struct{
    volatile unsigned int magic_header;
    volatile unsigned char version;
    volatile unsigned char reserved0;
    volatile unsigned char reserved1;
    volatile unsigned char reserved2;
    volatile unsigned int  clk_vol_unit_count;//16
    volatile unsigned int clk_list_unit_count;//6
    CLK_VOL_UNIT core0_freq;
    CLK_VOL_UNIT core1_freq;
    CLK_VOL_UNIT clk_scu;
    CLK_VOL_UNIT ddr_freq;
    CLK_VOL_UNIT dcdc_arm;
    CLK_VOL_UNIT dcdc_core;
    CLK_VOL_UNIT dcdc_mem;
    CLK_VOL_UNIT dcdc_gen0;
    CLK_VOL_UNIT dcdc_gen1;
    CLK_VOL_UNIT dcdc_gpu;
    CLK_VOL_UNIT dcdc_memq;
    CLK_VOL_UNIT dcdc_sram;
    CLK_VOL_UNIT debug_flags[4];
    CLK_LIST_UNIT algio_freq;
    CLK_LIST_UNIT clk_periph;
    CLK_LIST_UNIT axi_freq;
    CLK_LIST_UNIT peri_apb_freq;
    CLK_LIST_UNIT apcpu_axi_freq;
    CLK_LIST_UNIT aon_apb_freq;
    volatile unsigned int  magic_end;
}MCU_CLK_PARA_T;

extern const MCU_CLK_PARA_T mcu_clk_para;

#ifdef __cplusplus
}
#endif

#endif
