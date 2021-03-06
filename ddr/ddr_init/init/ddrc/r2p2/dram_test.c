/**************
**qos configure
**interleave configure
**lowpower
**Self-Refresh
**
**
************/
#include "ddrc_init.h"
#include "dram_test.h"

#ifdef DDR_SCAN_ENABLE
#define dmc_print_str(x) NULL
#define print_Hex(x) NULL
#endif
extern DRAM_INFO_T dram_info;
extern int rd_en_test_flag;
/**********************Bist*******************************/
u8 bist_data_mode[3]={FOUR_WORDS,FOUR_WORDS,TWO_WORDS};
//Open this define if you want userpattern define
#ifdef DDR_SCAN_ENABLE
u32 user_mode_pattern[]=
{
	BIST_DATA_00_PATTERN,
	BIST_DATA_01_PATTERN,
	BIST_DATA_02_PATTERN,
	BIST_DATA_03_PATTERN,
	BIST_DATA_04_PATTERN,
	BIST_DATA_05_PATTERN,
	BIST_DATA_06_PATTERN,
	BIST_DATA_07_PATTERN,
	BIST_DATA_08_PATTERN,
	BIST_DATA_09_PATTERN,
	BIST_DATA_10_PATTERN,
	BIST_DATA_11_PATTERN,
	BIST_DATA_12_PATTERN,
	BIST_DATA_13_PATTERN,
	BIST_DATA_14_PATTERN,
	BIST_DATA_15_PATTERN,
	BIST_DATA_16_PATTERN,
	BIST_DATA_17_PATTERN,
	BIST_DATA_18_PATTERN,
	BIST_DATA_19_PATTERN,
	BIST_DATA_20_PATTERN,
	BIST_DATA_21_PATTERN,
	BIST_DATA_22_PATTERN,
	BIST_DATA_23_PATTERN,
	BIST_DATA_24_PATTERN,
	BIST_DATA_25_PATTERN,
	BIST_DATA_26_PATTERN,
	BIST_DATA_27_PATTERN,
	BIST_DATA_28_PATTERN,
	BIST_DATA_29_PATTERN,
	BIST_DATA_30_PATTERN,
	BIST_DATA_31_PATTERN
};
#else

u32 user_mode_pattern[1]={0};
#endif
u32 sipi_mode_pattern[]=
{
	BIST_SIPI_DATA_00,
	BIST_SIPI_DATA_01,
	BIST_SIPI_DATA_02,
	BIST_SIPI_DATA_03,
	BIST_SIPI_DATA_04,
	BIST_SIPI_DATA_05,
	BIST_SIPI_BIT_PATTERN_0,
	BIST_SIPI_BIT_PATTERN_1
};

u32 lfsr_mode_pattern[] =
{
	BIST_DATA_LFSR_SEED_L0,
	BIST_DATA_LFSR_SEED_L1,
	BIST_DATA_LFSR_SEED_H0,
	BIST_DATA_LFSR_SEED_H1
};
void	bist_en()
{
	u32 master_bit_set = ((PORT6_PUBCP_AUDCP_EN<<6)|(PORT5_WTLCP_EN<<5)|(PORT4_AP_VSP_AON_EN<<4)
		|(PORT3_ISP_EN<<3)|(PORT2_DPU_DCAM_EN<<2)|(PORT1_GPU_EN<<1)|(PORT0_CPU_EN<<0));
	REG32(BIST_PORT_ADDR) = master_bit_set;
}
void	bist_en_single(u8 bist_chn)
{
	u32 master_bit_set =0x0;
	if(bist_chn == 0x0)
		master_bit_set=(0x1<<0x0);
	else if(bist_chn==0x1)
		master_bit_set=(0x1<<0x2);
	else if(bist_chn==0x2)
		master_bit_set=(0x1<<0x5);
	REG32(BIST_PORT_ADDR) = master_bit_set;
}

void bist_dis()
{
	REG32(BIST_PORT_ADDR)=0x0;
}

void bist_clear_reset(u32 bist_chn_num)
{
	volatile u32 val = 0;
	volatile u32 offset = (bist_chn_num*0x4000);
	reg_bit_set(BIST_BASE_ADDR+0x00+offset,2,1,0x0);
}

void bist_clear(u32 bist_chn_num)
{
	volatile u32 val = 0;
	volatile u32 offset = bist_chn_num*0x4000;
	reg_bit_set(BIST_BASE_ADDR+0x00+offset,2,1,0x1);
	reg_bit_set(BIST_BASE_ADDR+0x00+offset,1,1,0x0);
}

void bist_trigger(u32 bist_chn_num)
{
	volatile u32 val = 0;
	volatile u32 offset = (bist_chn_num*0x4000);
	reg_bit_set(BIST_BASE_ADDR+0x00+offset,1,1,0x1);
}

void bist_wait_done(u32 bist_chn_num)
{
		volatile u32 offset= (bist_chn_num*0x4000);
		while((__raw_readl(BIST_BASE_ADDR+0xb0+offset)&0x2) != 0x2);
}

