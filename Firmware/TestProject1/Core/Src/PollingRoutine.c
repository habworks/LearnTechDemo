/*
 * PollingRoutine.c
 * Hab Collector
 * Test1
 */

#include "main.h"
#include "PollingRoutine.h"
#include "string.h"
#include "cmsis_os.h"  // included for typedef of osSemaphoreId

// FOR EVERYTHING BUT ADC STUFF
extern UART_HandleTypeDef huart6;
extern osSemaphoreId binarySemUartMsgHandle;
extern osMessageQId queueScreen1Handle;
extern osMessageQId queueScreen2Handle;
uint8_t uartMsgBuf[UART_BUF_SIZE];
uint8_t uartMsgData[2];
uint8_t msgIndex = 0;
uint8_t msgRdyFlag = 0;
ButtonClicked screen1ButtonClicked;
ButtonClicked screen2ButtonClicked;


// FOR ADC STUFF
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern osSemaphoreId binarySemAnalogHandle;
extern osThreadId taskAnalogInputHandle;
uint16_t ADC_ReadValue[10] = {0};
float Mean = 0;
uint8_t ADC_UpddateDisplay = 0;
volatile uint8_t Flag1 = 0;
volatile uint8_t Flag2 = 0;


// FOR ALL
Type_PresentScreen PresentScreen = OPEN_SCREEN;




const char helloFromST[] = "Hello Hab from STM32 ";
const char screen1[] = "Screen 1\r\n";
const char screen2[] = "Screen 2\r\n";
const char noPermission[] = "You do not have permission to toggle LED\r\n";
const char permission[] = "You are permitted to toggle LED\r\n";
const char ledOn[]= "LED ON\r\n";
const char ledOff[] = "LED OFF\r\n";

const char myString[100];


// This is the init portion for the task StartTaskUartMsg
// Start the Uart for use in IRQ mode
void PollingInit()
{
	HAL_UART_Receive_IT(&huart6, uartMsgData, 1);
}


// This is body of task StartTaskUartMsg
// Via Global Vars it can be used to check the status of any screen button presses
void PollingRoutine()
{
	uint16_t item;
	if (msgRdyFlag)
	{
		xSemaphoreGive(binarySemUartMsgHandle);
		msgRdyFlag = 0;
	}

	// A queue containing the button pressed for Screen 1 is received - it is checked to see which button is pressed and take action
	if ((xQueueReceive(queueScreen1Handle, &item, (TickType_t)10)) == pdPASS)
	{
		memset(&myString, 0, sizeof(myString));
		switch(item)
		{
		case 1:			// Toggle button
			if (screen1ButtonClicked.Status.button1 == 0)
			{
				strcat(myString, noPermission);
			}
			else
			{
				strcat(myString, permission);
			}
			break;
		case 2:
			strcat(myString, helloFromST);
			strcat(myString, screen1);
			break;
		}
		SendUartMsg(myString);
	}

	// A queue containing the button pressed for Screen 2 is received - it is checked to see which button is pressed and take action
	if ((xQueueReceive(queueScreen2Handle, &item, (TickType_t)10)) == pdPASS)
	{
		memset(myString, 0, sizeof(myString));
		switch(item)
		{
		case 1:		// Toggle button, to set led

			if (screen2ButtonClicked.Status.button1 == 0)
			{
				strcat(myString, ledOff);
				HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
			}
			else
			{
				strcat(myString, ledOn);
				HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
			}
			break;
		case 2:
			strcat(myString, helloFromST);
			strcat(myString, screen2);
			break;
		}
		SendUartMsg(myString);
	}

}


// This function transmits a msg via the Uart
// It is done via IRQ so only the msg and msg lenght need be loaded
void SendUartMsg(char *msg)
{
	if (HAL_UART_Transmit_IT(&huart6, (uint8_t *)msg, strlen(msg)) != HAL_OK)
	{
		// Check for error
	}
}


// This is the IRQ Callback function for the Uart.  It is defined as weak in HAL Lib.
// Use this IRQ to capture incoming bytes, byte by byte to an array
// If a CR is received then Set a flag that alerts the system the buffer is ready and reset the msg array
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (msgIndex == 0)
	{
		memset(&uartMsgBuf, 0, sizeof(uartMsgBuf));
	}
	if (uartMsgData[0] != 13)  // Carriage Return
	{
		uartMsgBuf[msgIndex++] =  uartMsgData[0];
	}
	else
	{
		uartMsgBuf[msgIndex++] = uartMsgData[0];
		msgIndex = 0;
		msgRdyFlag = 1;
 	}

	// Enable IRQ again
	HAL_UART_Receive_IT(&huart6, uartMsgData, 1);

}


// This is the init portion for the task StartTaskAnalogInput
// Start the ADC in DMA - start the timer for use in IRQ - DMA mode
void AnalogTaskInit()
{
	HAL_ADC_Start_DMA(&hadc1, &ADC_ReadValue, 10);
	HAL_TIM_Base_Start_IT(&htim1);
}

// This is body of task StartTaskAnalogInput
// TBD
void AnalogTaskPoll()
{
	osEvent AnalogEvent;
	AnalogEvent = osSignalWait((BIT_0 | BIT_1), osWaitForever);
	// NOTE BASED ON EXPIRMENTS IT LOOKS LIKE THE SIGNAL CLEARS ITSELF - BUT NOT TOTTALY SURE / READ MORE
	if (AnalogEvent.value.signals == (BIT_0 | BIT_1))
	{
		uint32_t Sum = 0;
		for (uint8_t Index = 0; Index < 10; Index++)
		{
			Sum += ADC_ReadValue[Index];
		}
		Mean = Sum / 10;
		if (PresentScreen == SCREEN_3)
		{
			ADC_UpddateDisplay = 1;
			osDelay(25);
		}
	}

}

// This is the IRQ Callback function for the ADC.  It is defined as weak in HAL Lib.
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (PresentScreen != SCREEN_3)
		return;
	// NOTE: this is here because I could not set the signal directly from the ISR for some reason I think related to IRQ priority
	Flag1 = 1;
}


void defaultTaskPoll()
{

	if (PresentScreen == SCREEN_3)
	{
		osSignalSet(taskAnalogInputHandle, (BIT_0 | BIT_1));
	}
}
