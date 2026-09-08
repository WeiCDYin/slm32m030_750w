#ifndef __UART_H__
#define __UART_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/*
 * USART2 byte-level driver for the Modbus RTU link over RS485.
 *
 * RX is interrupt driven one byte at a time: uart2_rx_enable(true) arms a
 * single-byte receive; every received byte lands in the registered rx
 * callback. TX is interrupt driven too: uart2_putc() starts a one-byte
 * transfer and the tx callback fires when the byte has been sent -- exactly
 * the flow FreeModbus' RTU port expects (one byte per callback, the stack
 * decides whether the frame continues).
 *
 * RS485 half-duplex direction is PB8: HIGH drives (TX), LOW receives. The
 * direction is switched at the FRAME level by the FreeModbus port
 * (portserial.c vMBPortSerialEnable), not per byte, so DE stays asserted for
 * the whole frame and drops only after the last byte has been shifted out.
 * A write to uart2_set_de() is all a caller needs -- the PB8 pin is owned by
 * this module (MspInit) and must not be re-used elsewhere.
 */

typedef enum
{
    UART_PAR_NONE = 0,
    UART_PAR_EVEN,
    UART_PAR_ODD,
} uart_par_t;

typedef void (*uart_rx_cb_t)(uint8_t byte);
typedef void (*uart_tx_cb_t)(void);

/* Init USART2: 8N1/8E1/8O1/9-bit parity, 1 stop bit, TX+RX. data_bits is 8 or 9. */
void uart2_init(uint32_t baud, uint8_t data_bits, uart_par_t parity);

/* Callbacks: rx fires per received byte; tx fires per byte sent. */
void uart2_set_rx_cb(uart_rx_cb_t cb);
void uart2_set_tx_cb(uart_tx_cb_t cb);

/* Arm/disarm the single-byte receive chain. */
void uart2_rx_enable(bool enable);

/* Start an interrupt-driven one-byte transmit. */
void uart2_putc(uint8_t byte);

/* RS485 direction: true -> PB8 high (drive), false -> PB8 low (receive). */
void uart2_set_de(bool tx);

#ifdef __cplusplus
}
#endif
#endif
