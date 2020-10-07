#include <gui/screen2_screen/Screen2View.hpp>


// Hab this file was created - I am editing it here
#include "PollingRoutine.h"
extern ButtonClicked screen1ButtonClicked;
extern ButtonClicked screen2ButtonClicked;

extern Type_PresentScreen PresentScreen;

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}



/******************** SCREEN ACTIONS *************************/

// ACTION FLOW GUI >> VIEW >> PRESENTER >> MODEL
// THIS IS VIEW
// The toggle button was pressed.  Do 2 things
//   1. Depending on the state of global var for Screen 1 button Enable or Disable Click
//   2. Call a function in the presenter to take care of button clicks on screen 1
// This action is only possible if you are in screen 1
// Note: Passing information from the GUI to the hardware is much easire than hardware to GUI.
// From the corresponding view function load a global var(s) - you can operate on these vars later to make things happen
// Probably best not to take action here - but I am not sure you can not.
void Screen2View::toggleButton1Clicked()
{
	if (screen1ButtonClicked.Status.button1 == 0)
	{
		toggleButton1.setTouchable(false);
	}
	else
	{
		toggleButton1.setTouchable(true);
	}
	screen2ButtonClicked.Status.button1 = toggleButton1.getState();
	presenter->buttonClicked(1);		//1 represents button 2 for example - this should be an enum
}


// ACTION FLOW GUI >> VIEW >> PRESENTER >> MODEL
// THIS IS VIEW
// The Tx button was pressed.  Call a function in the presenter to take care of button clicks on screen 2
// This action is only possible if you are in screen 2
void Screen2View::button2Clicked()
{
	presenter->buttonClicked(2);		//2 represents button 2 for example - this should be an enum
}


// Set the viewing state for Screen 2 upon entry to the screen
// Use of global vars to transfer states
void Screen2View::updateScreen()
{
	toggleButton1.forceState(screen1ButtonClicked.Status.button1);
	toggleButton1.invalidate();
	PresentScreen = SCREEN_2;
}
