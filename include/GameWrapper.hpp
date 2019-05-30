#pragma once


#include <SFML/Window.hpp>

#include <vector>
#include <iostream>

#include <chrono>
#include <thread>


constexpr uint WIN_SIZE = 1024;


class GameWrapper
{
public:
    GameWrapper(bool render=true) : render(render), clock()
    {
        if (render) {
            sf::ContextSettings settings;
            settings.antialiasingLevel = 8;
            sf::RenderWindow* window_tmp = new sf::RenderWindow
                (sf::VideoMode(WIN_SIZE, WIN_SIZE), 
                 "Project Alpha", 
                 sf::Style::Titlebar | sf::Style::Close,
                 settings);
            window = window_tmp;
        }
        texture = new sf::RenderTexture;
        texture->create(WIN_SIZE, WIN_SIZE);
    }

    virtual void init() {};
    virtual bool is_over() = 0;

    void step() {
        if(render) {
            window->clear(default_background_color);
            sf::Event event;
            while(window->pollEvent(event))
            {
                switch(event.type) {
                    case sf::Event::Closed:
                        window->close();
                        break;
                    case sf::Event::KeyPressed:
                        if(render) {
                            key_pressed(event.key.code);
                        }
                        break;
                    default:
                        break;
                }
            }
            draw(window);
            window->display();
        }
        texture->clear(default_background_color);
        draw(texture);
        
        if (!is_over()) {
            update(clock.restart().asSeconds());
        }
    }

    std::vector<unsigned char> get_state() {
        sf::Image img = texture->getTexture().copyToImage();
        uint img_sz = img.getSize().x * img.getSize().y * 4;
        const unsigned char* img_start = img.getPixelsPtr();
        std::vector<unsigned char> state(img_start, img_start + img_sz);
        return state;
    }

    virtual void key_pressed(sf::Keyboard::Key key) {};

    bool is_running() {
        return render ? window->isOpen() : true;
    }

private:
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget* target) = 0;

protected:
    sf::Color default_background_color = sf::Color::White;

private:
    sf::RenderWindow* window;
    sf::RenderTexture* texture;
    bool render;
    sf::Clock clock;
};