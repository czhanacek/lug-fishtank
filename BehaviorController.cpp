#include "BehaviorController.hpp"

BehaviorController::BehaviorController() {

}

void BehaviorController::visit(Fish & fish) {
    std::cout << "Visited normal fish" << std::endl;
}

void BehaviorController::visit(BackgroundWater & water) {
    std::cout << "Visited background water" << std::endl;
}
void BehaviorController::visit(WebcamPassthrough & water) {
    std::cout << "Visited webcam passthrough" << std::endl;
}

void BehaviorController::visit(PriDrawable & drawable) {
    std::cout << "Visited generic drawable" << std::endl;
}
