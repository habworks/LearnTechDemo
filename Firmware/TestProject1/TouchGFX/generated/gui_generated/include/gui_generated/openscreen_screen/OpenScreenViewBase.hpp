/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef OPENSCREENVIEWBASE_HPP
#define OPENSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/openscreen_screen/OpenScreenPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class OpenScreenViewBase : public touchgfx::View<OpenScreenPresenter>
{
public:
    OpenScreenViewBase();
    virtual ~OpenScreenViewBase() {}
    virtual void setupScreen();
    virtual void handleTickEvent();
    virtual void afterTransition();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Image image1;
    touchgfx::FadeAnimator< touchgfx::ScalableImage > scalableImage1;

private:

    /*
     * Delay Variable Declarations
     */
    static const uint16_t INTERACTION2_DURATION = 30;
    uint16_t interaction2Counter;
    /*
     * Interaction Callback Declarations
     */
    touchgfx::Callback < OpenScreenViewBase, const touchgfx::FadeAnimator<touchgfx::ScalableImage>& >  interaction1EndedCallback;


    /*
     * Interaction Handlers
     */
    void interaction1EndedCallbackHandler(const touchgfx::FadeAnimator<touchgfx::ScalableImage>& comp);

};

#endif // OPENSCREENVIEWBASE_HPP
