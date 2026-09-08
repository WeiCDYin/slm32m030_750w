/*****************************************************************************************************
**                                                                                                  **
** Copyright (C) Shanghai SoulSemi Technologies (2024)                                              **
**                                                                                                  **
** all rights reserved.                                                                             **
**                                                                                                  **
** This document contains proprietary information belonging to SoulSemi Technologies. Passing on    **
** and copying of this document, and communication of its contents is not permitted without prior   **
** written authorization.                                                                           **
**                                                                                                  **
******************************************************************************************************
**                                                                                                  **
**  @file       : saturn_ll_reg.h                                                         **
**                                                                                                  **
**  @version    : hal_app_1.0.0                                                                     **
**                                                                                                  **
**  @date       : 2024-11-30                                                                     **
**                                                                                                  **
**  @author     : MCU Software Team                                                                 **
**                                                                                                  **
**  @vendor     : Shanghai SoulSemi Technologies                                                    **
**                                                                                                  **
**  @brief      : hal sdk example - saturn_ll_reg api.                                    **
**                                                                                                  **
**  @details    : This file contains                                                                **
**                 - functionality of saturn_ll_reg.                                      **
**                                                                                                  **
** @addtogroup  : saturn_ll_reg                                                              **
**                                                                                                  **
**  MAY BE CHANGED BY USER [Yes/No]: Yes                                                            **
**                                                                                                  **
*****************************************************************************************************/
#ifndef _SATURN_LL_REG_H_
#define _SATURN_LL_REG_H_

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************************************
|    Other Header File Inclusion
*****************************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
/*****************************************************************************************************
|    Compile Option or configuration Section (for test/debug)
*****************************************************************************************************/

/*****************************************************************************************************
|    Macro Definition
*****************************************************************************************************/
#define SATURN_REG00 0x00U
#define SATURN_REG01 0x01U
#define SATURN_REG02 0x02U
#define SATURN_REG03 0x03U
#define SATURN_REG04 0x04U
#define SATURN_REG05 0x05U
#define SATURN_REG06 0x06U
#define SATURN_REG07 0x07U
#define SATURN_REG08 0x08U
#define SATURN_REG09 0x09U
#define SATURN_REG10 0x0AU
#define SATURN_REG11 0x0BU
/*****************************************************************************************************
|    Enum Definition
*****************************************************************************************************/
typedef enum {
    PGA_GAIN_4 = 0,
    PGA_GAIN_8,
    PGA_GAIN_16,
    PGA_GAIN_32,
} PAG_GAIN_SEL_E;
typedef enum {
    PGA_IN_SEL_P = 0, /*OP正端连接引脚OPxP*/
    PGA_IN_SEL_NULL,  /*OP正端内部悬空*/
} PGA_IN_SEL_E;
typedef enum {
    PGA_REF_SEL_N = 0, /*OP负端连接引脚OPxP*/
    PGA_REF_SEL_NULL,  /*OP负端内部悬空*/
} PGA_REF_SEL_E;

typedef enum {
    PAG_INTERNAL_ENABLED = 0,
    PAG_INTERNAL_DISABLED,
} PGA_EN_E;
typedef enum {
    PAG_INTERNAL_GAIN_USED = 0,
    PAG_EXTERNAL_GAIN_USED,
} PGA_GAIN_INTERNAL_EN_E;

typedef enum /*Hysteresis option select of VC:*/
{ VC_HYS_SEL_20MV = 0,
  VC_HYS_SEL_40MV,
  VC_HYS_SEL_80MV,
  VC_HYS_SEL_100MV,
} VC_HYS_SEL_E;
typedef enum {
    OP_REF_VOLT_1_9V = 0, /*共模电平选择1.9V*/
    OP_REF_VOLT_AVDD50_2,
    OP_REF_VOLT_0_6V,
    OP_REF_VOLT_1_2V,
} OP_REF_VOLT_E;

typedef enum {
    BEMFX_CON_DISABLED = 0,
    BEMFX_CON_ENABLED,
} BEMFX_CON_E;
typedef enum {
    DAC_SEL_5V = 0,
    DAC_SEL_1_2V,
} DAC_SEL_E;
typedef enum {
    VC_SELP_OPXP = 0,
    VC_SELP_OPXO,
    VC_SELP_CMPXP,
    VC_SELP_NULL,
} VC_SELP_SEL_E;
typedef enum {
    DAC_DRIVE_STRONG = 0,
    DAC_DRIVE_WEAK,
} DAC_DRIVE_E;
typedef enum {
    DAC_EN_OA_ENABLED = 0,
    DAC_EN_OA_DISABLED,
} DAC_EN_OA_E;
typedef enum {
    DAC_S_DACU_OPEN = 0,
    DAC_S_DACU_CLOSE,
} DAC_S_DACU_E;
typedef enum {
    DAC_S_OA_CLOSE = 0,
    DAC_S_OA_OPEN,
} DAC_S_OA_E;
typedef enum {
    VC_SELN_1_2V = 0,
    VC_SELN_DAC_OUT,
    VC_SELN_BRME_MID,
    VC_SELN_CMPXN,
} VC_SELN_SELECT_E;
typedef enum {
    VC_DLY_SLOW_SPPED_MODE = 0,
    VC_DLY_FAST_SPPED_MODE,
} VC_DLY_SEL_E;
typedef enum {
    VC_EN_DISABLED = 0,
    VC_EN_ENABLED,
} VC_EN_CFG_E;

