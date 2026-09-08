/* FreeModbus port: USART2 RS485 serial transport.
 * The byte-level UART driver (Bsp/uart.c) calls into the stack: every RX byte
 * triggers pxMBFrameCBByteReceived(), every TX byte sent triggers
 * pxMBFrameCBTransmitterEmpty() -- the RTU layer then drains/feeds one byte at
 * a time through xMBPortSerialGetByte/xMBPortSerialPutByte.
 *
 * RS485 half-duplex direction (PB8, high = drive / low = receive) is switched
 * at the FRAME level here: vMBPortSerialEnable(_, TRUE) asserts DE before the
 * first byte, and vMBPortSerialEnable(_, FALSE) drops it once the RTU layer has
 * drained the last byte (called from the last TxCplt), so DE holds for the
 * whole frame instead of toggling per byte. */
#include "uart.h"
#include "mb.h"
#include "mbport.h"

static volatile CHAR s_last_byte;

static void uart_rx_isr(uint8_t byte)
{
    s_last_byte = (CHAR)byte;
    (void)pxMBFrameCBByteReceived();
}

static void uart_tx_isr(void)
{
    (void)pxMBFrameCBTransmitterEmpty();
}

BOOL xMBPortSerialInit(UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity, UCHAR ucStopBits)
{
    uart_par_t parity;

    (void)ucPort;
    (void)ucStopBits; /* 1 stop bit only, as configured in uart.c */

    parity = (eParity == MB_PAR_EVEN) ? UART_PAR_EVEN : (eParity == MB_PAR_ODD) ? UART_PAR_ODD : UART_PAR_NONE;

    uart2_set_rx_cb(uart_rx_isr);
    uart2_set_tx_cb(uart_tx_isr);
    uart2_init(ulBaudRate, ucDataBits, parity);
    return TRUE;
}

void xMBPortSerialClose(void)
{
    uart2_rx_enable(false);
    uart2_set_de(false); /* back to receive */
}

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    /* RS485: assert DE while transmitting a frame, drop it otherwise. The RTU
     * layer calls this with xTxEnable=TRUE before the first byte and FALSE from
     * the last byte's TxCplt, so DE spans the whole frame. */
    uart2_set_de(xTxEnable ? true : false);
    uart2_rx_enable(xRxEnable ? true : false);
    /* Kick the first byte: eMBRTUSend only arms the transmitter, the port layer
     * must prime the TX-empty chain. pxMBFrameCBTransmitterEmpty ->
     * xMBRTUTransmitFSM -> xMBPortSerialPutByte -> uart2_putc starts the IT
     * transfer; subsequent bytes are fed from the TxCplt callback. */
    if (xTxEnable)
        (void)pxMBFrameCBTransmitterEmpty();
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    uart2_putc((uint8_t)ucByte);
    return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    *pucByte = s_last_byte;
    return TRUE;
}
