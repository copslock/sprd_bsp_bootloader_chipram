#include "ddrc_init.h"
/**************************************LP4 DMC DTMG**************************************/
#define DMC_LP4_1333_MR1		0x34
#define DMC_LP4_1333_MR2		0x24
#define DMC_LP4_1333_MR3		0x33
#define DMC_LP4_1333_MR22		0x38
#define	DMC_LP4_1333_DTMG0		0x48100D0D
#if LP4X_ODT_EN(7)
#define	DMC_LP4_1333_DTMG1		0x02107C03
#else
#define	DMC_LP4_1333_DTMG1		0x02107C03
#endif
#if LP4_ODT_EN(7)
#define	DMC_LP4_1333_DTMG1_RW	0x02107C03
#else
#define	DMC_LP4_1333_DTMG1_RW	0x02107C03
#endif
#define	DMC_LP4_1333_DTMG2		0x773B5926
#if defined(VRCG_ON_EN)
#define	DMC_LP4_1333_DTMG3		0xA5091B1E
#else
#define	DMC_LP4_1333_DTMG3		0xA50F1B1E
#endif
#define	DMC_LP4_1333_DTMG4		0x22630605
#define	DMC_LP4_1333_DTMG5		0x00000506
#define	DMC_LP4_1333_DTMG6		0x00000C0B
#define	DMC_LP4_1333_DTMG7		0x0086029A	//drf_self_ref_idle_cnt[15:0] ddr_clk/2
#define	DMC_LP4_1333_DTMG8		0x229C0000
#define	DMC_LP4_1333_DTMG9		0x00030C80
#define	DMC_LP4_1333_DTMG10		0x15140002
//MRW Flow
#if defined(VRCG_ON_EN)
#define	DMC_LP4_1333_DTMG11		0x00000000
#define	DMC_LP4_1333_DTMG12		0x00000000
#if LP4X_ODT_EN(7)
#define	DMC_LP4_1333_DTMG13_RW	0x01340224
#define	DMC_LP4_1333_DTMG14_RW	0x0e0e0b05
#else
#define	DMC_LP4_1333_DTMG13_RW	0x01340224
#define	DMC_LP4_1333_DTMG14_RW	0x0e5d0b00
#endif
#if LP4_ODT_EN(7)
#define	DMC_LP4_1333_DTMG13		0x01340224
#define	DMC_LP4_1333_DTMG14		0x0e0e0b05
#else
#define	DMC_LP4_1333_DTMG13		0x01340224
#define	DMC_LP4_1333_DTMG14		0x0e5d0b00
#endif
#define	DMC_LP4_1333_DTMG15		0x00000000
#define	DMC_LP4_1333_DTMG16		0x00000000
#define	DMC_LP4_1333_DTMG17		0x00000000
#define	DMC_LP4_1333_DTMG18		0x00000000
#else //vrcg off
#if LP4X_ODT_EN(7)
#define	DMC_LP4_1333_DTMG11		0x0b050224
#define	DMC_LP4_1333_DTMG12		0x0d080e0e
#else
#define	DMC_LP4_1333_DTMG11		0x0b050224
#define	DMC_LP4_1333_DTMG12		0x0d080e5d
#endif
#if LP4_ODT_EN(7)
#define	DMC_LP4_1333_DTMG11_RW	0x0b050224
#define	DMC_LP4_1333_DTMG12_RW	0x0d080e0e
#else
#define	DMC_LP4_1333_DTMG11_RW	0x0b050224
#define	DMC_LP4_1333_DTMG12_RW	0x0d080e5d
#endif
#define	DMC_LP4_1333_DTMG13		0x01340134
#define	DMC_LP4_1333_DTMG14		0x01340134
#define	DMC_LP4_1333_DTMG15		0x01340134
#define	DMC_LP4_1333_DTMG16		0x01340134
#define	DMC_LP4_1333_DTMG17		0x0d000134
#define	DMC_LP4_1333_DTMG18		0x01340134
#endif
#define	DMC_LP4_1333_DTMG19		0xFFFFFFFF


