/**
  ******************************************************************************
  * @file    slm32x030_hal_flash_ex.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   Header file of Flash HAL Extended module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_FLASH_EX_H_
#define _SLM32X030_HAL_FLASH_EX_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup FLASHEx
  * @{
  */ 

/** @addtogroup FLASHEx_Private_Constants
  * @{
  */

#define FLASH_SIZE_DATA_REGISTER     (0x1FFFF7E0U)
#define OBR_REG_INDEX                (1U)
#define SR_FLAG_MASK                 ((uint32_t)(FLASH_SR_BSY | FLASH_SR_PGERR | FLASH_SR_WRPRTERR | FLASH_SR_EOP))

/**
  * @}
  */  

/** @addtogroup FLASHEx_Private_Macros
  * @{
  */

#define IS_FLASH_TYPEERASE(VALUE)   (((VALUE) == FLASH_TYPEERASE_PAGES) || ((VALUE) == FLASH_TYPEERASE_MASSERASE))

#define IS_OPTIONBYTE(VALUE)        (((VALUE) <= (OPTIONBYTE_WRP | OPTIONBYTE_RDP | OPTIONBYTE_USER | OPTIONBYTE_DATA)))

#define IS_WRPSTATE(VALUE)          (((VALUE) == OB_WRPSTATE_DISABLE) || ((VALUE) == OB_WRPSTATE_ENABLE))

#define IS_OB_RDP_LEVEL(LEVEL)      (((LEVEL) == OB_RDP_LEVEL_0) || ((LEVEL) == OB_RDP_LEVEL_1)|| ((LEVEL) == OB_RDP_LEVEL_2))

#define IS_OB_DATA_ADDRESS(ADDRESS) (((ADDRESS) == OB_DATA_ADDRESS_DATA0) || ((ADDRESS) == OB_DATA_ADDRESS_DATA1)) 

#define IS_OB_IWDG_SOURCE(SOURCE)   (((SOURCE) == OB_IWDG_SW) || ((SOURCE) == OB_IWDG_HW))

#define IS_OB_STOP_SOURCE(SOURCE)   (((SOURCE) == OB_STOP_NO_RST) || ((SOURCE) == OB_STOP_RST))

#define IS_OB_STDBY_SOURCE(SOURCE)  (((SOURCE) == OB_STDBY_NO_RST) || ((SOURCE) == OB_STDBY_RST))



/*Currently used according to this*/
#define IS_FLASH_NB_PAGES(ADDRESS,NBPAGES)  ((ADDRESS)+((NBPAGES)*FLASH_PAGE_SIZE)-1 <= 0x0803FFFFU)

#define IS_OB_WRP(PAGE)                     (((PAGE) != 0x0000000U))

#define IS_FLASH_BANK(BANK) (((BANK) == FLASH_BANK_1))

#define IS_FLASH_PROGRAM_ADDRESS(ADDRESS)   (((ADDRESS) >= FLASH_BASE) && (((*((uint16_t *)FLASH_SIZE_DATA_REGISTER)) == 0x80U) ? \
                                              ((ADDRESS) <= FLASH_BANK1_END) : (((*((uint16_t *)FLASH_SIZE_DATA_REGISTER)) == 0x40U) ? \
                                              ((ADDRESS) <= 0x0800FFFF) : (((*((uint16_t *)FLASH_SIZE_DATA_REGISTER)) == 0x20U) ? \
                                              ((ADDRESS) <= 0x08007FFF) : ((ADDRESS) <= 0x08003FFFU)))))

/**
  * @}
  */  

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup FLASHEx_Exported_Types FLASHEx Exported Types
  * @{
  */  

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
    uint32_t TypeErase;   /*!< TypeErase: Mass erase or page erase. This parameter can be a value of @ref FLASHEx_Type_Erase */

    uint32_t Banks;       /*!< Select banks to erase when Mass erase is enabled. This parameter must be a value of @ref FLASHEx_Banks */    

    uint32_t PageAddress; /*!< PageAdress: Initial FLASH page address to erase when mass erase is disabled.
                                This parameter must be a number between Min_Data = 0x08000000 and Max_Data = FLASH_BANKx_END 
                                (x = 1 or 2 depending on devices)*/

    uint32_t NbPages;     /*!< NbPages: Number of pagess to be erased.
                                This parameter must be a value between Min_Data = 1 and Max_Data = (max number of pages - value of initial page)*/                                                          
} FLASH_EraseInitTypeDef;

