#ifndef OPENSCREENVIEW_HPP
#define OPENSCREENVIEW_HPP

#include <gui_generated/openscreen_screen/OpenScreenViewBase.hpp>
#include <gui/openscreen_screen/OpenScreenPresenter.hpp>

class OpenScreenView : public OpenScreenViewBase
{
public:
    OpenScreenView();
    virtual ~OpenScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // OPENSCREENVIEW_HPP