#define DMC_LP4_1200_MR1		0x34
#define DMC_LP4_1200_MR2		0x24
#define DMC_LP4_1200_MR3		0x33
#define DMC_LP4_1200_MR22		0x38
#define	DMC_LP4_1200_DTMG0		0x470F0C0C
#if	LP4X_ODT_EN(6)
#define	DMC_LP4_1200_DTMG1		0x02107B02
#else
#define	DMC_LP4_1200_DTMG1		0x02107B02
#endif
#if	LP4_ODT_EN(6)
#define	DMC_LP4_1200_DTMG1_RW	0x02107B02
#else
#define	DMC_LP4_1200_DTMG1_RW	0x02107B02
#endif
#define	DMC_LP4_1200_DTMG2		0x6B365071
#if defined(VRCG_ON_EN)
#define	DMC_LP4_1200_DTMG3		0x9508181B
#else
#define	DMC_LP4_1200_DTMG3		0x950F181B
#endif
#define	DMC_LP4_1200_DTMG4		0x22630605
#define	DMC_LP4_1200_DTMG5		0x00000506
#define	DMC_LP4_1200_DTMG6		0x00000C0B
#define	DMC_LP4_1200_DTMG7		0x00780258	//drf_self_ref_idle_cnt[15:0] ddr_clk/2
#define	DMC_LP4_1200_DTMG8		0x22590000
#define	DMC_LP4_1200_DTMG9		0x00030C80
#define	DMC_LP4_1200_DTMG10		0x15120002
#if defined(VRCG_ON_EN)
#define	DMC_LP4_1200_DTMG11		0x00000000
#define	DMC_LP4_1200_DTMG12		0x00000000
#if LP4X_ODT_EN(6)
#define	DMC_LP4_1200_DTMG13		0x01340224
#define	DMC_LP4_1200_DTMG14		0x0e190b05
#else
#define	DMC_LP4_1200_DTMG13		0x01340224
#define	DMC_LP4_1200_DTMG14		0x0e5d0b00
#endif
#if LP4_ODT_EN(6)
#define	DMC_LP4_1200_DTMG13_RW	0x01340224
#define	DMC_LP4_1200_DTMG14_RW	0x0e190b05
#else
#define	DMC_LP4_1200_DTMG13_RW	0x0b050224
#define	DMC_LP4_1200_DTMG14_RW	0x0e5d0b00
#endif
#define	DMC_LP4_1200_DTMG15		0x00000000
#define	DMC_LP4_1200_DTMG16		0x00000000
#define	DMC_LP4_1200_DTMG17		0x00000000
#define	DMC_LP4_1200_DTMG18		0x00000000
#else //vrcg off
#if LP4X_ODT_EN(6)
#define	DMC_LP4_1200_DTMG11		0x0b050224
#define	DMC_LP4_1200_DTMG12		0x0d080e19
#else
#define	DMC_LP4_1200_DTMG11		0x0b050224
#define	DMC_LP4_1200_DTMG12		0x0d080e5d
#endif
#if LP4_ODT_EN(6)
#define	DMC_LP4_1200_DTMG11_RW	0x0b050224
#define	DMC_LP4_1200_DTMG12_RW	0x0d080e19
#else
#define	DMC_LP4_1200_DTMG11		0x0b050224
#define	DMC_LP4_1200_DTMG12		0x0d080e5d
#endif
#define	DMC_LP4_1200_DTMG13		0x01340134
#define	DMC_LP4_1200_DTMG14		0x01340134
#define	DMC_LP4_1200_DTMG15		0x01340134
#define	DMC_LP4_1200_DTMG16		0x01340134
#define	DMC_LP4_1200_DTMG17		0x01340d00
#define	DMC_LP4_1200_DTMG18		0x01340134
#endif
#define	DMC_LP4_1200_DTMG19		0xFFFFFFFF


