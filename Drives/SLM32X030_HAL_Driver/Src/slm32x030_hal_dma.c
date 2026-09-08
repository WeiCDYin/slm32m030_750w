/**
  ******************************************************************************
  * @file    slm32x030_hal_dma.c
  * @author  Application Team
  * @version V1.0.0
  * @date    8-11-2024
  * @brief   
  ******************************************************************************
 **/
#include "slm32x030_hal.h"

/** @addtogroup SLM32F1XA_HAL_Driver
  * @{
  */

/** @defgroup DMA DMA
  * @brief DMA HAL module driver
  * @{
  */

#ifdef HAL_DMA_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup DMA_Private_Functions DMA Private Functions
  * @{
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint16_t DataLength);
/**
  * @}
  */

/**
  * @brief  Initialize the DMA according to the specified
  *         parameters in the DMA_InitTypeDef and initialize the associated handle.
  * @param  hdma: Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma)
{
    uint32_t tmp = 0;

    /* Check the DMA handle allocation */
    if(hdma == NULL)
    {
        return HAL_ERROR;
    }

    hdma->Instance = (DMA_Channel_TypeDef *)(DMA_CHN_BASE + hdma->Init.DmaChannel * DMA_CHN_OFFSET);

    hdma->ChannelNum = (DMA_ChannelDef)((((uint32_t)hdma->Instance) - DMA_CHN_BASE) / DMA_CHN_OFFSET);
    hdma->DmaBaseAddress->CONFIG = DMA_EN_ENABLE;

    /* Change DMA peripheral state */
    hdma->State = HAL_DMA_STATE_BUSY;

    /* Get the CONTROL register value */
    tmp = hdma->Instance->CONTROL;

    tmp &= ~(DMA_DINC_ENABLE | DMA_SINC_ENABLE | DMA_BRKEN_ENABLE);
    tmp |= (hdma->Init.DestInc | hdma->Init.SrcInc | hdma->Init.Brken);
    
    /* Write to DMA Channel CONTROL register */
    hdma->Instance->CONTROL = tmp;

    /* Get the CONFIG register value */
    tmp = hdma->Instance->CONFIG;

    tmp &= ~(DMA_CHX_CONFIG_MODE | DMA_CHX_CONFIG_SRC_PERIPH | DMA_CHX_CONFIG_DEST_PERIPH);
    tmp |= (hdma->Init.Flowcntrl << DMA_CHX_CONFIG_MODE_Pos);
    tmp |= (hdma->Init.DmaChannel << DMA_CHX_CONFIG_SRC_PERIPH_Pos);
    tmp |= (hdma->Init.DmaChannel << DMA_CHX_CONFIG_DEST_PERIPH_Pos);

    tmp &= ~(DMA_CHX_CONFIG_SRCCONTI | DMA_CHX_CONFIG_DESTCONTI);

    /* Write to DMA Channel CONFIG register */
    hdma->Instance->CONFIG = tmp;

    __HAL_DMA_SET_PERIPH(hdma, hdma->Init.Periphx_sel);

    /* Initialise the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;
    /* Initialize the DMA state*/
    hdma->State = HAL_DMA_STATE_READY;
    /* Allocate lock resource and initialize it */
    hdma->Lock = HAL_UNLOCKED;

    return HAL_OK; 
}

/**
  * @brief  DeInitialize the DMA peripheral.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
    /* Check the DMA handle allocation */
    if(hdma == NULL)
    {
        return HAL_ERROR;
    }

    /* Disable the selected DMA Channelx */
    __HAL_DMA_CLR_CHX_EN(hdma);

    /* Reset DMA Channel control register */
    // hdma->Instance->CONTROL = 0x0U;

    /* Reset DMA Channel config register */
    // hdma->Instance->CONFIG = 0x0U;

    /* Clear all flags */
    __HAL_DMA_CLEAR_INTTCCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);
    __HAL_DMA_CLEAR_INTERRCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);
    __HAL_DMA_CLEAR_OVCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);

    /* Clean all callbacks */
    hdma->XferCpltCallback = NULL;
    hdma->XferOverlapCallback = NULL;
    hdma->XferErrorCallback = NULL;
    hdma->XferAbortCallback = NULL;

    /* Reset the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;

    /* Reset the DMA state */
    hdma->State = HAL_DMA_STATE_RESET;

    /* Release Lock */
    __HAL_UNLOCK(hdma);

    return HAL_OK;
}