u32 get_bist_result(u32 bist_chn_num)
{
	volatile u32 val =0;
	volatile u32 offset = bist_chn_num*0x4000;
	while((__raw_readl(BIST_BASE_ADDR+0xB0+offset)&0x2) != 0x2);

	if((__raw_readl(BIST_BASE_ADDR+0xB0+offset)&0x1) == 0x0)
	{
		return 0;
	}else
	{
		return 1;
	}
}

void bist_set(u32 bist_chn_num, u32 write_or_read, u32 data_pattern_mode, u32 bist_data_mode,
	u32 bist_size, u32 bist_src)
{
	volatile u32 i = 0;
	volatile u32 offset = bist_chn_num*0x4000;
	//bist clear
	bist_clear(bist_chn_num);
	__raw_writel(BIST_BASE_ADDR+0x00+offset,0x0);
	//bist enable
	reg_bit_set(BIST_BASE_ADDR+0x00+offset, 0, 1, 0x1);
	//bist_mode, 2'00:write 2'01 read, 2'10 all read after all write 2'11 read after write
	reg_bit_set(BIST_BASE_ADDR+0x00+offset, 3, 2, write_or_read);
	//bist_data_pattern_mode, 2'00:user data pattern, 2'01:sipi data pattern, 2'10:lfsr data pattern, 2'11:xtk_data_pattern
	reg_bit_set(BIST_BASE_ADDR+0x00+offset, 5, 2, data_pattern_mode);
	//bist_burst_length
	reg_bit_set(BIST_BASE_ADDR+0x00+offset, 7, 4,0xf);
	//bist_data_size, 3'000:byte, 3'001:half word, 3'010:word, 3'011:two words, 3'100:four words
	reg_bit_set(BIST_BASE_ADDR+0x00+offset,11, 3, bist_data_mode);
	//bist_write_outstanding_en/bist_read_outstanding_en
	reg_bit_set(BIST_BASE_ADDR+0x00+offset,24, 2, 0x3);
	//datawidth_mode
	reg_bit_set(BIST_BASE_ADDR+0x00+offset,28, 1, (dram_info.dram_type==DRAM_LP3)?0x1:0x0);
	//bist_trans_num
	__raw_writel(BIST_BASE_ADDR+0x04+offset,bist_size);
	//bist_start_addr
	__raw_writel(BIST_BASE_ADDR+0x08+offset,bist_src);
	switch(data_pattern_mode)
	{
		case USER_DATA_PATTERN:
			for(i = 0; i < 32; i++)
			{
				__raw_writel(BIST_BASE_ADDR+0x30+i*4+offset,user_mode_pattern[i]);
			}
			break;
		case SIPI_DATA_PATTERN:
			for(i = 0; i < 6; i++)
			{
				__raw_writel(BIST_BASE_ADDR+0x10+i*4+offset,sipi_mode_pattern[i]);
			}
			break;
		case LFSR_DATA_PATTERN:
			for(i = 0; i < 4; i++)
			{
				__raw_writel(BIST_BASE_ADDR+0xcc+i*4+offset,lfsr_mode_pattern[i]);
			}
			break;
	}

}

void bist_init(u32 write_or_read, u32 data_pattern_mode, u32 bist_size, u32* bist_src)
{
	u32 bist_chn_num=0;
	for(bist_chn_num=0;bist_chn_num <3;bist_chn_num++)
	{
		bist_set(bist_chn_num,write_or_read,data_pattern_mode,bist_data_mode[bist_chn_num],bist_size,bist_src[bist_chn_num]);
	}
}

