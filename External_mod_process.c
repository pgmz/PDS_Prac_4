/*
 * External_mod_process.c
 *
 *  Created on: 12/03/2017
 *      Author: Patricio Gomez
 */

#include "External_mod_process.h"

uint32_t PORTC_INT_FLAGS;

Mod_SM_type Mod_SM[3] = {
		{TREMOLO_RATE, TREMOLO_ALPHA, 20, TREMOLO_RATE_INC, TREMOLO_RATE_MAX, TREMOLO_RATE_MIN, true, true, false},
		{TREMOLO_ALPHA, AMP_PARAM, 0.5, TREMOLO_ALPHA_INC, TREMOLO_ALPHA_MAX, TREMOLO_ALPHA_MIN, false, true, true},
		{AMP_PARAM, TREMOLO_RATE, 1, AMP_PARAM_INC, AMP_PARAM_MAX, AMP_PARAM_MIN, true, false, true}
};

float *Effect_Rate = &Mod_SM[TREMOLO_RATE].Modifier;
float *Effect_Alpha = &Mod_SM[TREMOLO_ALPHA].Modifier;
float *Effect_Amplitude = &Mod_SM[AMP_PARAM].Modifier;

Mod_State_type Current_state = TREMOLO_RATE;

void External_mod_process_init(){

	/*Init botones y LEDS**/

	  CLOCK_EnableClock(kCLOCK_PortC);

		port_pin_config_t config =
		{
				kPORT_PullUp,
				kPORT_FastSlewRate,
				kPORT_PassiveFilterDisable,
				kPORT_OpenDrainDisable,
				kPORT_LowDriveStrength,
				kPORT_MuxAsGpio,
				kPORT_UnlockRegister
		};

		gpio_pin_config_t config_gpio =
		{
				kGPIO_DigitalInput
		};

		PORT_SetPinConfig(PORTC, 5U, &config);
		PORT_SetPinConfig(PORTC, 7U, &config);
		PORT_SetPinConfig(PORTC, 0U, &config);

		NVIC_SetPriority(PORTC_IRQn, 7);
		NVIC_EnableIRQ(PORTC_IRQn);


		PORT_SetPinInterruptConfig(PORTC, 5U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 5U, &config_gpio);

		PORT_SetPinInterruptConfig(PORTC, 7U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 7U, &config_gpio);

		PORT_SetPinInterruptConfig(PORTC, 0U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 0U, &config_gpio);

		gpio_pin_config_t config_led =
		{
				kGPIO_DigitalOutput
		};

		CLOCK_EnableClock(kCLOCK_PortB);
		CLOCK_EnableClock(kCLOCK_PortE);

		PORT_SetPinMux(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN, kPORT_MuxAsGpio);
		GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, &config_led);
		PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, kPORT_MuxAsGpio);
		GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &config_led);
		PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, kPORT_MuxAsGpio);
		GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &config_led);

		LED_GREEN_OFF();
		LED_RED_OFF();
		LED_BLUE_OFF();

		GPIO_WritePinOutput(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, Mod_SM[Current_state].led1);
		GPIO_WritePinOutput(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, Mod_SM[Current_state].led2);
		GPIO_WritePinOutput(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, Mod_SM[Current_state].led3);
}

void Modify_amplitude(uint8_t Increment){
	if(Increment){
		if(Mod_SM[Current_state].Modifier >= Mod_SM[Current_state].Modifier_max){
			Mod_SM[Current_state].Modifier = Mod_SM[Current_state].Modifier_max;
		} else {
			Mod_SM[Current_state].Modifier += Mod_SM[Current_state].Modifier_inc;
		}
	} else if(Increment == false){
		if(Mod_SM[Current_state].Modifier <= Mod_SM[Current_state].Modifier_min){
			Mod_SM[Current_state].Modifier = Mod_SM[Current_state].Modifier_min;
		} else {
			Mod_SM[Current_state].Modifier -= Mod_SM[Current_state].Modifier_inc;
		}
	}
}

void PORTC_IRQHandler(){

	/*De acuerdo al boton oprimido cambiar amplitud, estado o frecuencia de muestreo**/

	PORTC_INT_FLAGS = GPIO_GetPinsInterruptFlags(GPIOC);
	PORT_ClearPinsInterruptFlags(PORTC, ~0U);

	/*Cambiar estado y actualizar LEDS**/
	switch(PORTC_INT_FLAGS){
	case 1<<5U:
			Current_state = Mod_SM[Current_state].Mod_Next;
			GPIO_WritePinOutput(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, Mod_SM[Current_state].led1);
			GPIO_WritePinOutput(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, Mod_SM[Current_state].led2);
			GPIO_WritePinOutput(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, Mod_SM[Current_state].led3);
		    break;

    /*Incrementar de acuerdo al estado**/
	case 1<<7U:
			Modify_amplitude(true);
			break;

	/*Decrementar de acuerdo al estado**/
	case 1<<0U:
			Modify_amplitude(false);
			break;
	}

	PORTC_INT_FLAGS = 0;
}
