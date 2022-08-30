/**
 iPodBtCommBridge

  Author:
    Tucker Osman

  File Name:
    main.c

  Summary:
    This is the main code that controls the iPod n3g bluetooth communication bridge.

  Description:
    Generation Information :
	Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
	Device            :  PIC16F15213
	Driver Version    :  2.00
 */

#include "mcc_generated_files/mcc.h"
#include "ringbuffer.h"

ring_buffer_t input_buffer;
ring_buffer_t output_buffer;

uint8_t i2c_rx_byte, i2c_tx_byte;

static void I2C1_ReadISR(void)
{
	i2c_rx_byte = SSP1BUF;
	switch (i2c_rx_byte) {
	case 0x11: // device control 1: disable BT module
		BT_ENABLE_SetLow();
		break;
	case 0x12:
		BT_ENABLE_SetHigh();
		break;
	default:
		if (!ring_buffer_is_full(&output_buffer)) {
			ring_buffer_queue(&output_buffer, i2c_rx_byte);
		}
		break;
	}
}

static void I2C1_WriteISR(void)
{
	if (!ring_buffer_is_empty(&input_buffer)) {
		ring_buffer_dequeue(&input_buffer, &i2c_tx_byte);
		SSP1BUF = i2c_tx_byte;
	} else {
		SSP1BUF = 0x00;
	}
	SSP1CON1bits.CKP = 1;
}

void main(void)
{
	SYSTEM_Initialize();

	INTERRUPT_GlobalInterruptEnable();
	INTERRUPT_PeripheralInterruptEnable();

	// start i2c
	I2C1_Open();
	I2C1_SlaveSetReadIntHandler(I2C1_ReadISR);
	I2C1_SlaveSetWriteIntHandler(I2C1_WriteISR);

	ring_buffer_init(&input_buffer);
	ring_buffer_init(&output_buffer);

	char data;
	uint8_t count = 0;

	while (1) {
		// from the iPod to the BT transmitter
		if (!ring_buffer_is_empty(&output_buffer)) {
			count = ring_buffer_num_items(&output_buffer);

			for (uint8_t i = 0; i < count; i++) {
				ring_buffer_dequeue(&output_buffer, &data);
				putch(data);
			}
		}

		// from the BT transmitter to the iPod
		if (EUSART1_is_rx_ready()) {
			data = RC1REG;
			ring_buffer_queue(&input_buffer, data);
		}
	}
}