/**
  * @brief  Start the DMA Transfer.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.
  * @param  SrcAddress: The source memory Buffer address
  * @param  DstAddress: The destination memory Buffer address
  * @param  DataLength: The length of data to be transferred from source to destination
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
    /* Process locked */
    __HAL_LOCK(hdma);

    if(HAL_DMA_STATE_READY == hdma->State)
    {
        /* Change DMA peripheral state */
        hdma->State = HAL_DMA_STATE_BUSY;
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;

        /* Disable the peripheral */
        __HAL_DMA_CLR_CHX_EN(hdma);

        /* Clear conti mode */
        hdma->Instance->CONTROL &= ~DMA_CONTI_ENABLE;

        /* Configure the source, destination address and the data length & clear flags*/
        DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);

        if (hdma->Init.Continue == DMA_CONTI_ENABLE)
        {
            hdma->Instance->CONTROL |= hdma->Init.Continue;
        }
        
        /* Enable the Peripheral */    
        __HAL_DMA_SET_CHX_EN(hdma);
    }
    else
    {
        /* Process Unlocked */
        __HAL_UNLOCK(hdma); 
        return HAL_BUSY;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_DMA_USART_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
    uint32_t data;  

    /* Process locked */
    __HAL_LOCK(hdma);

    if(HAL_DMA_STATE_READY == hdma->State)
    {
        /* Change DMA peripheral state */
        hdma->State = HAL_DMA_STATE_BUSY;
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;

        /* Disable the peripheral */
        __HAL_DMA_CLR_CHX_EN(hdma);

        /* Clear conti mode */
        hdma->Instance->CONTROL &= ~DMA_CONTI_ENABLE;

        /* USART */
        data = *(uint32_t *)(SrcAddress);
        if((READ_BIT(USART1->CR1, USART_CR1_UE) == USART_CR1_UE))
        {
            WRITE_REG(USART1->DR, data);
            while(READ_BIT(USART1->SR, USART_SR_TXE) == RESET);
        } 
        else if(READ_BIT(USART2->CR1, USART_CR1_UE) == USART_CR1_UE)
        {
            WRITE_REG(USART2->DR, data);
            while(READ_BIT(USART2->SR, USART_SR_TXE) == RESET);
        }

        /* Configure the source, destination address and the data length & clear flags*/
        DMA_SetConfig(hdma, SrcAddress + 4, DstAddress, DataLength - 1);

        if (hdma->Init.Continue == DMA_CONTI_ENABLE)
        {
            hdma->Instance->CONTROL |= hdma->Init.Continue;
        }
        
        /* Enable the Peripheral */    
        __HAL_DMA_SET_CHX_EN(hdma);
    }
    else
    {
        /* Process Unlocked */
        __HAL_UNLOCK(hdma); 
        return HAL_BUSY;
    }

    return HAL_OK;
}

