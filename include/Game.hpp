#pragma once


#include "GameWrapper.hpp"
#include "utils.h"

#include <iostream>
#include <algorithm>

class Game : public GameWrapper 
{
public:
    Game(bool render=true) : GameWrapper(render),
        circle_radius(20),
        player(circle_radius),
        dest(circle_radius),
        speed(350),
        player_speed(0, 0)
    {
        player.setOrigin(circle_radius, circle_radius);
        player.setFillColor(sf::Color::Red);
        player.setOutlineThickness(2);
        player.setOutlineColor(sf::Color::Black);

        dest.setOrigin(circle_radius, circle_radius);
        dest.setFillColor(sf::Color::Cyan);
        dest.setOutlineThickness(2);
        dest.setOutlineColor(sf::Color::Black);

        default_background_color = sf::Color::White;
    };

    void init() override {
        auto rand_pos = [=](){
            return rand_int(circle_radius, WIN_SIZE - circle_radius);
        };
        player_pos.x = rand_pos();
        player_pos.y = rand_pos();
        do {
            dest_pos.x = rand_pos();
            dest_pos.y = rand_pos();
        }
        while (dist(player_pos, dest_pos) < WIN_SIZE / 3);
        player.setPosition(player_pos);
        dest.setPosition(dest_pos);
        player_speed = { 0, 0 };
    }

    void key_pressed(sf::Keyboard::Key key) override {
        switch(key) {
            case sf::Keyboard::Up: case sf::Keyboard::Z:
                player_speed = { 0, - speed }; break;
            case sf::Keyboard::Left: case sf::Keyboard::Q:
                player_speed = { - speed, 0 }; break;
            case sf::Keyboard::Down: case sf::Keyboard::S:
                player_speed = { 0, speed }; break;
            case sf::Keyboard::Right: case sf::Keyboard::D:
                player_speed = { speed, 0 }; break;
            default: break;
        }
    }

    bool is_over() override {
        return dist(player_pos, dest_pos) < circle_radius * 2;
    }

private:
    void update(float dt) override {
        std::cout << 1.0 / dt << " fps - " << dt << "\n";
        player_pos += player_speed * dt;

        // make sure the player stays within the screen
        player_pos.x = std::clamp<float>(player_pos.x, 0, WIN_SIZE);
        player_pos.y = std::clamp<float>(player_pos.y, 0, WIN_SIZE);

        std::cout << player_pos.x << " " << player_pos.y << "\n";

        player.setPosition(player_pos);
    }

    void draw(sf::RenderTarget* target) override {
        target->draw(dest);
        target->draw(player);
    }

private:
    const float circle_radius;

    sf::CircleShape player;
    sf::CircleShape dest;

    sf::Vector2f player_pos;
    sf::Vector2f dest_pos;

    float speed;
    sf::Vector2f player_speed;
};