#define DMC_LP4_933_MR1			0x24
#define DMC_LP4_933_MR2			0x1b
#define DMC_LP4_933_MR3			0x33
#define DMC_LP4_933_MR22		0x38
#define	DMC_LP4_933_DTMG0		0x460B0909
#if LP4X_ODT_EN(5)
#define	DMC_LP4_933_DTMG1		0x020D7B02
#else
#define	DMC_LP4_933_DTMG1		0x020D2B02
#endif
#if LP4_ODT_EN(5)
#define	DMC_LP4_933_DTMG1_RW	0x020D7B02
#else
#define	DMC_LP4_933_DTMG1_RW	0x020D2B02
#endif
#define	DMC_LP4_933_DTMG2		0x53294458
#if defined(VRCG_ON_EN)
#define	DMC_LP4_933_DTMG3		0x74061315
#else
#define	DMC_LP4_933_DTMG3		0x740D1315
#endif
#define	DMC_LP4_933_DTMG4		0x22530504
#define	DMC_LP4_933_DTMG5		0x00000405
#define	DMC_LP4_933_DTMG6		0x00000B0A
#define	DMC_LP4_933_DTMG7		0x005E01D2	//drf_self_ref_idle_cnt[15:0]
#define	DMC_LP4_933_DTMG8		0x11D40000
#define	DMC_LP4_933_DTMG9		0x00030C80
#define	DMC_LP4_933_DTMG10		0x150E0002
#if defined(VRCG_ON_EN)
#define	DMC_LP4_933_DTMG11		0x00000000
#define	DMC_LP4_933_DTMG12		0x00000000
#if LP4X_ODT_EN(5)
#define	DMC_LP4_933_DTMG13		0x0134021B
#define	DMC_LP4_933_DTMG14		0x0E190B05
#else
#define	DMC_LP4_933_DTMG13		0x0134021B
#define	DMC_LP4_933_DTMG14		0x0E5D0B00
#endif
#if LP4_ODT_EN(5)
#define	DMC_LP4_933_DTMG13_RW	0x0134021B
#define	DMC_LP4_933_DTMG14_RW	0x0E190B05
#else
#define	DMC_LP4_933_DTMG13_RW	0x0134021B
#define	DMC_LP4_933_DTMG14_RW	0x0E5D0B00
#endif
#define	DMC_LP4_933_DTMG15		0x00000000
#define	DMC_LP4_933_DTMG16		0x00000000
#define	DMC_LP4_933_DTMG17		0x00000000
#define	DMC_LP4_933_DTMG18		0x00000000
#else //vrcg off
#if LP4X_ODT_EN(5)
#define	DMC_LP4_933_DTMG11		0x0B05021B
#define	DMC_LP4_933_DTMG12		0x0D080E19
#else
#define	DMC_LP4_933_DTMG11		0x0B00021B
#define	DMC_LP4_933_DTMG12		0x0D080E5D
#endif
#if LP4_ODT_EN(5)
#define	DMC_LP4_933_DTMG11_RW	0x0B05021B
#define	DMC_LP4_933_DTMG12_RW	0x0D080E19
#else
#define	DMC_LP4_933_DTMG11_RW	0x0B00021B
#define	DMC_LP4_933_DTMG12_RW	0x0D080E5D
#endif
#define	DMC_LP4_933_DTMG13		0x01240124
#define	DMC_LP4_933_DTMG14		0x01240124
#define	DMC_LP4_933_DTMG15		0x01240124
#define	DMC_LP4_933_DTMG16		0x0D000124
#define	DMC_LP4_933_DTMG17		0x01240124
#define	DMC_LP4_933_DTMG18		0x01240124
#endif
#define	DMC_LP4_933_DTMG19		0xFFFFFFFF


