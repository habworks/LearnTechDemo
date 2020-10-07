#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

Screen2Presenter::Screen2Presenter(Screen2View& v)
    : view(v)
{

}

void Screen2Presenter::activate()
{

}

void Screen2Presenter::deactivate()
{

}


// Hab functions to take care of the GUI passsing information to hardware
void Screen2Presenter::buttonClicked(int button)
{
	model->screen2ButtonClicked(button);
}