void bist_init_for_scan(u32 bist_chn_num,u32 bist_start_addr,u32 bist_data_mode)
{
	u32 val=0;
	volatile u32 offset = bist_chn_num*0x4000;
	//bist clear
	bist_clear(bist_chn_num);
	switch(bist_data_mode)
	{
	case LFSR_DATA_PATTERN:
		 __raw_writel(BIST_BASE_ADDR+0x00+offset,0x000027d1);
		 reg_bit_set(BIST_BASE_ADDR+0x00+offset,28,1,(dram_info.dram_type==DRAM_LP3)?0x1:0x0);
		 __raw_writel(BIST_BASE_ADDR+0x04+offset,0x1ffe);
		 __raw_writel(BIST_BASE_ADDR+0x08+offset,bist_start_addr);
		 __raw_writel(BIST_BASE_ADDR+0xcc+offset,0x00d500ca);
		 __raw_writel(BIST_BASE_ADDR+0xd0+offset,0x00d700cb);
		 __raw_writel(BIST_BASE_ADDR+0xd4+offset,0x00ca00c5);
		 __raw_writel(BIST_BASE_ADDR+0xd8+offset,0x00e800d4);
		 break;
	case SIPI_DATA_PATTERN://m9+m12
		 __raw_writel(BIST_BASE_ADDR+0x00+offset,0x080023b1);
		 reg_bit_set(BIST_BASE_ADDR+0x00+offset,28,1,(dram_info.dram_type==DRAM_LP3)?0x1:0x0);
		 __raw_writel(BIST_BASE_ADDR+0x04+offset,0x170);
		 __raw_writel(BIST_BASE_ADDR+0x08+offset,bist_start_addr);
		 __raw_writel(BIST_BASE_ADDR+0x10+offset,0x8542a150);
		 __raw_writel(BIST_BASE_ADDR+0x14+offset,0x542a150a);
		 __raw_writel(BIST_BASE_ADDR+0x18+offset,0x540150a8);
		 __raw_writel(BIST_BASE_ADDR+0x1c+offset,0x40540540);
		 __raw_writel(BIST_BASE_ADDR+0x20+offset,0x05405450);
		 __raw_writel(BIST_BASE_ADDR+0x24+offset,0x54054054);
		 __raw_writel(BIST_BASE_ADDR+0x28+offset,0x00010001);
		 __raw_writel(BIST_BASE_ADDR+0x2c+offset,0x00010001);
		 __raw_writel(BIST_BASE_ADDR+0x30+offset,0x028140a0);
		 __raw_writel(BIST_BASE_ADDR+0x34+offset,0x28140a05);
		 __raw_writel(BIST_BASE_ADDR+0x38+offset,0xa800a050);
		 __raw_writel(BIST_BASE_ADDR+0x3c+offset,0x80a80a80);
		 __raw_writel(BIST_BASE_ADDR+0x40+offset,0x0a80a80a);
		 __raw_writel(BIST_BASE_ADDR+0x44+offset,0xa80a80a8);
		 break;
	}
}

#ifdef TEST_DEBUG
void print_bist_fail()
{
	u32 bist_fail_addr=0x0;
	u32 print_dram_addr=0x0;
	u32 i=0;
	reg_bit_set(0x327e0250,0,1,0x1);
	reg_bit_set(0x31053400,0,8,0x0);
	dmc_print_str("\r\nbist fail addr:");
	bist_fail_addr=__raw_readl(0x310700b4);
	print_Hex(bist_fail_addr);
	dmc_print_str("\r\nbist fail data:");
	for(i=0;i<4;i++)
	{
		print_Hex(__raw_readl(0x310700b8+i*0x4));
		dmc_print_str("  ");
	}
	dmc_print_str("\r\nbist exp data:");
	for(i=0;i<4;i++)
	{
		print_Hex(__raw_readl(0x310700e0+i*0x4));
		dmc_print_str("  ");
	}
}
#endif

void bist_test_entry_chn(u32 bist_chn_num,u32* bist_result)
{
	*bist_result=0;
	bist_en();
	bist_clear_reset(bist_chn_num);
	bist_trigger(bist_chn_num);
	bist_wait_done(bist_chn_num);
	*bist_result += get_bist_result(bist_chn_num);
//#if TEST_DEBUG_LOG_LEVEL>0
#if 1
	#ifdef TEST_DEBUG
	if(*bist_result>0)
	{
		dmc_print_str("\r\nbist test is fail!!!");
		//print_bist_fail();
		while(1);
	}else
	{
		dmc_print_str("\r\nbist test is pass!!!");
	}
	#endif
#endif
	bist_clear(bist_chn_num);
	bist_dis();
	return;
}

void bist_test_entry(u32* bist_result)
{
	*bist_result=0;
	bist_en();

	bist_clear_reset(BIST_CHN0);
	bist_clear_reset(BIST_CHN1);
	bist_clear_reset(BIST_CHN2);

	bist_trigger(BIST_CHN0);
	bist_trigger(BIST_CHN1);
	bist_trigger(BIST_CHN2);

	*bist_result += get_bist_result(BIST_CHN0);
	*bist_result += get_bist_result(BIST_CHN1);
	*bist_result += get_bist_result(BIST_CHN2);

	#ifdef TEST_DEBUG
	if(*bist_result>0)
	{
		while(1);
	}
	#endif

	bist_clear(BIST_CHN0);
	bist_clear(BIST_CHN1);
	bist_clear(BIST_CHN2);

	bist_dis();
	return;
}