/**
  * @brief  FLASH Options bytes program structure definition
  */
typedef struct
{
    uint32_t OptionType;  /*!< OptionType: Option byte to be configured. This parameter can be a value of @ref FLASHEx_OB_Type */
    uint32_t WRPState;    /*!< WRPState: Write protection activation or deactivation. This parameter can be a value of @ref FLASHEx_OB_WRP_State */
    uint32_t WRPPage;     /*!< WRPPage: specifies the page(s) to be write protected. This parameter can be a value of @ref FLASHEx_OB_Write_Protection */
    uint32_t Banks;        /*!< Select banks for WRP activation/deactivation of all sectors. This parameter must be a value of @ref FLASHEx_Banks */ 
    uint8_t RDPLevel;     /*!< RDPLevel: Set the read protection level. This parameter can be a value of @ref FLASHEx_OB_Read_Protection */
    uint8_t USERConfig;   /*!< USERConfig: Program the FLASH app Option Byte: 
                                IWDG / STOP / STDBY
                                This parameter can be a combination of @ref FLASHEx_OB_IWatchdog, @ref FLASHEx_OB_nRST_STOP, 
                                @ref FLASHEx_OB_nRST_STDBY */
    uint32_t DATAAddress; /*!< DATAAddress: Address of the option byte DATA to be programmed
                                This parameter can be a value of @ref FLASHEx_OB_Data_Address */
    uint8_t DATAData;     /*!< DATAData: Data to be stored in the option byte DATA. This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF */
} FLASH_OBProgramInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Constants FLASHEx Exported Constants
  * @{
  */  

/** @defgroup FLASHEx_Constants FLASH Constants
  * @{
  */ 

/** @defgroup FLASHEx_Page_Size Page Size
  * @{
  */ 

#define FLASH_PAGE_SIZE          (0x400U)

#define FLASH_OBR_USER           (0x7UL << FLASH_POSITION_WDGSW_BIT)       /*!< 0x0000003C */                 /*!< app Option Bytes */

/**
  * @}
  */

/** @defgroup FLASHEx_Type_Erase Type Erase
  * @{
  */ 
#define FLASH_TYPEERASE_PAGES     (0x00U)  /*!<Pages erase only*/
#define FLASH_TYPEERASE_MASSERASE (0x02U)  /*!<Flash mass erase activation*/

/**
  * @}
  */

/** @defgroup FLASHEx_Banks Banks
  * @{
  */
#if defined(FLASH_BANK2_END)
    #define FLASH_BANK_1            (1U) /*!< Bank 1   */
    #define FLASH_BANK_2            (2U) /*!< Bank 2   */
    #define FLASH_BANK_BOTH         ((uint32_t)FLASH_BANK_1 | FLASH_BANK_2) /*!< Bank1 and Bank2  */
#else
    #define FLASH_BANK_1            (1U) /*!< Bank 1   */
#endif
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup FLASHEx_OptionByte_Constants Option Byte Constants
  * @{
  */ 

/** @defgroup FLASHEx_OB_Type Option Bytes Type
  * @{
  */
#define OPTIONBYTE_WRP              (0x01U)  /*!<WRP option byte configuration*/
#define OPTIONBYTE_RDP              (0x02U)  /*!<RDP option byte configuration*/
#define OPTIONBYTE_USER             (0x04U)  /*!<USER option byte configuration*/
#define OPTIONBYTE_DATA             (0x08U)  /*!<DATA option byte configuration*/

/**
  * @}
  */

/** @defgroup FLASHEx_OB_WRP_State Option Byte WRP State
  * @{
  */ 
#define OB_WRPSTATE_DISABLE         (0x00U)  /*!<Disable the write protection of the desired pages*/
#define OB_WRPSTATE_ENABLE          (0x01U)  /*!<Enable the write protection of the desired pagess*/

/**
  * @}
  */

/** @defgroup FLASHEx_OB_Write_Protection Option Bytes Write Protection
  * @{
  */

#define OB_WRP_PAGES0TO8            (0x00000001U) /*!< Write protection of page 0 to 7 */
#define OB_WRP_PAGES8TO15           (0x00000002U) /*!< Write protection of page 8 to 15 */
#define OB_WRP_PAGES16TO23          (0x00000004U) /*!< Write protection of page 16 to 23 */
#define OB_WRP_PAGES24TO31          (0x00000008U) /*!< Write protection of page 24 to 31 */
#define OB_WRP_PAGES32TO39          (0x00000010U) /*!< Write protection of page 32 to 39 */
#define OB_WRP_PAGES40TO47          (0x00000020U) /*!< Write protection of page 40 to 47 */
#define OB_WRP_PAGES48TO55          (0x00000040U) /*!< Write protection of page 48 to 55 */
#define OB_WRP_PAGES56TO63          (0x00000080U) /*!< Write protection of page 56 to 63 */


#define OB_WRP_PAGES64TO71          (0x00000100U)   /*!< Write protection of page 64 to 71 */
#define OB_WRP_PAGES72TO79          (0x00000200U)   /*!< Write protection of page 72 to 79 */
#define OB_WRP_PAGES80TO87          (0x00000400U)   /*!< Write protection of page 80 to 87 */
#define OB_WRP_PAGES88TO95          (0x00000800U)   /*!< Write protection of page 88 to 95 */
#define OB_WRP_PAGES96TO103         (0x00001000U)   /*!< Write protection of page 96 to 103 */
#define OB_WRP_PAGES104TO111        (0x00002000U)   /*!< Write protection of page 104 to 111 */
#define OB_WRP_PAGES112TO119        (0x00004000U)   /*!< Write protection of page 112 to 119 */
#define OB_WRP_PAGES120TO127        (0x00008000U)   /*!< Write protection of page 120 to 127 */
#define OB_WRP_PAGES128TO135        (0x00010000U)   /*!< Write protection of page 128 to 135 */
#define OB_WRP_PAGES136TO143        (0x00020000U)   /*!< Write protection of page 136 to 143 */
#define OB_WRP_PAGES144TO151        (0x00040000U)   /*!< Write protection of page 144 to 151 */
#define OB_WRP_PAGES152TO159        (0x00080000U)   /*!< Write protection of page 152 to 159 */
#define OB_WRP_PAGES160TO167        (0x00100000U)   /*!< Write protection of page 160 to 167 */
#define OB_WRP_PAGES168TO175        (0x00200000U)   /*!< Write protection of page 168 to 175 */
#define OB_WRP_PAGES176TO183        (0x00400000U)   /*!< Write protection of page 176 to 183 */
#define OB_WRP_PAGES184TO191        (0x00800000U)   /*!< Write protection of page 184 to 191 */
#define OB_WRP_PAGES192TO199        (0x01000000U)   /*!< Write protection of page 192 to 199 */
#define OB_WRP_PAGES200TO207        (0x02000000U)   /*!< Write protection of page 200 to 207 */
#define OB_WRP_PAGES208TO215        (0x04000000U)  /*!< Write protection of page 208 to 215 */
#define OB_WRP_PAGES216TO223        (0x08000000U)   /*!< Write protection of page 216 to 223 */
#define OB_WRP_PAGES224TO231        (0x10000000U)   /*!< Write protection of page 224 to 231 */
#define OB_WRP_PAGES232TO239        (0x20000000U)   /*!< Write protection of page 232 to 239 */
#define OB_WRP_PAGES240TO247        (0x40000000U)   /*!< Write protection of page 240 to 247 */
#define OB_WRP_PAGES248TO255        (0x80000000U)    /*!< Write protection of page 248 to 255 */

#define OB_WRP_ALLPAGES             (0xFFFFFFFFU)  /*!< Write protection of all Pages */
 
#define OB_WRP_PAGES0TO79MASK       (OB_WRP_PAGES0TO8 | OB_WRP_PAGES8TO15 | OB_WRP_PAGES16TO23 | OB_WRP_PAGES24TO31 | OB_WRP_PAGES40TO47 |  \
                                     OB_WRP_PAGES48TO55 | OB_WRP_PAGES56TO63 | OB_WRP_PAGES64TO71  | OB_WRP_PAGES72TO79)
#define OB_WRP_PAGES32TO63MASK      (OB_WRP_PAGES32TO39 | OB_WRP_PAGES40TO47 | OB_WRP_PAGES48TO55 | OB_WRP_PAGES56TO63)
#define OB_WRP_PAGES64TO95MASK      (OB_WRP_PAGES64TO71 | OB_WRP_PAGES72TO79 | OB_WRP_PAGES80TO87 | OB_WRP_PAGES88TO95)
#define OB_WRP_PAGES96TO127MASK     (OB_WRP_PAGES96TO103 | OB_WRP_PAGES104TO111 | OB_WRP_PAGES112TO119 | OB_WRP_PAGES120TO127)


/**
  * @}
  */

/** @defgroup FLASHEx_OB_Read_Protection Option Byte Read Protection
  * @{
  */
#define OB_RDP_LEVEL_0            ((uint8_t)0xA5)
#define OB_RDP_LEVEL_1            ((uint8_t)0xFF)
#define OB_RDP_LEVEL_2            ((uint8_t)0xCC)
/**
  * @}
  */
  
/** @defgroup FLASHEx_OB_IWatchdog Option Byte IWatchdog
  * @{
  */ 
#define OB_IWDG_SW                ((uint16_t)0x0001)  /*!< Software IWDG selected */
#define OB_IWDG_HW                ((uint16_t)0x0000)  /*!< Hardware IWDG selected */
/**
  * @}
  */

/** @defgroup FLASHEx_OB_nRST_STOP Option Byte nRST STOP
  * @{
  */ 
#define OB_STOP_NO_RST            ((uint16_t)0x0002) /*!< No reset generated when entering in STOP */
#define OB_STOP_RST               ((uint16_t)0x0000) /*!< Reset generated when entering in STOP */
/**
  * @}
  */ 

/** @defgroup FLASHEx_OB_nRST_STDBY Option Byte nRST STDBY
  * @{
  */ 
#define OB_STDBY_NO_RST           ((uint16_t)0x0004) /*!< No reset generated when entering in STANDBY */
#define OB_STDBY_RST              ((uint16_t)0x0000) /*!< Reset generated when entering in STANDBY */
/**
  * @}
  */



/** @defgroup FLASHEx_OB_Data_Address  Option Byte Data Address
  * @{
  */
#define OB_DATA_ADDRESS_DATA0     (0x1FFFF804U)
#define OB_DATA_ADDRESS_DATA1     (0x1FFFF806U)
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FLASHEx_Constants
  * @{
  */ 

/** @defgroup FLASH_Flag_definition Flag definition
  * @brief Flag definition
  * @{
  */
 

 #define FLASH_FLAG_BSY             FLASH_SR_BSY              /*!< FLASH Busy flag                          */ 
 #define FLASH_FLAG_PGERR           FLASH_SR_PGERR            /*!< FLASH Programming error flag             */
 #define FLASH_FLAG_WRPERR          FLASH_SR_WRPRTERR         /*!< FLASH Write protected error flag         */
 #define FLASH_FLAG_EOP             FLASH_SR_EOP              /*!< FLASH End of Operation flag              */

 #define FLASH_FLAG_OPTVERR         ((OBR_REG_INDEX << 8U | FLASH_OBR_OPTERR)) /*!< Option Byte Error        */
/**
  * @}
  */
  
/** @defgroup FLASH_Interrupt_definition Interrupt definition
  * @brief FLASH Interrupt definition
  * @{
  */


 #define FLASH_IT_EOP               FLASH_CR_EOPIE          /*!< End of FLASH Operation Interrupt source */
 #define FLASH_IT_ERR               FLASH_CR_ERRIE          /*!< Error Interrupt source                  */

/**
  * @}
  */  

/**
  * @}
  */
  

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Macros FLASHEx Exported Macros
  * @{
  */

/** @defgroup FLASH_Interrupt Interrupt
 *  @brief macros to handle FLASH interrupts
 * @{
 */ 


/**
  * @brief  Enable the specified FLASH interrupt.
  * @param  __INTERRUPT__  FLASH interrupt 
  *         This parameter can be any combination of the following values:
  *     @arg @ref FLASH_IT_EOP End of FLASH Operation Interrupt
  *     @arg @ref FLASH_IT_ERR Error Interrupt    
  * @retval none
  */ 
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__)  (FLASH->CR |= (__INTERRUPT__))

/**
  * @brief  Disable the specified FLASH interrupt.
  * @param  __INTERRUPT__  FLASH interrupt 
  *         This parameter can be any combination of the following values:
  *     @arg @ref FLASH_IT_EOP End of FLASH Operation Interrupt
  *     @arg @ref FLASH_IT_ERR Error Interrupt    
  * @retval none
  */ 
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__)   (FLASH->CR &= ~(__INTERRUPT__))

/**
  * @brief  Get the specified FLASH flag status. 
  * @param  __FLAG__ specifies the FLASH flag to check.
  *          This parameter can be one of the following values:
  *            @arg @ref FLASH_FLAG_EOP    FLASH End of Operation flag 
  *            @arg @ref FLASH_FLAG_WRPERR FLASH Write protected error flag 
  *            @arg @ref FLASH_FLAG_PGERR  FLASH Programming error flag
  *            @arg @ref FLASH_FLAG_BSY    FLASH Busy flag
  *            @arg @ref FLASH_FLAG_OPTVERR  Loaded OB and its complement do not match
  * @retval The new state of __FLAG__ (SET or RESET).
  */
#define __HAL_FLASH_GET_FLAG(__FLAG__)          (((__FLAG__) == FLASH_FLAG_OPTVERR) ? \
                                                 (FLASH->OBR & FLASH_OBR_OPTERR) : \
                                                 (FLASH->SR & (__FLAG__)))
