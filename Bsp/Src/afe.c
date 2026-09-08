#include "slm32x030_hal.h"
#include "bsp_hal.h"
#include "spi.h"
#include "afe.h"

/* SPI1 chip-select for the AFE (PA15). */
#define AFE_NSS_PORT GPIOA
#define AFE_NSS_PIN  GPIO_PIN_15

// afe spi
#define W_MODE_DATA_OFFSET_1     2
#define W_MODE_ADDRESS_OFFSET    4
#define W_MODE_READ_WRITE_OFFSET 3
#define W_MODE_DATA_OFFSET_2     6
#define W_MODE_DATA_Msk          0xc0

#define WRITE_MODE    1
#define READ_MODE     0
#define R_MODE_DATA   0x00
#define R_MODE_RX_Msk 0x04

#define R_MODE_RX1_Msk_1    0xFC
#define R_MODE_RX1_OFFSET_1 1
#define R_MODE_RX0_Msk_1    0x01
#define R_MODE_RX0_OFFSET_1 7

#define R_MODE_RX1_Msk_2    0xFE
#define R_MODE_RX1_OFFSET_2 2
#define R_MODE_RX0_Msk_2    0x03
#define R_MODE_RX0_OFFSET_2 6

typedef struct
{
    uint8_t *data;
    uint8_t  val;
    uint8_t  addr;
} AFE_RegCfgTypeDef;

static saturn_reg00_bits register00_saturn = {
    .wr_dac_data = DAC_OUTPUT_CODE,
};

static saturn_reg01_bits register01_saturn = {
    .wr_op0_pga_gain = AFE_PGA_GAIN,
    .wr_op1_pga_gain = AFE_PGA_GAIN,
    .wr_op2_pga_gain = AFE_PGA_GAIN,
    .wr_op3_pga_gain = AFE_PGA_GAIN,
};

static saturn_reg03_bits register03_saturn = {
    .wr_op0_pga_ref_sel    = PGA_REF_SEL_N,
    .wr_op1_pga_ref_sel    = PGA_REF_SEL_N,
    .wr_op2_pga_ref_sel    = PGA_REF_SEL_N,
    .wr_op3_pga_ref_sel    = PGA_REF_SEL_NULL,
    .wr_op0_pga_pag_in_sel = PGA_IN_SEL_P,
    .wr_op1_pga_pag_in_sel = PGA_IN_SEL_P,
    .wr_op2_pga_pag_in_sel = PGA_IN_SEL_P,
    .wr_op3_pga_pag_in_sel = PGA_IN_SEL_NULL,
};

static saturn_reg04_bits register04_saturn = {
    .wr_op0_pga_gain_en = PAG_INTERNAL_GAIN_USED,
    .wr_op1_pga_gain_en = PAG_INTERNAL_GAIN_USED,
    .wr_op2_pga_gain_en = PAG_INTERNAL_GAIN_USED,
    .wr_op3_pga_gain_en = PAG_INTERNAL_GAIN_USED,
    .wr_op0_pga_en      = PAG_INTERNAL_ENABLED,
    .wr_op1_pga_en      = PAG_INTERNAL_ENABLED,
    .wr_op2_pga_en      = PAG_INTERNAL_ENABLED,
    .wr_op3_pga_en      = PAG_INTERNAL_DISABLED,
};

static saturn_reg05_bits register05_saturn = {
    .wr_cmp2_hysteresis_option_sel = VC_HYS_SEL_20MV,
    .wr_cmp3_hysteresis_option_sel = VC_HYS_SEL_20MV,
    .wr_cmp4_hysteresis_option_sel = VC_HYS_SEL_20MV,
    .wr_cmp5_hysteresis_option_sel = VC_HYS_SEL_20MV,
};

static saturn_reg06_bits register06_saturn = {
    .wr_dac_sel                    = DAC_SEL_5V,
    .wr_bemfx_con                  = BEMFX_CON_DISABLED,
    .wr_pga_refer_sel              = OP_REF_VOLT_AVDD50_2,
    .wr_cmp0_hysteresis_option_sel = VC_HYS_SEL_20MV,
    .wr_cmp1_hysteresis_option_sel = VC_HYS_SEL_20MV,
};

static saturn_reg07_bits register07_saturn = {
    .wr_cmp2_vc_selp = VC_SELP_CMPXP,
    .wr_cmp3_vc_selp = VC_SELP_NULL,
    .wr_cmp4_vc_selp = VC_SELP_NULL,
    .wr_cmp5_vc_selp = VC_SELP_NULL,
};

static saturn_reg08_bits register08_saturn = {
    .wr_dac_s_oa_en   = DAC_S_OA_OPEN,
    .wr_dac_s_dacu_en = DAC_S_DACU_OPEN,
    .wr_dac_en_oa     = DAC_EN_OA_ENABLED,
    .wr_dac_drive_en  = DAC_DRIVE_STRONG,
    .wr_cmp0_vc_selp  = VC_SELP_CMPXP,
    .wr_cmp1_vc_selp  = VC_SELP_CMPXP,
};

static saturn_reg09_bits register09_saturn = {
    .wr_cmp2_vc_seln = VC_SELN_DAC_OUT,
    .wr_cmp3_vc_seln = VC_SELP_NULL,
    .wr_cmp4_vc_seln = VC_SELP_NULL,
    .wr_cmp5_vc_seln = VC_SELP_NULL,
};

