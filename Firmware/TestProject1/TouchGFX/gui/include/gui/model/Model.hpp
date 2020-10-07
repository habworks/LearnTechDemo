#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

// Hab this file edited from original
class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void uartMsgRdy();

    void screen1ButtonClicked(int button);
    void screen2ButtonClicked(int button);
    void analogUpdate();

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
