/*
 * PollingRoutine.c
 * Hab Collector
 * Working:
 * 	UART6 in IRQ
 * 	ADC in DMA
 * 	RTC set time and get time (time is not accurate - but it does update)
 */

#include "main.h"
#include "PollingRoutine.h"
#include "string.h"
#include "cmsis_os.h"  // included for typedef of osSemaphoreId
#include "ff_gen_drv.h"
#include "sd_diskio.h"

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

// FOR RTC
extern RTC_HandleTypeDef hrtc;

// FOR FATFS
FATFS SDFatFs;
FIL MyFile;
char SDPath[4];
uint8_t workBuffer[2*_MAX_SS];
uint8_t FAT_Error = 0;


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
char TimeString[100];


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
	// Note about the clock.  The Asynchronous and Synchronous RTC clock divider must be configured such that they provide 1 Hz when applied to the RTC Clock
	// If the RTC Clock is set to 32.768KHz then the dividers should be set to 127 and 255 respectively (+1 to both)
	// See Figure 5 of ST document AN3371
	RTC_TimeTypeDef PresentTime;
	RTC_DateTypeDef PresentDate;
	uint16_t FirstRead;

	  FRESULT res;                                          /* FatFs function common result code */
	  uint32_t byteswritten, bytesread;                     /* File write/read counts */
	  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
	  uint8_t rtext[100];
	  uint8_t DstFileNamae[50];


	 static uint8_t DstFileNumber = 0;


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
				DstFileNumber++;
				sprintf(DstFileNamae,"0:DestinationFile_%d.txt",DstFileNumber);
				strcat(myString, permission);
				// ST stuff
				/*##-2- Register the file system object to the FatFs module ##############*/
				if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
				{
				  /* FatFs Initialization Error */
					FAT_Error = 1;
				  Error_Handler();
				}
				else
				{
					// Test 1
					/*##-1- Create and Open a new text file object with write access #####*/
					if(f_open(&MyFile, "0:STM32C.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
					{
					  /* 'STM32.TXT' file Open for write Error */
						FAT_Error = 3;
						Error_Handler();
					}

					/*##-2- Write data to the text file ################################*/
					res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
					if((byteswritten == 0) || (res != FR_OK))
					{
					/* 'STM32.TXT' file Write or EOF Error */
					  FAT_Error = 4;
					  Error_Handler();
					}

					/*##-6- Close the open text file #################################*/
					f_close(&MyFile);

					// Test 2
					// Copy Src To Dst File
					FIL Fsrc, Fdst;
					BYTE buffer[1000];
					UINT br, bw;
					if (f_open(&Fsrc, "0:SrcFile.txt", FA_READ) != FR_OK)
						Error_Handler();
					if (f_open(&Fdst, DstFileNamae, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
						Error_Handler();
					for (;;)
					{
						f_read(&Fsrc, buffer, sizeof buffer, &br);  /* Read a chunk of data from the source file */
						if (br == 0)
							break; /* error or eof */
						f_write(&Fdst, buffer, br, &bw);            /* Write it to the destination file */
						if (bw < br)
							break; /* error or disk full */
					}
					f_close(&Fsrc);
					f_close(&Fdst);

					// Format to print
					strcat(myString, helloFromST);
					strcat(myString, screen1);
				}
			}
			break;

		case 2:
			HAL_RTC_WaitForSynchro(&hrtc);
			do // This while loop is needed or the get time will not update correctly
			{
				FirstRead = PresentTime.SubSeconds;
				HAL_RTC_GetTime(&hrtc, &PresentTime, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &PresentDate, RTC_FORMAT_BIN);
			}while (FirstRead != PresentTime.SubSeconds);
			// Format to print time string
			memset(TimeString, 0, sizeof(TimeString));
			sprintf(TimeString,"Time: %02d : %02d : %02d\r\n",PresentTime.Hours, PresentTime.Minutes, PresentTime.Seconds);
			break;
		}
		SendUartMsg(myString);
		volatile uint8_t Len = strlen(TimeString);
		osDelay(25); // This delay is needed or second screen won print
		SendUartMsg(TimeString);
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
			PresentTime.Hours = 10;
			PresentTime.Minutes = 0;
			PresentTime.Seconds = 0;
			HAL_RTC_SetTime(&hrtc, &PresentTime, RTC_FORMAT_BIN);
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
		Flag1 = 0;
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

	if ((PresentScreen == SCREEN_3) && (Flag1 == 1))
	{
		osSignalSet(taskAnalogInputHandle, (BIT_0 | BIT_1));
	}
}
