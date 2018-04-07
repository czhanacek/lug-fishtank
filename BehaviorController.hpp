
#pragma once

#include <iostream>


class BackgroundWater;
class WebcamPassthrough;
class Fish;
class PriDrawable;

class BehaviorController {
    public:
    BehaviorController();
    virtual void visit(Fish & fish);
    virtual void visit(BackgroundWater & water);
    virtual void visit(WebcamPassthrough & water);
    virtual void visit(PriDrawable & drawable);
};
