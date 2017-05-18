/**
	\file UART_TT.c
	\brief
		This is the starter file of the UART of the Tera Term.
		In this file all needed for the Tera Term communication is configured.
	\author Anthony Guzman, Patricio Gomez
	\date	3/03/2017
 */
#include "UART_TT.h"

/***Variable that determines the handler of the UART*/
uart_handle_t g_uartHandle;

/***Variable that configures the UART*/
uart_config_t user_config;

/***Variable that sends the data in the UART*/
uart_transfer_t sendXfer;

/***Variable that receives the data in the UART*/
uart_transfer_t receiveXfer;

/***Variable that prints the data in the terminal*/
uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};

/***Variable that receives the data in the terminal*/
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};

/***Variable that receives the value of the receive buffer of the terminal*/
volatile bool rxBufferEmpty = true;

/***Variable that receives the value of the transfer buffer*/
volatile bool txBufferFull = false;

/***Variable that determines if the a char is being sent*/
volatile bool txOnGoing = false;

/***Variable that determines if the a char is receive sent*/
volatile bool rxOnGoing = false;

UART_Mailbox_Type UART_Mailbox = {
		0, 0
};

void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing = false;
    }
}

void UART_Init_(void)
{

	UART_GetDefaultConfig(&user_config);
    user_config.baudRate_Bps = 115200;
    user_config.enableTx = true;
    user_config.enableRx = true;

	UART_Init(UART0, &user_config, CLOCK_GetFreq(UART0_CLK_SRC));
	UART_TransferCreateHandle(UART0, &g_uartHandle, UART_UserCallback, NULL);

	sendXfer.data = g_txBuffer;
	sendXfer.dataSize = ECHO_BUFFER_LENGTH;
	receiveXfer.data = g_rxBuffer;
	receiveXfer.dataSize = ECHO_BUFFER_LENGTH;

}

void UART_Echo ()
{
	/* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
	if ((!rxOnGoing) && rxBufferEmpty)
	{
		rxOnGoing = true;
		UART_TransferReceiveNonBlocking(UART0, &g_uartHandle, &receiveXfer, NULL);
	}

	/* If TX is idle and g_txBuffer is full, start to send data. */
	if ((!txOnGoing) && txBufferFull)
	{
		txOnGoing = true;
		UART_TransferSendNonBlocking(UART0, &g_uartHandle, &sendXfer);
	}

	/* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
	if ((!rxBufferEmpty) && (!txBufferFull))
	{
		rxBufferEmpty = true;
		UART_Mailbox.Mailbox_Flag = true;
		UART_Mailbox.Mailbox_Data = g_rxBuffer[0];
	}
}

uart_handle_t *Handler()
{
	return &g_uartHandle;
}

volatile bool txOnGoing_Value()
{
	return txOnGoing;
}

void txOnGoing_Assign_Value(uint8_t value)
{
	txOnGoing = value;
}

uint8_t UART_Mailbox_Flag(){
	return UART_Mailbox.Mailbox_Flag;
}

uint8_t UART_Mailbox_Data(){
	UART_Mailbox.Mailbox_Flag = false;
	return UART_Mailbox.Mailbox_Data;
}
