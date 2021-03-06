#ifndef _ISHARKL2_DFI_BM_H

#define _ISHARKL2_DFI_BM_H

#include "isharkl2_base_common.h"

#define TIME_MS_MAX	165356
#define DFS_TIMER_STEP  385


#define AON_APB_CGM_REG1				(AON_APB_CTL_BASE +0X0138)

#define BUSMON_TIMER_CFG				(BUSMON_TIMER_BASE)
#define BUSMON_TIMER1_HIGH_LEN		(BUSMON_TIMER_BASE+0x4)
#define BUSMON_TIMER1_LOW_LEN		(BUSMON_TIMER_BASE+0x8)
#define BUSMON_TIMER2_HIGH_LEN		(BUSMON_TIMER_BASE+0x10)
#define BUSMON_TIMER2_LOW_LEN		(BUSMON_TIMER_BASE+0x14)

#define PUB0_DFS_BUSMON_BASE						(PUB0_APB_RF_PUB0_BASE+0x4)
#define PUB0_DFS_BUSMON_CFG						(PUB0_DFS_BUSMON_BASE)
#define PUB0_DFS_BUSMON_INT_EN					(PUB0_DFS_BUSMON_BASE+0x4)
#define PUB0_DFS_BUSMON_INT_CLR					(PUB0_DFS_BUSMON_BASE+0x8)
#define PUB0_DFS_BUSMON_INT_STATUS				(PUB0_DFS_BUSMON_BASE+0xc)
#define PUB0_DFS_BUSMON_BDWIDTH_CNT_W0		(PUB0_DFS_BUSMON_BASE+0x10)
#define PUB0_DFS_BUSMON_BDWIDTH_CNT_W1		(PUB0_DFS_BUSMON_BASE+0x14)
#define PUB0_DFS_BUSMON_BDWIDTH_CNT_R0			(PUB0_DFS_BUSMON_BASE+0x18)
#define PUB0_DFS_BUSMON_BDWIDTH_CNT_R1			(PUB0_DFS_BUSMON_BASE+0x1c)

#define PUB0_DFS_BUSMON_OVERFLOW_THOLD_F0		(PUB0_DFS_BUSMON_BASE+0x20)
#define PUB0_DFS_BUSMON_UNDERFLOW_THOLD_F0	(PUB0_DFS_BUSMON_BASE+0x24)
#define PUB0_DFS_BUSMON_OVERFLOW_THOLD_F1		(PUB0_DFS_BUSMON_BASE+0x28)
#define PUB0_DFS_BUSMON_UNDERFLOW_THOLD_F1	(PUB0_DFS_BUSMON_BASE+0x2c)
#define PUB0_DFS_BUSMON_OVERFLOW_THOLD_F2		(PUB0_DFS_BUSMON_BASE+0x30)
#define PUB0_DFS_BUSMON_UNDERFLOW_THOLD_F2	(PUB0_DFS_BUSMON_BASE+0x34)
#define PUB0_DFS_BUSMON_OVERFLOW_THOLD_F3		(PUB0_DFS_BUSMON_BASE+0x38)
#define PUB0_DFS_BUSMON_UNDERFLOW_THOLD_F3	(PUB0_DFS_BUSMON_BASE+0x3c)
#define PUB0_DFS_BUSMON_BDWIDTH_BASE_NUM		(PUB0_DFS_BUSMON_BASE+0x40)

#define AON_CLK_PREDIV_GATE_EN_SEL3_CFG			(AON_PRE_DIV_CLK_GEN_BASE+0x48)
#define AON_CLK_PREDIV_GATE_EN_CTL3_CFG			(AON_PRE_DIV_CLK_GEN_BASE+0x5c)

extern void dfi_bm_cfg();
extern uint32 ddr_hwdfs_set(uint32 flag);

#endif