#define DMC_LP4_768_MR1			0x24
#define DMC_LP4_768_MR2			0x12
#define DMC_LP4_768_MR3			0x33
#define DMC_LP4_768_MR22		0x38
#define	DMC_LP4_768_DTMG0		0x450A0808
#if	LP4X_ODT_EN(4)
#define	DMC_LP4_768_DTMG1		0x020B7802
#else
#define	DMC_LP4_768_DTMG1		0x020B2802
#endif
#if	LP4_ODT_EN(4)
#define	DMC_LP4_768_DTMG1_RW	0x020B7802
#else
#define	DMC_LP4_768_DTMG1_RW	0x020B2802
#endif
#define	DMC_LP4_768_DTMG2		0x45223848
#if defined(VRCG_ON_EN)
#define	DMC_LP4_768_DTMG3		0x63050F12
#else
#define	DMC_LP4_768_DTMG3		0x630B0F12
#endif
#define	DMC_LP4_768_DTMG4		0x22430403
#define	DMC_LP4_768_DTMG5		0x00000304
#define	DMC_LP4_768_DTMG6		0x00000807
#define	DMC_LP4_768_DTMG7		0x004D0180	//drf_self_ref_idle_cnt[15:0]
#define	DMC_LP4_768_DTMG8		0x11810000
#define	DMC_LP4_768_DTMG9		0x00030C80
#define	DMC_LP4_768_DTMG10		0x150C0002
#if defined(VRCG_ON_EN)
#define	DMC_LP4_768_DTMG11		0x00000000
#define	DMC_LP4_768_DTMG12		0x00000000
#if	LP4X_ODT_EN(4)
#define	DMC_LP4_768_DTMG13		0x01340212
#define	DMC_LP4_768_DTMG14		0x0E190B05
#else
#define	DMC_LP4_768_DTMG13		0x01340212
#define	DMC_LP4_768_DTMG14		0x0E5D0B00
#endif
#if	LP4_ODT_EN(4)
#define	DMC_LP4_768_DTMG13_RW	0x01340212
#define	DMC_LP4_768_DTMG14_RW	0x0E190B05
#else
#define	DMC_LP4_768_DTMG13_RW	0x01340212
#define	DMC_LP4_768_DTMG14_RW	0x0E5D0B00
#endif
#define	DMC_LP4_768_DTMG15		0x00000000
#define	DMC_LP4_768_DTMG16		0x00000000
#define	DMC_LP4_768_DTMG17		0x00000000
#define	DMC_LP4_768_DTMG18		0x00000000
#else //vrcg off
#if	LP4X_ODT_EN(4)
#define	DMC_LP4_768_DTMG11		0x0B050212
#define	DMC_LP4_768_DTMG12		0x0D080E19
#else
#define	DMC_LP4_768_DTMG11		0x0B000212
#define	DMC_LP4_768_DTMG12		0x0D080E5D
#endif
#if	LP4_ODT_EN(4)
#define	DMC_LP4_768_DTMG11_RW	0x0B050212
#define	DMC_LP4_768_DTMG12_RW	0x0D080E19
#else
#define	DMC_LP4_768_DTMG11_RW	0x0B000212
#define	DMC_LP4_768_DTMG12_RW	0x0D080E5D
#endif
#define	DMC_LP4_768_DTMG13		0x01240124
#define	DMC_LP4_768_DTMG14		0x01240124
#define	DMC_LP4_768_DTMG15		0x01240124
#define	DMC_LP4_768_DTMG16		0x0D000124
#define	DMC_LP4_768_DTMG17		0x01240124
#define	DMC_LP4_768_DTMG18		0x01240124
#endif
#define	DMC_LP4_768_DTMG19		0xFFFFFFFF


