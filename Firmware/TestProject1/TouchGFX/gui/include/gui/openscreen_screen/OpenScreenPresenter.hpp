#ifndef OPENSCREENPRESENTER_HPP
#define OPENSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class OpenScreenView;

class OpenScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    OpenScreenPresenter(OpenScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~OpenScreenPresenter() {};

private:
    OpenScreenPresenter();

    OpenScreenView& view;
};

#endif // OPENSCREENPRESENTER_HPP