/**
  * @brief  Clear the specified FLASH flag.
  * @param  __FLAG__ specifies the FLASH flags to clear.
  *          This parameter can be any combination of the following values:
  *            @arg @ref FLASH_FLAG_EOP    FLASH End of Operation flag 
  *            @arg @ref FLASH_FLAG_WRPERR FLASH Write protected error flag 
  *            @arg @ref FLASH_FLAG_PGERR  FLASH Programming error flag 
  *            @arg @ref FLASH_FLAG_OPTVERR  Loaded OB and its complement do not match
  * @retval none
  */
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)        do { \
                                                    /* Clear FLASH_FLAG_OPTVERR flag */ \
                                                    if ((__FLAG__) == FLASH_FLAG_OPTVERR) \
                                                    { \
                                                        CLEAR_BIT(FLASH->OBR, FLASH_OBR_OPTERR); \
                                                    } \
                                                    else \
                                                    { \
                                                        /* Clear Flag in Bank1 */ \
                                                        FLASH->SR  = (__FLAG__); \
                                                    } \
                                                } while(0)



/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASHEx_Exported_Functions
  * @{
  */

/** @addtogroup FLASHEx_Exported_Functions_Group1
  * @{
  */
/* IO operation functions *****************************************************/
HAL_StatusTypeDef  HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);
HAL_StatusTypeDef  HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);

/**
  * @}
  */

/** @addtogroup FLASHEx_Exported_Functions_Group2
  * @{
  */
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef  HAL_FLASHEx_OBErase(void);
HAL_StatusTypeDef  HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);
void               HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);
uint32_t           HAL_FLASHEx_OBGetUserData(uint32_t DATAAdress);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_FLASH_EX_H_ */