#define DMC_LP4_622_MR1			0x24
#define DMC_LP4_622_MR2			0x12
#define DMC_LP4_622_MR3			0x33
#define DMC_LP4_622_MR22		0x38
#define	DMC_LP4_622_DTMG0		0x44080707
#define	DMC_LP4_622_DTMG1		0x020A2802
#define	DMC_LP4_622_DTMG2		0x371B343B
#if defined(VRCG_ON_EN)
#define	DMC_LP4_622_DTMG3		0x53040C0F
#else
#define	DMC_LP4_622_DTMG3		0x53080C0F
#endif
#define	DMC_LP4_622_DTMG4		0x22430403
#define	DMC_LP4_622_DTMG5		0x00000304
#define	DMC_LP4_622_DTMG6		0x00000807
#define	DMC_LP4_622_DTMG7		0x003F0137	//drf_self_ref_idle_cnt[15:0]
#define	DMC_LP4_622_DTMG8		0x11380000
#define	DMC_LP4_622_DTMG9		0x00030C80
#define	DMC_LP4_622_DTMG10		0x150A0002
#if defined(VRCG_ON_EN)
#define	DMC_LP4_622_DTMG11		0x00000000
#define	DMC_LP4_622_DTMG12		0x00000000
#define	DMC_LP4_622_DTMG13		0x01340212
#define	DMC_LP4_622_DTMG14		0x0E5D0B00
#define	DMC_LP4_622_DTMG15		0x00000000
#define	DMC_LP4_622_DTMG16		0x00000000
#define	DMC_LP4_622_DTMG17		0x00000000
#define	DMC_LP4_622_DTMG18		0x00000000
#else //vrcg off
#define	DMC_LP4_622_DTMG11		0x0B000212
#define	DMC_LP4_622_DTMG12		0x0D080E5D
#define	DMC_LP4_622_DTMG13		0x01240124
#define	DMC_LP4_622_DTMG14		0x01240124
#define	DMC_LP4_622_DTMG15		0x01240124
#define	DMC_LP4_622_DTMG16		0x0D000124
#define	DMC_LP4_622_DTMG17		0x01240124
#define	DMC_LP4_622_DTMG18		0x01240124
#endif
#define	DMC_LP4_622_DTMG19		0xFFFFFFFF


#define DMC_LP4_512_MR1			0x14
#define DMC_LP4_512_MR2			0x09
#define DMC_LP4_512_MR3			0x33
#define DMC_LP4_512_MR22		0x38
#define	DMC_LP4_512_DTMG0		0x43060505
#define	DMC_LP4_512_DTMG1		0x02092602
#define	DMC_LP4_512_DTMG2		0x2E162C30
#if defined(VRCG_ON_EN)
#define	DMC_LP4_512_DTMG3		0x43040A0C
#else
#define	DMC_LP4_512_DTMG3		0x43060A0C
#endif
#define	DMC_LP4_512_DTMG4		0x22430403
#define	DMC_LP4_512_DTMG5		0x00000304
#define	DMC_LP4_512_DTMG6		0x00000606
#define	DMC_LP4_512_DTMG7		0x00340100	//drf_self_ref_idle_cnt[15:0]
#define	DMC_LP4_512_DTMG8		0x51010000
#define	DMC_LP4_512_DTMG9		0x00030C80
#define	DMC_LP4_512_DTMG10		0x15080002
#if defined(VRCG_ON_EN)
#define	DMC_LP4_512_DTMG11		0x00000000
#define	DMC_LP4_512_DTMG12		0x00000000
#define	DMC_LP4_512_DTMG13		0x01340209
#define	DMC_LP4_512_DTMG14		0x0e5d0b00
#define	DMC_LP4_512_DTMG15		0x00000000
#define	DMC_LP4_512_DTMG16		0x00000000
#define	DMC_LP4_512_DTMG17		0x00000000
#define	DMC_LP4_512_DTMG18		0x00000000
#else //vrcg off
#define	DMC_LP4_512_DTMG11		0x0B000209
#define	DMC_LP4_512_DTMG12		0x0D080E5D
#define	DMC_LP4_512_DTMG13		0x01140114
#define	DMC_LP4_512_DTMG14		0x01140114
#define	DMC_LP4_512_DTMG15		0x01140114
#define	DMC_LP4_512_DTMG16		0x0D000114
#define	DMC_LP4_512_DTMG17		0x01140114
#define	DMC_LP4_512_DTMG18		0x01140114
#endif
#define	DMC_LP4_512_DTMG19		0xFFFFFFFF