void dram_bist_test()
{
	u32 bist_addr[3];
	u32 size;
	u32 bist_result=0;
	u32 bist_count;
	u32 bist_loop_times=0;

#if defined(CONFIG_NAND_SPL)
	#if defined(DRAM_TEST)||defined(AUTO_SLT_EN)
	if(dram_info.cs_num == 1)
	{
		bist_addr[0]=0x0;
		bist_addr[1]=(u32)(dram_info.cs0_size/4);
		bist_addr[2]=(u32)(dram_info.cs0_size/2);
	}else
	{
		bist_addr[0]=0x0;
		bist_addr[1]=(u32)(dram_info.cs0_size/2);
		bist_addr[2]=(u32)(dram_info.cs0_size);
	}
	size=((dram_info.cs0_size+dram_info.cs1_size-0x100000)>>8)-1;
#if TEST_DEBUG_LOG_LEVEL>0
	dmc_print_str("BIST TEST START\r\n");
#endif
	#if defined(DRAM_TEST)
	bist_loop_times=8;
	#endif
	#if defined(AUTO_SLT_EN)
	bist_loop_times=3;
	#endif
	for(bist_count=0;bist_count<bist_loop_times;bist_count++)
	{
		bist_init(BIST_ALLWRC,SIPI_DATA_PATTERN,size,bist_addr);
		bist_test_entry_chn(BIST_CHN0,&bist_result);
		bist_init(BIST_ALLWRC,LFSR_DATA_PATTERN,size,bist_addr);
		bist_test_entry_chn(BIST_CHN0,&bist_result);
		bist_init(BIST_ALLWRC,USER_DATA_PATTERN,size,bist_addr);
		bist_test_entry_chn(BIST_CHN0,&bist_result);
	}
	#else //else of #if defined(DRAM_TEST)||defined(AUTO_SLT_EN)
	if(dram_info.cs_num == 1)
	{
		bist_addr[0]=0x0;
		bist_addr[1]=(u32)(dram_info.cs0_size-0x1000);
	}else
	{
		bist_addr[0]=0x0;
		bist_addr[1]=(u32)(dram_info.cs0_size+dram_info.cs1_size-0x1000);
	}
	size=0xf;
	bist_init(BIST_ALLWRC,SIPI_DATA_PATTERN,size,bist_addr);
	bist_test_entry_chn(BIST_CHN0,&bist_result);
	bist_test_entry_chn(BIST_CHN1,&bist_result);
	#endif
#else //else of defined(CONFIG_NAND_SPL)
	if(dram_info.cs_num == 1)
	{
		bist_addr[0]=0x0;
		bist_addr[1]=(u32)(dram_info.cs0_size/4);
		bist_addr[2]=(u32)(dram_info.cs0_size/2);
	}else
	{
		bist_addr[0]=0x0;
		bist_addr[1]=(u32)(dram_info.cs0_size/2);
		bist_addr[2]=(u32)(dram_info.cs0_size);
	}
	size=(u32)(((dram_info.cs0_size+dram_info.cs1_size-0x100000)>>8)-1);
	bist_init(BIST_ALLWRC,SIPI_DATA_PATTERN,size,bist_addr);
	bist_test_entry_chn(BIST_CHN0,&bist_result);
	if(bist_result>0)
	{
		dmc_print_str("\r\nall of DRAM bist scan result is fail!!!!");
	}else
	{
		dmc_print_str("\r\nall of DRAM bist scan result is ok!!!!");
	}
#endif
}
/*************************************Interface repackaging SharkL5pro***********************************/
/*Enable bist and trigger bist_num:0~2*/
void bist_enable_trigger(u32 bist_chn_num)
{
	bist_en_single(bist_chn_num);
	bist_clear_reset(bist_chn_num);
	bist_trigger(bist_chn_num);
}
void print_bist_result(u8 bist_result)
{
#if 1
	#ifdef TEST_DEBUG
		if(bist_result>0)
		{
			dmc_print_str("\r\nbist test is fail!!!");
		//	print_bist_fail();
			while(1);
		}else
		{
			dmc_print_str("\r\nbist test is pass!!!");
		}
	#endif
#endif
}
u8 bist_test(BIST_INFO *bist_info)
{
	u8 bist_result=0;
	u32 bist_size=0;
	u32 bist_start_address=0;
	/*Configure bist and trigger*/
	switch (bist_info->bist_num)
		{
#ifdef ALL_BIST_CHN
		case 3:/*Enable 3 bist*/
			/*BIST 2*/
			bist_size =bist_info->bist2_end-bist_info->bist2_start;
			bist_start_address=bist_info->bist2_start;
			bist_size=(bist_size>>8)-1;//transform burst num unit,(four words*0xf=256=2^8,===>bist_size/(2^8))
			bist_set(BIST_CHN2,BIST_ALLWRC,SIPI_DATA_PATTERN,bist_data_mode[BIST_CHN2],bist_size,bist_start_address);
			bist_enable_trigger(BIST_CHN2);//enable and triger
#endif
		case 2:/*Enable 2 bist*/
			/*BIST 1*/
			bist_size =bist_info->bist1_end-bist_info->bist1_start;
			bist_start_address=bist_info->bist1_start;
			bist_size=(bist_size>>8)-1;//transform burst num unit,(four words*0xf=256=2^8,===>bist_size/(2^8))
			bist_set(BIST_CHN1,BIST_ALLWRC,SIPI_DATA_PATTERN,bist_data_mode[BIST_CHN1],bist_size,bist_start_address);
			bist_enable_trigger(BIST_CHN1);
			break;
		case 1:/*Enable 1 bist*/
			/*BIST 0*/
			bist_size =bist_info->bist0_end-bist_info->bist0_start;
			bist_start_address=bist_info->bist0_start;
			bist_size=(bist_size>>8)-1;//transform burst num unit,(four words*0xf=256=2^8,===>bist_size/(2^8))
			bist_set(BIST_CHN0,BIST_ALLWRC,SIPI_DATA_PATTERN,bist_data_mode[BIST_CHN0],bist_size,bist_start_address);
			bist_enable_trigger(BIST_CHN0);
			break;
		default:
#if TEST_DEBUG_LOG_LEVEL>0
			dmc_print_str("bist_test mode error\r\n");
#endif
			while(1);
		}
	/*Wait bist done and get bist result*/
	switch (bist_info->bist_num)
		{
#ifdef ALL_BIST_CHN
		case 3:
			bist_wait_done(BIST_CHN2);
			bist_result += get_bist_result(BIST_CHN2);
			bist_clear(BIST_CHN2);
#endif
		case 2:
			bist_wait_done(BIST_CHN1);
			bist_result += get_bist_result(BIST_CHN1);
			bist_clear(BIST_CHN1);
			break;
		case 1:
			bist_wait_done(BIST_CHN0);
			bist_result += get_bist_result(BIST_CHN0);
			bist_clear(BIST_CHN0);
			break;
		}
	/*Disable bist*/
	bist_dis();
	/*print bist result*/
	//print_bist_result(bist_result);

	return bist_result;/*0:pass >=1:fail*/
}
/*Just for three bist module test*/
BIST_INFO bist_info;
#ifdef ALL_BIST_CHN
int dram_bist_all_space_all_channel()
{
	bist_info.bist_num=3;
	bist_info.bist0_start=0x0;
	bist_info.bist0_end=dram_info.cs0_size;
	bist_info.bist1_start=dram_info.cs0_size;
	bist_info.bist1_end=dram_info.cs0_size+(dram_info.cs1_size/2);
	bist_info.bist2_start=dram_info.cs0_size+(dram_info.cs1_size/2);
	bist_info.bist2_end=dram_info.cs0_size+dram_info.cs1_size;

	if(bist_test(&bist_info) !=0)
		{
		dmc_print_str("\r\ndram_bist_all_space_all_channel\r\n");
	}


}
#endif
/*******************************************************
 * Scan DDR FULL Space except except_start~except_end
 *
 * except_start==except_end:scan 0~dram_size
 *******************************************************/

