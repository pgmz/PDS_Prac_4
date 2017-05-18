/**
	\file UART_TT.h
	\brief
		This is the header file of the UART of the Tera Term
		In this file all needed for the Tera Term communication is configured.
	\author Anthony Guzmán, Patricio Gómez
	\date	3/03/2017
 */

#ifndef SOURCE_UART_TT_H_
#define SOURCE_UART_TT_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "clock_config.h"

#include "board.h"
#include "pin_mux.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "clock_config.h"


/**
 * \brief Definitions to configure the buffer of the UART for the echo
 */
#define ECHO_BUFFER_LENGTH 1

/**
 * \brief Definitions to determine the values in ASCII from A to F, enter and ESC
 */
#define A_A 65
#define F_A 70
#define ENTER_VALUE 13
#define ESC_VALUE '\e'

typedef struct{
	uint8_t Mailbox_Flag :1;
	uint8_t Mailbox_Data;
}UART_Mailbox_Type;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is callback of the UART
 	 \param[in] UART_Type (receives the pointer of the UART if it is UART0 or UART4)
 	 \param[in] uart_handle_t (handle pointer of the UART)
 	 \param[in] status_t (status of the UART)
 	 \param[in] void (pointer to the data that is the used)
 	 \return void
 */
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is initialization of the UART for the Tera Term
 	 \param[in] void (receives nothing)
 	 \return void
 */
void UART_Init_(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is echo of the Tera Term
 	 \param[in] void (receives nothing)
 	 \return void
 */
void UART_Echo();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is return of the UART handler
 	 \param[in]  void (receives nothing)
 	 \return uart_handle_t
 */
uart_handle_t *Handler();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is return of the txOnGoingValue of the Tera Term
 	 \param[in]  void (receives nothing)
 	 \return volatile bool
 */
volatile bool txOnGoing_Value();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is the assign value of the txOnGoingValue of the Tera Term
 	 \param[in]  uint8_t (receives value for the txOnGoingValue )
 	 \return void
 */
void txOnGoing_Assign_Value(uint8_t value);

#endif /* SOURCE_UART_TT_H_ */