/**
  * @brief  Starts the DMA Transfer with interrupt enabled.
  * @param  hdma      : the configuration information for the specified DMA Channel.
  * @param  SrcAddress: The source memory Buffer address
  * @param  DstAddress: The destination memory Buffer address
  * @param  DataLength: The length of data to be transferred from source to destination
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
    /* Process locked */
    __HAL_LOCK(hdma);

    if(HAL_DMA_STATE_READY == hdma->State)
    {
        /* Change DMA peripheral state */
        hdma->State = HAL_DMA_STATE_BUSY;
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;

        /* Disable the peripheral */
        __HAL_DMA_CLR_CHX_EN(hdma);

        /* Clear conti mode */
        hdma->Instance->CONTROL &= ~DMA_CONTI_ENABLE;
        
        if (hdma->Init.Continue == DMA_CONTI_ENABLE)
        {
            hdma->Instance->CONTROL |= hdma->Init.Continue;
        }

        /* Configure the source, destination address and the data length & clear flags*/
        DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);

        __HAL_DMA_ENABLE_IT(hdma, DMA_IT_TCINT | DMA_IT_ERRINT);
        
        /* Enable the Peripheral */    
        __HAL_DMA_SET_CHX_EN(hdma);
    }
    else
    {
        /* Process Unlocked */
        __HAL_UNLOCK(hdma); 
        return HAL_BUSY;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_DMA_USART_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
    uint32_t data;

    /* Process locked */
    __HAL_LOCK(hdma);

    if(HAL_DMA_STATE_READY == hdma->State)
    {
        /* Change DMA peripheral state */
        hdma->State = HAL_DMA_STATE_BUSY;
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;

        /* Disable the peripheral */
        __HAL_DMA_CLR_CHX_EN(hdma);

        /* Clear conti mode */
        hdma->Instance->CONTROL &= ~DMA_CONTI_ENABLE;

        /* USART */
        data = *(uint32_t *)(SrcAddress);
        if((READ_BIT(USART1->CR1, USART_CR1_UE) == USART_CR1_UE))
        {
            WRITE_REG(USART1->DR, data);
            while(READ_BIT(USART1->SR, USART_SR_TXE) == RESET);
        } 
        else if(READ_BIT(USART2->CR1, USART_CR1_UE) == USART_CR1_UE)
        {
            WRITE_REG(USART2->DR, data);
            while(READ_BIT(USART2->SR, USART_SR_TXE) == RESET);
        }

        /* Configure the source, destination address and the data length & clear flags*/
        DMA_SetConfig(hdma, SrcAddress + 4, DstAddress, DataLength - 1);

        if (hdma->Init.Continue == DMA_CONTI_ENABLE)
        {
            hdma->Instance->CONTROL |= hdma->Init.Continue;
        }

        __HAL_DMA_ENABLE_IT(hdma, DMA_IT_TCINT | DMA_IT_ERRINT);
        
        /* Enable the Peripheral */    
        __HAL_DMA_SET_CHX_EN(hdma);
    }
    else
    {
        /* Process Unlocked */
        __HAL_UNLOCK(hdma); 
        return HAL_BUSY;
    }

    return HAL_OK;
}