int dram_bist_all_space(u32 except_start,u32 except_end)
{
	u8 ret=0;
	u32 dram_size=0;

	bist_info.bist_num=2;//use for bist0

	if(dram_info.cs_num==2)
		dram_size=dram_info.cs0_size+dram_info.cs1_size;
	else if(dram_info.cs_num==1)
		dram_size=dram_info.cs0_size;

	if(except_start>except_end)
		{
		dmc_print_str("Error:dram_bist_all_space : except_start > except_end !! ");
		return -1;
	}
	/*scan bist 0 ~ except_start*/
		bist_info.bist1_start=0;
		bist_info.bist1_end=except_start;
		ret=bist_test(&bist_info);
		if(ret !=0)
			{
				dmc_print_str("dram_bist_all_space bist error\r\n");
				while(1);
				return ret;
			}
	/*scan bist except_end ~ dram end */
		bist_info.bist1_start=except_end;
		bist_info.bist1_end=dram_size-1;
		if(bist_info.bist1_start >= (bist_info.bist1_end-1))/*No need bist if end address > dram size */
			return 0;

		ret=bist_test(&bist_info);
		if(ret !=0)
			{
				dmc_print_str("dram_bist_all_space bist error\r\n");
				while(1);
				return ret;
			}
	return 0;

}
int ddr_scan_fdl1_test(int start, int end)
{
	dmc_print_str("\r\nScan all ddr space Start!!\r\n");
	print_Hex(start);
	print_Hex(end);
	reg_bit_set(PUB_SYS_AUTO_LIGHT_SLEEP_ENABLE,0,16,0x0);
	reg_bit_set(DDR_CHN_SLEEP_CTRL0,16,10,0x0);
	reg_bit_set(LIGHT_SLEEP_ENABLE,10,1,0x0);
	dram_bist_all_space(start,end);
	dmc_print_str("\r\nScan all ddr space End!!\r\n");
	return 0;
}
int ddr_selfrefresh_test(u32 timeout)
{
	return 0;
}
/*scan ddr specific(start~end) space*/
int dram_bist_specific_space(u32 start_address,u32 end_address)
{
	u8 ret=0;
	u64 dram_size=0;

	bist_info.bist_num=1;//use for bist0

	if(dram_info.cs_num==2)
		dram_size=dram_info.cs0_size+dram_info.cs1_size;
	else if(dram_info.cs_num==1)
		dram_size=dram_info.cs0_size;

	if((start_address>end_address)||(start_address > dram_size-1)||(end_address > dram_size-1))
		{
		dmc_print_str("Error:dram_bist_specific_space : start > end or scan address > dram_size ");
		return 1;
	}

	bist_info.bist0_start=start_address;
	bist_info.bist0_end=end_address;
	ret=bist_test(&bist_info);
	if(ret !=0)
		{
			if(!rd_en_test_flag)
			dmc_print_str("dram_bist_specific_space bist error\r\n");
			return ret;
		}
	return 0;
}
/*bist for SPL*/
void dram_bist_spl_test()
{
	u8 ret =0;

	ret= dram_bist_specific_space(0x0,0x1000);//512Byte
	print_bist_result(ret);
}
/*bist for FDL1*/
void dram_bist_fdl_test()
{
	u32 start_address=0x0;
	u32 end_address =(u32)((dram_info.cs0_size+dram_info.cs1_size-0x100000));
	u8 ret =0;

	ret = dram_bist_specific_space(start_address,end_address);
	print_bist_result(ret);
}
/************************************************************************
*scan_mode:
*		 SCAN_ALL_ADDRESS_DRAM	    :Scan all dram space except start~end
*		 SCAN_SPECIFIC_ADDRESS_DRAM :Scan start~end  dram space
*************************************************************************/
int dmc_ddr_test(BIST_SCAN_MODE scan_mode,u32 start,u32 end)
{
	int ret=0;
	switch (scan_mode)
		{
		case SCAN_ALL_ADDRESS_DRAM:
			ret=dram_bist_all_space(start,end);
			if(ret == 1)
			{
			//dmc_print_str("Error:SCAN_ALL_ADDRESS_DRAM!! while loop\r\n");
			while(1);
			}else
			{
			dmc_print_str("SCAN_ALL_ADDRESS_DRAM pass\r\n");
			}
			return ret;
		case SCAN_SPECIFIC_ADDRESS_DRAM:
			ret= dram_bist_specific_space(start,end);
			if(ret == 1)
			{
			//dmc_print_str("Error:SCAN_SPECIFIC_ADDRESS_DRAM error!! while loop\r\n");
			while(1);
			}
			return ret;
		case DRAM_BIST_TEST:
			dram_bist_spl_test();
			break;
		default:
			dmc_print_str("Error:dmc_ddr_test:scan_mode error!!\r\n");
			return -1;
		}
	return 0;
}
/*bist for all freq*/
void dram_bist_all_freq(u32 ddr_mode)
{
	u32 current_freq=0;
	int fn=0;
	if(dram_info.dram_type!=DRAM_LP3)
		{
			current_freq=reg_bit_get(DMC_CTL0_(0x12c),8,3);//Current high freq
			if(current_freq>7)
				{
				//dmc_print_str("Error for dram_bist_all_freq");
				while(1);
				}
			ddr_mode =ddr_mode & (~(0x03<<8));//256 384
			if(current_freq==0x7)
				{
				dram_bist_spl_test();
				sw_dfs_go(4);//1024
				dpll_cfg(1333);
				current_freq=current_freq-1;
				}
			for(fn=current_freq;fn>=0;fn--)
			{
			if(((ddr_mode >> (8+fn))&0x01)== 0)
				{
				switch (fn)
					{
					case 6://1536
						//vddcore change to 900mv
						regulator_set_voltage("vddcore",900);
						break;
					case 5://1333
						regulator_set_voltage("vddcore",800);
						break;
					default:
						regulator_set_voltage("vddcore",750);
						break;
					}
				sw_dfs_go(fn);
				dram_bist_spl_test();
				}

			}
		}

}

