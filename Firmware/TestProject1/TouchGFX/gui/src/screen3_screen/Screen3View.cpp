#include <gui/screen3_screen/Screen3View.hpp>
#include "string.h"
#include "PollingRoutine.h"

extern Type_PresentScreen PresentScreen;

Screen3View::Screen3View()
{

}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}


void Screen3View::analogUpdate(float value)
{
	memset(&textAreaADCBuffer, 0, TEXTAREAADC_SIZE);
	Unicode::snprintfFloat(textAreaADCBuffer, sizeof(textAreaADCBuffer), "%.3f", (value * 805.7e-6));
	textAreaADC.invalidate();
}


// Set the viewing state for Screen 3 upon entry to the screen
// Use of global vars to transfer states
void Screen3View::updateScreen()
{
	PresentScreen = SCREEN_3;
}