/*****************************************************************************************************
|    Typedef Definition
*****************************************************************************************************/
typedef struct {
    uint8_t wr_linphy_aa_opa_sw : 1;
    uint8_t wr_linphy_aa_opa_en : 1;
    uint8_t wr_linphy_ipu_sel : 1;
    uint8_t wr_linphy_ipu_en : 1;
    uint8_t wr_linphy_oc_en : 1;
    uint8_t wr_linphy_pu_sel : 1;
    uint8_t reserved : 2;

} st_saturn09_reg00_bits;

/* SATURN09 register_01 */
typedef struct {
    uint8_t wr_pga_vc_sel : 1;
    uint8_t wr_pga_gain1 : 4;
    uint8_t reserved : 1;
    uint8_t linphy_slr : 2;

} st_saturn09_reg01_bits;

/* SATURN09 register_02 */
typedef struct {
    uint8_t wr_f303_init_set : 4;   /*bit 3~0*/
    uint8_t ro_f030_init_state : 1; /*bit 4*/
    uint8_t ro_linphy_state : 1;    /*bit 5*/
    uint8_t ro_linphy_rx_state : 1; /*bit 6*/
    uint8_t ro_linphy_tx_state : 1; /*bit 7*/

} st_saturn09_reg02_bits;

/* SATURN09 register_03 */
typedef struct {
    uint8_t wr_bg_trim_tt : 8;

} st_saturn09_reg03_bits;

/* SATURN09 register_04 */
typedef struct {
    uint8_t wr_bg_trim_vt : 8;

} st_saturn09_reg04_bits;

/* SATURN09 register_05 */
typedef struct {
    uint8_t wr_bg_trim : 8;

} st_saturn09_reg05_bits;

/* SATURN09 register_06 */
typedef struct {
    uint8_t wr_rgb_trim : 7;
    uint8_t reserved : 1;

} st_saturn09_reg06_bits;

/* SATURN09 register_07 */
typedef struct {
    uint8_t wr_rgb_b_outcurrent_b8 : 1;
    uint8_t wr_rgb_g_outcurrent_b8 : 1;
    uint8_t wr_rgb_r_outcurrent_b8 : 1;
    uint8_t reserved : 5;

} st_saturn09_reg07_bits;

/* SATURN09 register_08 */
typedef struct {
    uint8_t wr_rgb_r_outcurrent_b0_7 : 8;

} st_saturn09_reg08_bits;

/* SATURN09 register_09 */
typedef struct {
    uint8_t wr_rgb_g_outcurrent_b0_7 : 8;

} st_saturn09_reg09_bits;

/* SATURN09 register_10 */
typedef struct {
    uint8_t wr_rgb_b_outcurrent_b0_7 : 8;

} st_saturn09_reg10_bits;

/* SATURN09 register_11 */
typedef struct {
    uint8_t wr_rgb_sr_b : 2;
    uint8_t wr_rgb_sr_g : 2;
    uint8_t wr_rgb_sr_r : 2;
    uint8_t reserved : 2;

} st_saturn09_reg11_bits;

/* SATURN09 register_12 */
typedef struct {
    uint8_t wr_linphy_en : 1;
    uint8_t wr_linphy_rx_en : 1;
    uint8_t wr_linphy_tx_en : 1;
    uint8_t ro_ch_sel2_state : 1;
    uint8_t ro_pga_state : 1;
    uint8_t ro_rgb_b_state : 1;
    uint8_t ro_rgb_g_state : 1;
    uint8_t ro_rgb_r_state : 1;

} st_saturn09_reg12_bits;

/* SATURN09 register_13 */
typedef struct {
    uint8_t wr_pga_ch_sel0 : 1;
    uint8_t wr_pga_ch_sel1 : 1;
    uint8_t wr_pga_ch_sel2 : 1;
    uint8_t wr_pga_en : 1;
    uint8_t wr_rgb_b_en : 1;
    uint8_t wr_rgb_g_en : 1;
    uint8_t wr_rgb_r_en : 1;
    uint8_t reserved : 1;

} st_saturn09_reg13_bits;

/* SATURN09 register_14 */
typedef struct {
    uint8_t wr_test_mode_en : 4; /*0x06 --into test mode, 0x0c -- out from test mode*/
    uint8_t wr_sio_o_sel : 1;
    uint8_t ro_test_mode_state : 1;
    uint8_t ro_ch_sel0_state : 1;
    uint8_t ro_ch_sel1_state : 1;

} st_saturn09_reg14_bits;

typedef struct {
    uint8_t wr_dac_data : 8; /*input data*/
} saturn_reg00_bits;

