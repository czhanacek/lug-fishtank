#include "PriDrawable.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <string>
#include <thread>
#include <mutex>

class BackgroundWater : public PriDrawable {
    private:
    int width, height;
    std::string name; 
    std::mutex mtx;
    public:
    BackgroundWater(int width, int height) : PriDrawable(0) {
        this->width = width;
        this->height = height;
        name = "water";
        ticks = 0;
        populateFrames(10);
    }

    void populateFrames(int frames) {
        for(int i = 0; i < frames; i++) {
            std::cout << "Generating frame " << i << " of " << frames << std::endl;
            std::cout << "texture size is " << textures.size() << std::endl; 
            std::thread textureThread(&BackgroundWater::addFrameToTextures, this);
            textureThread.detach();
        }
        //resetToDefaultTexture();
    }

    virtual void access(BehaviorController &behave) {
        behave.visit((*this));
    }

    void addFrameToTextures() {
        mtx.lock();
        auto newBackground = regenerate();
        textures.push_back(newBackground);
        mtx.unlock();
    }

    void animate() {
        if(ticks == 10) {
            ticks = 0;
            if(textures.size() > currentTextureIndex) {
                setTexture(*textures[currentTextureIndex]);
                currentTextureIndex = (currentTextureIndex + 1) % 10;
            }
        }
        else {
            ticks++;
        }
        
    }

    ~BackgroundWater() {
        std::cout << "DESTROYED!!!!" << std::endl;
    }

    sf::Texture * regenerate() {
        auto temp = new sf::Texture();
        sf::Image img;
        img.create(width, height, sf::Color::Black);
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_int_distribution<int> dist(10, 150);
        sf::Color currentColor = getNewColor();
        for(int y = 0; y < height; y++) {
            int x = 0;
            while(x < width) {
                int iters = dist(e2);
                currentColor = getNewColor();
                for(int i = 0; i < iters; i++)  {
                    if(x + i < width) {
                        img.setPixel(x + i, y, currentColor);
                    }
                    
                }
                x += iters;
            }
        }
        
        temp->loadFromImage(img);
        return temp;
    }

    sf::Color getNewColor() {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_real_distribution<double> distribution(10, 25); // seemed to work well
        return sf::Color(0, (int)(220 + distribution(e2)) % 255, (int)(200 + distribution(e2)) % 255);
    }

};