#ifdef DRAM_MEM_TEST

/************************memtest***************************************/
static int compare_20_times = 0;
struct memtest{
	char *name;
	int (*fp)();
	};

struct memtest tests[] = {
	{ "Random Value", test_random_value },
	{ "Compare XOR", test_xor_comparison },
	{ "Compare SUB", test_sub_comparison },
	{ "Compare MUL", test_mul_comparison },
	{ "Compare DIV",test_div_comparison },
	{ "Compare OR", test_or_comparison },
	{ "Compare AND", test_and_comparison },
	{ NULL, NULL }
	};

static u32 seed = 0x2345abcd;

u32 rand_u32(){
	u32 d;
	d=((seed >> 3)^seed)&0x1ffffffe;
	seed=(seed>>29)|d<<2;
	d=((seed>>3)^seed)&0xf;
	seed>>=3;
	seed|=d<<28;
	return seed;
}


void readRepeat(u32 *p1, u32 *p2, u32 *bufa, u32 *bufb, u32 count, int n_repeat) {
	int i = 0;
	int resu32t=0;
	for(i = 0; i < n_repeat; i++) {
		resu32t = (*p1 == *p2)? 0: -1;
		if (resu32t){
			compare_20_times ++;
			print_String("ReadRepeat addr:");
			print_Hex((u32)*p1);
			print_String("!=");
			print_String("addr:");
			print_Hex((u32)*p2);
			print_String("\n");
		}
	}
}

