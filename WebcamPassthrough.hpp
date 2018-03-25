#include<opencv2/opencv.hpp>
#include <thread>
#include "PriDrawable.hpp"
#include <X11/Xlib.h> 
#include <queue>

class WebcamPassthrough : public PriDrawable {
    private:
    cv::VideoCapture cap;
    cv::Mat capture, capture2;
    
    int width, height;
    int counter;
    std::queue<cv::Mat> imgqueue;
    int buffer_length;
    int displayedTextureIndex;
    public:
    WebcamPassthrough(int width, int height) : PriDrawable(0) {
        cap = cv::VideoCapture(-1);
        this->width = width;
        buffer_length = 30;
        this->height = height;
        counter = 0;
        displayedTextureIndex = 0;
        currentTextureIndex = 0;
        if(!cap.isOpened()) {
            return;
        }

        cap.set(CV_CAP_PROP_FRAME_WIDTH,100);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,75);

        auto newthread = std::thread(&WebcamPassthrough::frameGrabberThread, this);
        newthread.detach();
    }

    void frameGrabberThread() {
        counter = 0;
        while(textures.size() < buffer_length) {
            sf::Texture * vidframe = new sf::Texture();
            sf::Image image;
            cap >> capture;
            cv::cvtColor(capture,capture2,cv::COLOR_BGR2RGBA); 
            image.create(capture2.cols, capture2.rows, capture2.ptr());
            vidframe->loadFromImage(image);
            textures.push_back(vidframe);
            std::cout << "added frame to buffer. size is now " << textures.size() << std::endl;

        }
        currentTextureIndex = 0;
        while(1) {
            cap >> capture;
            sf::Image image;
            cv::cvtColor(capture,capture2,cv::COLOR_BGR2RGBA); 
            //image.create(capture2.cols, capture2.rows, capture2.ptr());

            std::cout << "Gonna try and update..." << std::endl;
            textures[(currentTextureIndex - 10) % buffer_length]->update(capture2.ptr());
            std::cout << "updated buffer at index " << (currentTextureIndex - 10) % buffer_length << std::endl;

            currentTextureIndex = (currentTextureIndex + 1) % buffer_length;
            
        }
        
    }

    void animate() {

        // auto thing = sf::seconds(1);
        // sf::sleep(thing);
        //cv::resize(frameRGBA, frameRGBA, cv::Size(), 1, 1);
        if(textures.size() >= buffer_length && displayedTextureIndex != currentTextureIndex) {
            std::cout << "Set texture at " << currentTextureIndex << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            setTexture(*(textures[(currentTextureIndex) % buffer_length]));
            
            displayedTextureIndex = currentTextureIndex;
            float xscale = (float)width / 100;
            float yscale = (float)height / 75;
            
            setScale(xscale, yscale);
            setPosition(width / 2.0, height / 2.0);
            setOrigin(textures[currentTextureIndex]->getSize().x / 2, textures[currentTextureIndex]->getSize().y / 2);
            scale(-1, 1);
            
        }
      
    }
};
