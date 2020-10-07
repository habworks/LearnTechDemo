#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

// Hab this file edited from original

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::uartMsgRdy()
{
	view.uartMsgRdy();
}


// Hab functions to take care of the GUI passsing information to hardware
void Screen1Presenter::buttonClicked(int button)
{
	model->screen1ButtonClicked(button);
}
