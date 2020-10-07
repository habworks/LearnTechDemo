/*
 * PollingRoutine.h
 * Hab Collector
 */

#ifndef _POLLINGROUTINE_H_
#define _POLLINGROUTINE_H_
#ifdef __cplusplus
extern"C" {
#endif



#define UART_BUF_SIZE	16
#define BIT_0	((uint8_t)(1<<0))
#define BIT_1 	((uint8_t)(1<<1))
#define BIT_2 	((uint8_t)(1<<2))
#define BIT_3 	((uint8_t)(1<<3))
#define BIT_4 	((uint8_t)(1<<4))
#define BIT_5 	((uint8_t)(1<<4))
#define BIT_6 	((uint8_t)(1<<5))
#define BIT_7 	((uint8_t)(1<<6))



void PollingInit();
void PollingRoutine();
void SendUartMsg(char *msg);
void AnalogTaskInit();
void AnalogTaskPoll();
void defaultTaskPoll();

typedef union
{
	struct
	{
		uint8_t	Byte;
	}Bytes;
	struct
	{
		unsigned button1:1;
		unsigned button2:1;
		unsigned button3:1;
		unsigned :5;
	}Status;
}ButtonClicked;


typedef enum
{
	OPEN_SCREEN = 0,
	SCREEN_1,
	SCREEN_2,
	SCREEN_3
}Type_PresentScreen;


#ifdef __cplusplus
}
#endif
#endif