/**
  * @brief  Aborts the DMA Transfer.
  * @param  hdma  : the configuration information for the specified DMA Channel.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma)
{
    return HAL_OK;
}

/**
  * @brief  Aborts the DMA Transfer in Interrupt mode.
  * @param  hdma  : pointer to a DMA_HandleTypeDef structure that contains
  *                 the configuration information for the specified DMA Channel.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma)
{
    return HAL_OK;
}

/**
  * @brief  Polling for transfer complete.
  * @param  hdma:    pointer to a DMA_HandleTypeDef structure that contains
  *                  the configuration information for the specified DMA Channel.
  * @param  CompleteLevel: Specifies the DMA level complete.
  * @param  Timeout:       Timeout duration.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t Timeout)
{
    return HAL_OK;
}

/**
  * @brief  Polling for transfer complete.
  * @param  hdma:    pointer to a DMA_HandleTypeDef structure that contains
  *                  the configuration information for the specified DMA Channel.
  * @param  CompleteLevel: Specifies the DMA level complete.
  * @param  Timeout:       Timeout duration.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_USART_PollForTransfer(DMA_HandleTypeDef *hdma)
{
    //Disable the current channel and allow other channels to work, with only one channel working at a time
    // *(__IO uint32_t *)(DMA_CH0_CONFIG_ADDR + hdma->Init.ChannelNum*0x20) &= ~(DMA_Ch_En_ENABLE << DMA_CHX_CONFIG_CH_EN_Pos);  

    // if((READ_BIT(USART1->CR1, USART_CR1_UE) == USART_CR1_UE))
    // {
    //     if((READ_BIT(USART1->CR1, USART_CR1_TE) == USART_CR1_TE) && (READ_BIT(USART1->CR3, USART_CR3_DMAT) == USART_CR3_DMAT))
    //     {
    //         while(READ_BIT(USART1->SR, USART_SR_TXE) == RESET);
    //     }      
    // } 
    // else if(READ_BIT(USART2->CR1, USART_CR1_UE) == USART_CR1_UE)
    // {
    //     if((READ_BIT(USART2->CR1, USART_CR1_TE) == USART_CR1_TE) && (READ_BIT(USART2->CR3, USART_CR3_DMAT) == USART_CR3_DMAT))
    //     {
    //         while(READ_BIT(USART2->SR, USART_SR_TXE) == RESET);
    //     } 
    // }

    // SET_BIT(DMA->INTTCCLR, 1 << (hdma->Init.ChannelNum)); 
    // if(READ_BIT(DMA->RAWINTERRSTA , 1 << (hdma->Init.ChannelNum)) != RESET)
    // {
    //    SET_BIT(DMA->INTERRCLR, 1 << (hdma->Init.ChannelNum));    
    // }

    // if(READ_BIT(DMA->OVSTA , 1 << (hdma->Init.ChannelNum)) != RESET)
    // {
    //    SET_BIT(DMA->OVCLR, 1 << (hdma->Init.ChannelNum)); 
    // }
    return HAL_OK;
}

void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
    /* USART */
    if((READ_BIT(USART1->CR1, USART_CR1_UE) == USART_CR1_UE))
    {
        if((READ_BIT(USART1->CR1, USART_CR1_TE) == USART_CR1_TE) && (READ_BIT(USART1->CR3, USART_CR3_DMAT) == USART_CR3_DMAT))
        {
            while(READ_BIT(USART1->SR, USART_SR_TXE) == RESET);
        }      
    } 
    else if(READ_BIT(USART2->CR1, USART_CR1_UE) == USART_CR1_UE)
    {
        if((READ_BIT(USART2->CR1, USART_CR1_TE) == USART_CR1_TE) && (READ_BIT(USART2->CR3, USART_CR3_DMAT) == USART_CR3_DMAT))
        {
            while(READ_BIT(USART2->SR, USART_SR_TXE) == RESET);
        } 
    }

    /* Transfer Error Interrupt management ***************************************/
    if(__HAL_DMA_GET_INTERRSTA_FLAG(hdma, 0x1U << hdma->ChannelNum) != RESET)
    {
        /* Disable the transfer error interrupt */
        __HAL_DMA_DISABLE_IT(hdma, DMA_IT_ALL);
        /* Clear the transfer error flag */
        __HAL_DMA_CLEAR_INTERRCLR_FLAG(hdma, 0x1U << hdma->ChannelNum); 
        /* Update error code */
        hdma->ErrorCode = HAL_DMA_ERROR_TE;
        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;
        /* Process Unlocked */
        __HAL_UNLOCK(hdma);  

        if(hdma->XferErrorCallback != NULL)
        {
            /* Transfer error callback */
            hdma->XferErrorCallback(hdma);
        }
    }

    /* Transfer Complete Interrupt management ***************************************/
    if(__HAL_DMA_GET_INTTCSTA_FLAG(hdma, 0x1U << hdma->ChannelNum) != RESET)
    {
        if(RESET == (hdma->Instance->CONTROL & DMA_CHX_CONTROL_CONTI))
        {
            /* Disable the transfer complete and error interrupt */
            __HAL_DMA_DISABLE_IT(hdma, DMA_IT_TCINT | DMA_IT_ERRINT);
            /* Change the DMA state */
            hdma->State = HAL_DMA_STATE_READY;
            /* Process Unlocked */
            __HAL_UNLOCK(hdma);
        }

        /* Clear the complete transfer flag */
        __HAL_DMA_CLEAR_INTTCCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);

        if(hdma->XferCpltCallback != NULL)
        {
            /* Transfer complete callback */
            hdma->XferCpltCallback(hdma);
        }
    }

    /* Overload management ***************************************/
    if(__HAL_DMA_GET_OVSTA_FLAG(hdma, 0x1U << hdma->ChannelNum) != RESET)
    {
        /* Clear overload flag */
        __HAL_DMA_CLEAR_OVCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);

        if((hdma->XferOverlapCallback != NULL))
        {
            /* Transfer Overload callback */
            hdma->XferOverlapCallback(hdma);
        }
    }
}