static saturn_reg10_bits register10_saturn = {
    .wr_cmp2_vc_dly  = VC_DLY_FAST_SPPED_MODE,
    .wr_cmp3_vc_dly  = VC_DLY_FAST_SPPED_MODE,
    .wr_cmp4_vc_dly  = VC_DLY_FAST_SPPED_MODE,
    .wr_cmp5_vc_dly  = VC_DLY_FAST_SPPED_MODE,
    .wr_cmp0_vc_seln = VC_SELN_DAC_OUT,
    .wr_cmp1_vc_seln = VC_SELN_DAC_OUT,
};

static saturn_reg11_bits register11_saturn = {
    .wr_cmp0_vc_en  = VC_EN_DISABLED,
    .wr_cmp1_vc_en  = VC_EN_ENABLED,
    .wr_cmp2_vc_en  = VC_EN_DISABLED,
    .wr_cmp3_vc_en  = VC_EN_DISABLED,
    .wr_cmp4_vc_en  = VC_EN_DISABLED,
    .wr_cmp5_vc_en  = VC_EN_DISABLED,
    .wr_cmp0_vc_dly = VC_DLY_FAST_SPPED_MODE,
    .wr_cmp1_vc_dly = VC_DLY_FAST_SPPED_MODE,
};

static AFE_RegCfgTypeDef g_afe_cfg_tbl[] = {
    {(uint8_t *)&register07_saturn, 0, SATURN_REG07}, {(uint8_t *)&register08_saturn, 0, SATURN_REG08},
    {(uint8_t *)&register09_saturn, 0, SATURN_REG09}, {(uint8_t *)&register10_saturn, 0, SATURN_REG10},
    {(uint8_t *)&register05_saturn, 0, SATURN_REG05}, {(uint8_t *)&register06_saturn, 0, SATURN_REG06},
    {(uint8_t *)&register11_saturn, 0, SATURN_REG11}, {(uint8_t *)&register04_saturn, 0, SATURN_REG04},
    {(uint8_t *)&register03_saturn, 0, SATURN_REG03}, {(uint8_t *)&register01_saturn, 0, SATURN_REG01},
    {(uint8_t *)&register00_saturn, 0, SATURN_REG00},
};

void afe_register_rw(uint8_t *data, uint8_t address, uint8_t opt_type)
{
    /* cs disable, need send 8 bit,to notify slave sync*/
    uint8_t aTxBuffer[2] = {0, 0};
    HAL_SPI_Transmit(&g_spi1_handle, aTxBuffer, 1, 1500);

    /* cs enable, send data begin*/
    HAL_GPIO_WritePin(AFE_NSS_PORT, AFE_NSS_PIN, GPIO_PIN_RESET);

    if (opt_type == READ_MODE)
    {
        uint8_t aRxBuffer[2] = {0, 0};
        aTxBuffer[1]         = R_MODE_DATA;
        aTxBuffer[0]         = address << W_MODE_ADDRESS_OFFSET | READ_MODE << W_MODE_READ_WRITE_OFFSET;

        HAL_SPI_TransmitReceive(&g_spi1_handle, aTxBuffer, aRxBuffer, 2, 1500);
        while (HAL_SPI_GetState(&g_spi1_handle) != HAL_SPI_STATE_READY) {}

        if ((aRxBuffer[0] & R_MODE_RX_Msk) == 0)
        {
            data[0] = (aRxBuffer[0] & R_MODE_RX0_Msk_1) << R_MODE_RX0_OFFSET_1 | (aRxBuffer[1] & R_MODE_RX1_Msk_2) >> 1;
        }
        else
        {
            data[0] = (aRxBuffer[0] & R_MODE_RX0_Msk_2) << R_MODE_RX0_OFFSET_2 | (aRxBuffer[1] & R_MODE_RX1_Msk_1) >> 2;
        }
    }
    else
    {
        aTxBuffer[1] = *data << W_MODE_DATA_OFFSET_1;
        aTxBuffer[0] = address << W_MODE_ADDRESS_OFFSET | WRITE_MODE << W_MODE_READ_WRITE_OFFSET | (*data & W_MODE_DATA_Msk) >> W_MODE_DATA_OFFSET_2;
        HAL_SPI_Transmit(&g_spi1_handle, aTxBuffer, 2, 1500);

        while (HAL_SPI_GetState(&g_spi1_handle) != HAL_SPI_STATE_READY) {}
    }

    HAL_GPIO_WritePin(AFE_NSS_PORT, AFE_NSS_PIN, GPIO_PIN_SET);
}

void afe_register_r(uint8_t *val, uint8_t addr)
{
    afe_register_rw(val, addr, READ_MODE);
}

void afe_register_w(uint8_t val, uint8_t addr)
{
    uint8_t register_val = val;

    afe_register_rw(&register_val, addr, WRITE_MODE);
}

void afe_init(void)
{
    for (uint8_t index = 0; index < (sizeof(g_afe_cfg_tbl) / sizeof(AFE_RegCfgTypeDef)); index++)
    {
        g_afe_cfg_tbl[index].val = *(uint8_t *)(g_afe_cfg_tbl[index].data);
        afe_register_w(g_afe_cfg_tbl[index].val, g_afe_cfg_tbl[index].addr);
    }
}
