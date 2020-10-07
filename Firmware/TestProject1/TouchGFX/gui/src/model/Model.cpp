#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

// Hab this file edited from original
#include "cmsis_os.h"
#include "PollingRoutine.h"

extern osSemaphoreId binarySemUartMsgHandle;
extern osMessageQId queueScreen1Handle;
extern osMessageQId queueScreen2Handle;

extern osSemaphoreId binarySemAnalogHandle;
extern uint16_t ADC_ReadValue[10];
extern float Mean;
extern uint8_t ADC_UpddateDisplay;

extern Type_PresentScreen PresentScreen;

Model::Model() : modelListener(0)
{

}


// Hab this function was here but empty - you added the contents
// Within the model tick is where the GUI looks to make updates to all screens.
// It is how the hardware action is displayed to the screen.  Set actions for GUI updating to begin in here.
// The Model tick handles the GUI updating for all screens
void Model::tick()
{
	if (binarySemUartMsgHandle != NULL)  // Simple test to make sure the semaphore was created
	{
		if (xSemaphoreTake(binarySemUartMsgHandle, (TickType_t)10) == pdTRUE)
		{
			uartMsgRdy();
		}
	}

	if (ADC_UpddateDisplay == 1)
	{
		ADC_UpddateDisplay = 0;
		analogUpdate();
	}


/*
	if (binarySemAnalogHandle != NULL)
	{
		if (osSemaphoreWait(binarySemAnalogHandle, 100) == osOK)
		{
			analogUpdate();
		}
	}
*/
}

/* To display the uart msg to the GUI set the Model listener to call the Model presenter.
 * Within the ModelListner.hpp the function call is established for the ModelPresenter
 * The Model Presenter is specific to the screen that will be called.  Within the Model Presenter of said screen you must have the presenter function and prototype
 * See Screen1Presenter.cpp and .hpp for this function
 * The function Presenter calls the View function.  The View function is located in the Screen View for that specific GUI
 * See Screen1View.cpp and .hpp for this function
 * It is the Screen View function that actually handels the updating of the display
 * */
void Model::uartMsgRdy()
{
	modelListener->uartMsgRdy_Presenter();
}


// ACTION FLOW GUI >> VIEW >> PRESENTER >> MODEL
// THIS IS MODEL
// A button from screen 1 was pressed load the button type (1=toggle 2=Tx test) to the screen 1 queue
// This action is only possible if you are in screen 1
void Model::screen1ButtonClicked(int button)
{
	uint16_t item;
	item = button;
	xQueueSend(queueScreen1Handle, &item, (TickType_t)10);
}

// ACTION FLOW GUI >> VIEW >> PRESENTER >> MODEL
// THIS IS MODEL
// A button from screen 2 was pressed load the button type (1=toggle 2=Tx test) to the screen 2 queue
// This action is only possible if you are in screen 2
void Model::screen2ButtonClicked(int button)
{
	uint16_t item;
	item = button;
	xQueueSend(queueScreen2Handle, &item, (TickType_t)10);
}


/* To display the analog ADC voltage to the GUI set the Model listener to call the Model presenter.
 * Within the ModelListner.hpp the function call is established for the ModelPresenter
 * The Model Presenter is specific to the screen that will be called.  Within the Model Presenter of said screen you must have the presenter function and prototype
 * See Screen3Presenter.cpp and .hpp for this function
 * The function Presenter calls the View function.  The View function is located in the Screen View for that specific GUI
 * See Screen3View.cpp and .hpp for this function
 * It is the Screen View function that actually handles the updating of the display
 * */
void Model::analogUpdate()
{
	if (PresentScreen == SCREEN_3)
		modelListener->analogUpdate_Presenter(Mean);
}