#define DMC_LP4_384_MR1			0x14
#define DMC_LP4_384_MR2			0x09
#define DMC_LP4_384_MR3			0x33
#define DMC_LP4_384_MR22		0x38
#define	DMC_LP4_384_DTMG0		0x42050404
#define	DMC_LP4_384_DTMG1		0x02092502
#define	DMC_LP4_384_DTMG2		0x22102424
#if defined(VRCG_ON_EN)
#define	DMC_LP4_384_DTMG3		0x33040809
#else
#define	DMC_LP4_384_DTMG3		0x33040809
#endif
#define	DMC_LP4_384_DTMG4		0x22430403
#define	DMC_LP4_384_DTMG5		0x00000304
#define	DMC_LP4_384_DTMG6		0x00000606
#define	DMC_LP4_384_DTMG7		0x002700C0	//drf_self_ref_idle_cnt[15:0]
#define	DMC_LP4_384_DTMG8		0x50C10000
#define	DMC_LP4_384_DTMG9		0x00030C80
#define	DMC_LP4_384_DTMG10		0x15060002
#if defined(VRCG_ON_EN)
#define	DMC_LP4_384_DTMG11		0x00000000
#define	DMC_LP4_384_DTMG12		0x00000000
#define	DMC_LP4_384_DTMG13		0x01340209
#define	DMC_LP4_384_DTMG14		0x0e5d0b00
#define	DMC_LP4_384_DTMG15		0x00000000
#define	DMC_LP4_384_DTMG16		0x00000000
#define	DMC_LP4_384_DTMG17		0x00000000
#define	DMC_LP4_384_DTMG18		0x00000000
#else //vrcg off
#define	DMC_LP4_384_DTMG11		0x0B000209
#define	DMC_LP4_384_DTMG12		0x0D080E5D
#define	DMC_LP4_384_DTMG13		0x01140114
#define	DMC_LP4_384_DTMG14		0x01140114
#define	DMC_LP4_384_DTMG15		0x01140114
#define	DMC_LP4_384_DTMG16		0x0D000114
#define	DMC_LP4_384_DTMG17		0x01140114
#define	DMC_LP4_384_DTMG18		0x01140114
#endif
#define	DMC_LP4_384_DTMG19		0xFFFFFFFF

#define DMC_LP4_256_MR1			0x04
#define DMC_LP4_256_MR2			0x00
#define DMC_LP4_256_MR3			0x33
#define DMC_LP4_256_MR22		0x38
#define	DMC_LP4_256_DTMG0		0x42040303
#define	DMC_LP4_256_DTMG1		0x02082402
#define	DMC_LP4_256_DTMG2		0x170B1C18
#if defined(VRCG_ON_EN)
#define	DMC_LP4_256_DTMG3		0x23040506
#else
#define	DMC_LP4_256_DTMG3		0x23040506
#endif
#define	DMC_LP4_256_DTMG4		0x22430302
#define	DMC_LP4_256_DTMG5		0x00000203
#define	DMC_LP4_256_DTMG6		0x00000404
#define	DMC_LP4_256_DTMG7		0x001A0080	//drf_self_ref_idle_cnt[15:0]
#define	DMC_LP4_256_DTMG8		0x50810000
#define	DMC_LP4_256_DTMG9		0x00030C80
#define	DMC_LP4_256_DTMG10		0x15040002
#if defined(VRCG_ON_EN)
#define	DMC_LP4_256_DTMG11		0x00000000
#define	DMC_LP4_256_DTMG12		0x00000000
#define	DMC_LP4_256_DTMG13		0x01340200
#define	DMC_LP4_256_DTMG14		0x0e5d0b00
#define	DMC_LP4_256_DTMG15		0x00000000
#define	DMC_LP4_256_DTMG16		0x00000000
#define	DMC_LP4_256_DTMG17		0x00000000
#define	DMC_LP4_256_DTMG18		0x00000000
#else //vrcg off
#define	DMC_LP4_256_DTMG11		0x0B000200
#define	DMC_LP4_256_DTMG12		0x0D080E5D
#define	DMC_LP4_256_DTMG13		0x01040104
#define	DMC_LP4_256_DTMG14		0x01040104
#define	DMC_LP4_256_DTMG15		0x01040104
#define	DMC_LP4_256_DTMG16		0x0D000104
#define	DMC_LP4_256_DTMG17		0x01040104
#define	DMC_LP4_256_DTMG18		0x01040104
#endif
#define	DMC_LP4_256_DTMG19		0xFFFFFFFF