/**
  * @brief Register callbacks
  * @param hdma: pointer to a DMA_HandleTypeDef structure that contains
  *              the configuration information for the specified DMA Channel.
  * @param CallbackID: User Callback identifer
  *                    a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
  * @param pCallback: pointer to private callbacsk function which has pointer to 
  *                   a DMA_HandleTypeDef structure as parameter.
  * @retval HAL status
  */                          
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)( DMA_HandleTypeDef * _hdma))
{
    HAL_StatusTypeDef status = HAL_OK;

    /* Process locked */
    __HAL_LOCK(hdma);

    if(HAL_DMA_STATE_READY == hdma->State)
    {
        switch (CallbackID)
        {
            case  HAL_DMA_XFER_CPLT_CB_ID:
                hdma->XferCpltCallback = pCallback;
            break; 

            case  HAL_DMA_XFER_OVERLAP_CB_ID:
                hdma->XferOverlapCallback = pCallback;
            break; 

            case  HAL_DMA_XFER_ERROR_CB_ID:
                hdma->XferErrorCallback = pCallback;
            break;         

            case  HAL_DMA_XFER_ABORT_CB_ID:
                hdma->XferAbortCallback = pCallback;
            break; 
            
            default:
                status = HAL_ERROR;
            break;                                                            
        }
    }
    else
    {
        status = HAL_ERROR;
    } 

    /* Release Lock */
    __HAL_UNLOCK(hdma);

    return status;
}

/**
  * @brief UnRegister callbacks
  * @param hdma: pointer to a DMA_HandleTypeDef structure that contains
  *              the configuration information for the specified DMA Channel.
  * @param CallbackID: User Callback identifer
  *                    a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
  * @retval HAL status
  */              
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;

    /* Process locked */
    __HAL_LOCK(hdma);
    
    if(HAL_DMA_STATE_READY == hdma->State)
    {
        switch (CallbackID)
        {
            case  HAL_DMA_XFER_CPLT_CB_ID:
                hdma->XferCpltCallback = NULL;
            break;

            case  HAL_DMA_XFER_OVERLAP_CB_ID:
                hdma->XferOverlapCallback = NULL;
            break;             

            case  HAL_DMA_XFER_ERROR_CB_ID:
                hdma->XferErrorCallback = NULL;
            break;         

            case  HAL_DMA_XFER_ABORT_CB_ID:
                hdma->XferAbortCallback = NULL;
            break; 

            case  HAL_DMA_XFER_ALL_CB_ID:
                hdma->XferCpltCallback = NULL;
                hdma->XferOverlapCallback = NULL;
                hdma->XferErrorCallback = NULL;
                hdma->XferAbortCallback = NULL;
            break; 

            default:
                status = HAL_ERROR;
            break;
        }
    }
    else
    {
        status = HAL_ERROR;
    } 
    
    /* Release Lock */
    __HAL_UNLOCK(hdma);
    
    return status;
}

/**
  * @brief  Return the DMA hande state.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.
  * @retval HAL state
  */
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma)
{
    /* Return DMA handle state */
    return hdma->State;
}

/**
  * @brief  Return the DMA error code.
  * @param  hdma : pointer to a DMA_HandleTypeDef structure that contains
  *              the configuration information for the specified DMA Channel.
  * @retval DMA Error Code
  */
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma)
{
    return hdma->ErrorCode;
}

/** @addtogroup DMA_Private_Functions
  * @{
  */

/**
  * @brief  Sets the DMA Transfer parameter.
  * @param  hdma:       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Channel.
  * @param  SrcAddress: The source memory Buffer address
  * @param  DstAddress: The destination memory Buffer address
  * @param  DataLength: The length of data to be transferred from source to destination
  * @retval HAL status
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint16_t DataLength)
{
    /* Clear the DMA Channel all flags */
    __HAL_DMA_CLEAR_INTTCCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);
    __HAL_DMA_CLEAR_INTERRCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);
    __HAL_DMA_CLEAR_OVCLR_FLAG(hdma, 0x1U << hdma->ChannelNum);

    /* Configure DMA Channel data length */
    hdma->Instance->CONTROL &= ~DMA_CHX_CONTROL_TS;
    hdma->Instance->CONTROL |= DataLength;

    /* Configure DMA Channel source address */
    hdma->Instance->SRCADDR = SrcAddress;

    /* Configure DMA Channel destination address */
    hdma->Instance->DESTADDR = DstAddress;
}

#endif  /* HAL_DMA_MODULE_ENABLED */

