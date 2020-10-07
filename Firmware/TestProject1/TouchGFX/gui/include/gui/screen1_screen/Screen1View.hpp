#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP


// Hab this file edited from original

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // Uart added
    virtual void uartMsgRdy();
    // Screen 1 added functions
    virtual void toggleButton1Clicked();
    virtual void button2Clicked();
    virtual void updateScreen();
protected:
};

#endif // SCREEN1VIEW_HPP

