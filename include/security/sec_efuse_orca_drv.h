#ifndef _ORCA_EFUSE_DRIVE_H
#define _ORCA_EFUSE_DRIVE_H
#include <sci_types.h>
#include <timer.h>
#include <asm/arch-orca/chip_orca/aon_apb.h>
/******************/
typedef enum {
	EFUSE_RESULT_SUCCESS = 0,
	EFUSE_RD_ERROR,
	EFUSE_WR_ERROR,
	EFUSE_PARAM_ERROR
} Efuse_Result_Ret;

/******************/
#define EFUSE_MAGIC				0x8810
#define ERR_FLAG_MASK				0xFFFF
#define BIT_MASK(bit)  				(~(1<<bit))

/*********EFUSE CLK**************************************/
#define REG_AON_SEC_APB_EFUSE_SEC_ENABLE	REG_AON_APB_APB_EB1

/*******EFUSE CONTROL REGISTER***************************/
#define REG_AON_EFUSE_BASE			0x63400000
#define EFUSE_ALL0_INDEX			(REG_AON_EFUSE_BASE + 0x8)
#define EFUSE_MODE_CTRL				(REG_AON_EFUSE_BASE + 0xC)
#define EFUSE_IP_VER				(REG_AON_EFUSE_BASE + 0x14)
#define EFUSE_CFG0				(REG_AON_EFUSE_BASE + 0x18)
#define EFUSE_SEC_EN				(REG_AON_EFUSE_BASE + 0x40)
#define EFUSE_SEC_ERR_FLAG			(REG_AON_EFUSE_BASE + 0x44)
#define EFUSE_SEC_FLAG_CLR			(REG_AON_EFUSE_BASE + 0x48)
#define EFUSE_SEC_MAGIC_NUM			(REG_AON_EFUSE_BASE + 0x4C)
#define EFUSE_FW_CFG				(REG_AON_EFUSE_BASE + 0x50)
#define EFUSE_PW_SWT				(REG_AON_EFUSE_BASE + 0x54)
#define BLOCK_MAP				(REG_AON_EFUSE_BASE + 0x1000)

extern void efuse_enable(void);
extern void efuse_disable(void);
extern Efuse_Result_Ret efuse_write_drv(uint32 start_id, uint32 end_id, uint32 *pReadData,uint32 Isdouble);
extern Efuse_Result_Ret efuse_read_drv(uint32 start_id, uint32 end_id, uint32 *pReadData,uint32 Isdouble);
extern Efuse_Result_Ret efuse_rewrite_double_bit(uint32 block_id, uint32 writeData);

#endif
