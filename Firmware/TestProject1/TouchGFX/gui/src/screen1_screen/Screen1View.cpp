#include <gui/screen1_screen/Screen1View.hpp>

#include "string.h"
#include "PollingRoutine.h"

extern uint8_t uartMsgBuf[UART_BUF_SIZE];
extern ButtonClicked screen1ButtonClicked;

extern Type_PresentScreen PresentScreen;

// Hab this file edited from original

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::uartMsgRdy()
{

	if (uartMsgBuf[0] == 0) return;

	memset(&textArea2Buffer, 0, TEXTAREA2_SIZE);

	Unicode::strncpy(textArea2Buffer, (char *)uartMsgBuf, TEXTAREA2_SIZE - 1);
	textArea2Buffer[16] = '\0'; // make sure last index is null
	textArea2.invalidate();

}





/******************** SCREEN ACTIONS *************************/

// ACTION FLOW GUI >> VIEW >> PRESENTER >> MODEL
// THIS IS VIEW
// The toggle button was pressed.  Call a function in the presenter to take care of button clicks on screen 1
// This action is only possible if you are in screen 1
void Screen1View::toggleButton1Clicked()
{
	screen1ButtonClicked.Status.button1 = toggleButton1.getState();
	presenter->buttonClicked(1);		//1 represents button 2 for example - this should be an enum
}

// ACTION FLOW GUI >> VIEW >> PRESENTER >> MODEL
// THIS IS VIEW
// The Tx button was pressed.  Call a function in the presenter to take care of button clicks on screen 1
// This action is only possible if you are in screen 1
void Screen1View::button2Clicked()
{
    presenter->buttonClicked(2);		//2 represents button 2 for example - this should be an enum
}

// Set the viewing state for Screen 1 upon entry to the screen
// Use of global vars to transfer states
void Screen1View::updateScreen()
{
	toggleButton1.forceState(screen1ButtonClicked.Status.button1);
	toggleButton1.invalidate();
	PresentScreen = SCREEN_1;
}