int compare_regions(u32 *bufa, u32 *bufb, u32 count){
	int r = 0;
	u32 i,j;
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	for (j = 0; j < 20; j++){
		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++, p1++, p2++){
			if (*p1 != *p2){
				readRepeat(p1, p2, bufa, bufb, count, 10);                /* printf("Skipping to next test..."); */
				if (compare_20_times == 10){
					r = -1;
				}else{
					r = 0; /* complete the last comparison.*/
				}
			}
		}
	}
	return r;
}

int test_stuck_address(u32 *bufa, u32 count){
	u32 *p1 = bufa;
	unsigned int j;
	u32  i;
	for (j = 0; j < 1; j++){
		p1 = (u32*) bufa;
		for(i = 0; i < count; i++){
			*p1 = ((j + i) % 2) == 0 ? (u32) p1 : ~((u32) p1);
			p1++;
		}
		p1 = (u32*) bufa;
		for(i = 0; i < count; i++, p1++){
			if (*p1 != (((j + i) % 2) == 0 ? (u32) p1 : ~((u32) p1))){
				print_String("FAILURE: possible bad address line at physical address 0x:");
				print_Hex((u32*)p1);
				print_String("\n");
				while(1);
			}
		}
	}
	print_String("Stuck address test pass\n");
	return 0;
}

int test_random_value(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	for (i = 0; i < count; i++){
		*p1++ = *p2++ = rand_u32();
	}
	return compare_regions(bufa, bufb, count);
}

int test_xor_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	u32 q = rand_u32();
	for (i = 0; i < count; i++){
		*p1++ ^= q;
		*p2++ ^= q;
	}
	return compare_regions(bufa, bufb, count);
}

int test_sub_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	u32 q = rand_u32();
	for (i = 0; i < count; i++){
		*p1++ -= q;
		*p2++ -= q;
	}
	return compare_regions(bufa, bufb, count);
}

int test_mul_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	u32 q = rand_u32();
	for (i = 0; i < count; i++){
		*p1++ *= q;
		*p2++ *= q;
	}
	return compare_regions(bufa, bufb, count);
}

int test_div_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	u32 q = rand_u32();
	for (i = 0; i < count; i++){
		if (!q){
			q++;
		}
		*p1++ /= q;
		*p2++ /= q;
	}
	return compare_regions(bufa, bufb, count);
}

int test_or_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	u32 q = rand_u32();
	for (i = 0; i < count; i++){
		*p1++ |= q;
		*p2++ |= q;
	}
	return compare_regions(bufa, bufb, count);
}

int test_and_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	u32 q = rand_u32();
	for (i = 0; i < count; i++){
		*p1++ &= q;
		*p2++ &= q;
	}
	return compare_regions(bufa, bufb, count);
}

int test_seqinc_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	u32 i;
	u32 q = rand_u32();
	for (i = 0; i < count; i++){
		*p1++ = *p2++ = (i + q);
	}
	return compare_regions(bufa, bufb, count);
}

int test_solidbits_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	unsigned int j;
	u32 q;
	u32 i;
	for (j = 0; j < 64; j++){
		q = (j % 2) == 0 ? UL_ONEBITS : 0;
		p1 = (u32 *) bufa;
		p2 = (u32 *) bufb;
		for (i = 0; i < count; i++){
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}
		if (compare_regions(bufa, bufb, count)){
			return -1;
		}
	}
	return 0;
}

int test_checkerboard_comparison(u32 *bufa, u32 *bufb, u32 count)
{
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	unsigned int j;
	u32 q;
	u32 i;
	for (j = 0; j < 64; j++){
		q = (j % 2) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
		p1 = (u32 *) bufa;
		p2 = (u32 *) bufb;
		for (i = 0; i < count; i++)
		{
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}
		if (compare_regions(bufa, bufb, count)){
			return -1;
		}
	}
	return 0;
}

int test_blockseq_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	unsigned int j;
	u32 i;
	for (j = 0; j < 256; j++){
		p1 = (u32 *) bufa;
		p2 = (u32 *) bufb;
		for (i = 0; i < count; i++){
			*p1++ = *p2++ = (u32) UL_BYTE(j);
		}
		if (compare_regions(bufa, bufb, count)){
			return -1;
		}
	}
	return 0;
}

int test_walkbits0_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	unsigned int j;
	u32 i;
	for (j = 0; j < UL_LEN * 2; j++){
		p1 = (u32 *) bufa;
		p2 = (u32 *) bufb;
		//printf("setting %3u", j);
		for (i = 0; i < count; i++){
			if (j < UL_LEN) { /* Walk it up. */
				*p1++ = *p2++ = ONE << j;
			} else { /* Walk it back down. */
			*p1++ = *p2++ = ONE << (UL_LEN * 2 - j - 1);
			}
		}
		if (compare_regions(bufa, bufb, count)){
			return -1;
		}
	}
	return 0;
}