typedef struct {
    uint8_t wr_op0_pga_gain : 2; /*00 to 11 to select Gain APGA of 10x, 20x, 40x, 80x.*/
    uint8_t wr_op1_pga_gain : 2;
    uint8_t wr_op2_pga_gain : 2;
    uint8_t wr_op3_pga_gain : 2;
} saturn_reg01_bits;

typedef struct {
    uint8_t wr_f030_init_done : 4; /*f030_init_done：写0x9代表030初始化完成，*/
    uint8_t ro_f030_init_done : 1; /*wr_f030_init_done*/
    uint8_t reserved : 3;          /*reserved*/
} saturn_reg02_bits;

typedef struct {
    uint8_t wr_op0_pga_ref_sel : 1; /*PGA REF voltage selection.*/
    uint8_t wr_op1_pga_ref_sel : 1;
    uint8_t wr_op2_pga_ref_sel : 1;
    uint8_t wr_op3_pga_ref_sel : 1;
    uint8_t wr_op0_pga_pag_in_sel : 1; /*PGA input voltage selection.*/
    uint8_t wr_op1_pga_pag_in_sel : 1;
    uint8_t wr_op2_pga_pag_in_sel : 1;
    uint8_t wr_op3_pga_pag_in_sel : 1;
} saturn_reg03_bits;

typedef struct {
    uint8_t wr_op0_pga_gain_en : 1; /*pag 放大倍数使能.*/
    uint8_t wr_op1_pga_gain_en : 1;
    uint8_t wr_op2_pga_gain_en : 1;
    uint8_t wr_op3_pga_gain_en : 1;
    uint8_t wr_op0_pga_en : 1; /*PGA使能.*/
    uint8_t wr_op1_pga_en : 1;
    uint8_t wr_op2_pga_en : 1;
    uint8_t wr_op3_pga_en : 1;
} saturn_reg04_bits;

typedef struct {
    uint8_t wr_cmp2_hysteresis_option_sel : 2; /*Hysteresis option select of VC:*/
    uint8_t wr_cmp3_hysteresis_option_sel : 2;
    uint8_t wr_cmp4_hysteresis_option_sel : 2;
    uint8_t wr_cmp5_hysteresis_option_sel : 2;
} saturn_reg05_bits;
typedef struct {
    uint8_t wr_dac_sel : 1;       /*DAC输出电压范围选择*/
    uint8_t wr_bemfx_con : 1;     /*MID 过零检测开关*/
    uint8_t wr_pga_refer_sel : 2; /*共模电平选择*/
    uint8_t wr_cmp0_hysteresis_option_sel : 2;
    uint8_t wr_cmp1_hysteresis_option_sel : 2; /*Hysteresis option select of VC:*/
} saturn_reg06_bits;

typedef struct {
    uint8_t wr_cmp2_vc_selp : 2;
    uint8_t wr_cmp3_vc_selp : 2;
    uint8_t wr_cmp4_vc_selp : 2;
    uint8_t wr_cmp5_vc_selp : 2;
} saturn_reg07_bits;
typedef struct {
    uint8_t wr_dac_s_oa_en : 1;
    uint8_t wr_dac_s_dacu_en : 1;
    uint8_t wr_dac_en_oa : 1;
    uint8_t wr_dac_drive_en : 1;
    uint8_t wr_cmp0_vc_selp : 2;
    uint8_t wr_cmp1_vc_selp : 2;
} saturn_reg08_bits;

typedef struct {
    uint8_t wr_cmp2_vc_seln : 2;
    uint8_t wr_cmp3_vc_seln : 2;
    uint8_t wr_cmp4_vc_seln : 2;
    uint8_t wr_cmp5_vc_seln : 2;
} saturn_reg09_bits;

typedef struct {
    uint8_t wr_cmp2_vc_dly : 1;
    uint8_t wr_cmp3_vc_dly : 1;
    uint8_t wr_cmp4_vc_dly : 1;
    uint8_t wr_cmp5_vc_dly : 1;
    uint8_t wr_cmp0_vc_seln : 2;
    uint8_t wr_cmp1_vc_seln : 2;
} saturn_reg10_bits;

typedef struct {
    uint8_t wr_cmp0_vc_en : 1;
    uint8_t wr_cmp1_vc_en : 1;
    uint8_t wr_cmp2_vc_en : 1;
    uint8_t wr_cmp3_vc_en : 1;
    uint8_t wr_cmp4_vc_en : 1;
    uint8_t wr_cmp5_vc_en : 1;
    uint8_t wr_cmp0_vc_dly : 1;
    uint8_t wr_cmp1_vc_dly : 1;
} saturn_reg11_bits;

typedef union {
    saturn_reg09_bits reg_bit;
    uint8_t           reg;
} saturn_reg09_u;

typedef union {
    saturn_reg04_bits reg_bit;
    uint8_t           reg;
} saturn_reg04_u;
/*****************************************************************************************************
|    Global variables Declaration
*****************************************************************************************************/

/*****************************************************************************************************
|    Global Function Declaration
*****************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ANALOG_LL_REG_H_ */