int test_walkbits1_comparison(u32 *bufa, u32 *bufb, u32 count) {
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	unsigned int j;
	u32 i;
	for (j = 0; j < UL_LEN * 2; j++) {
		p1 = (u32 *) bufa;
		p2 = (u32 *) bufb;
		//printf("setting %3u", j);
		for (i = 0; i < count; i++){
			if (j < UL_LEN) { /* Walk it up. */
				*p1++ = *p2++ = UL_ONEBITS ^ (ONE << j);
			} else { /* Walk it back down. */
			*p1++ = *p2++ = UL_ONEBITS ^ (ONE << (UL_LEN * 2 - j - 1));
			}
		}
		if (compare_regions(bufa, bufb, count)){
			return -1;
		}
	}
	return 0;
}

int test_bitspread_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	unsigned int j;
	u32 i;
	for (j = 0; j < UL_LEN * 2; j++){
		p1 = (u32 *) bufa;
		p2 = (u32 *) bufb;
		for (i = 0; i < count; i++){
			if (j < UL_LEN) { /* Walk it up. */
				*p1++ = *p2++ = (i % 2 == 0)? (ONE << j) | (ONE << (j + 2)): UL_ONEBITS ^ ((ONE << j)| (ONE << (j + 2)));
				} else { /* Walk it back down. */
				*p1++ = *p2++ = (i % 2 == 0)? (ONE << (UL_LEN * 2 - 1 - j)) |
					(ONE << (UL_LEN * 2 + 1 - j)): UL_ONEBITS ^ (ONE << (UL_LEN * 2 - 1 - j)|
					(ONE << (UL_LEN * 2 + 1 - j)));
				}
			}
		if (compare_regions(bufa, bufb, count)){
			return -1;
		}
	}
	return 0;
}

int test_bitflip_comparison(u32 *bufa, u32 *bufb, u32 count){
	u32 *p1 = bufa;
	u32 *p2 = bufb;
	unsigned int j, k;
	u32 q;
	u32 i;
	for (k = 0; k < UL_LEN; k++){
		q = ONE << k;
		for (j = 0; j < 8; j++){
			q = ~q;
			p1 = (u32 *) bufa;
			p2 = (u32 *) bufb;
			for (i = 0; i < count; i++){
				*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
			}
			if (compare_regions(bufa, bufb, count)){
				return -1;
			}
		}
	}
	return 0;
}

void ddr_memtester(void){
	u32 regval;
	test_stuck_address((u32*)DST_ADDR, MEM_TEST_LEN);
	for (regval=0; ; regval++){
		if (!tests[regval].name)
			break;
		if (!tests[regval].fp((u32 *)DST_ADDR, (u32 *)SOURCE_ADDR, MEM_TEST_LEN)){
			switch (regval){
				case 0:
					print_String("Random Value ");
					break;
				case 1:
					print_String("Compare XOR ");
					break;
				case 2:
					print_String("Compare SUB ");
					break;
				case 3:
					print_String("Compare MUL ");
					break;
				case 4:
					print_String("Compare DIV ");
					break;
				case 5:
					print_String("Compare OR  ");
					break;
				case 6:
					print_String("Compare AND ");
					break;
			}
			print_String(" test pass\n");
		}else{
			switch (regval){
				case 0:
					print_String("Random Value ");
					break;
				case 1:
					print_String("Compare XOR ");
					break;
				case 2:
					print_String("Compare SUB ");
					break;
				case 3:
					print_String("Compare MUL ");
					break;
				case 4:
					print_String("Compare DIV ");
					break;
				case 5:
					print_String("Compare OR  ");
					break;
				case 6:
					print_String("Compare AND ");
					break;
			}
			print_String(" test failed\n");
			while(1);
		}
	}
}

void ddr_mem_test(void){
	int i;
	unsigned int regval;
	for (i = 0; i < MEM_TEST_LEN; i+=4){
		REG32(SOURCE_ADDR + i) = SOURCE_ADDR + i;
		REG32(DST_ADDR + i) = 0;
	}	//while(1){//		memset((void*)DST_ADDR, 0x0, MEM_TEST_LEN);
	for ( i = 0; i < MEM_TEST_LEN; i+= 1024){
		sprd_memcpy((void*)(DST_ADDR + i), (void*)(SOURCE_ADDR + i), 1024);
		regval = sprd_memcmp((void*)(SOURCE_ADDR + i), (void*)(DST_ADDR + i),1024);
		if (regval){
			dmc_print_str("little ddr copy failed\n");
			while(1);
		}else{
		//		dmc_print_str("ddr copy pass\n");
		}
	}
	regval = sprd_memcmp((void*)(SOURCE_ADDR), (void*)(DST_ADDR),MEM_TEST_LEN);
	if(regval){
		dmc_print_str("big ddr cmp failed\n");
		while(1);
	}else{
		dmc_print_str("big mem cmp pass\n");
	}
}

void dram_mem_test(u32 src_addr,u32 dst_addr)
{
	ddr_memtester();
	ddr_mem_test();
}
#endif
