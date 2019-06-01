#pragma once


#include "GameWrapper.hpp"
#include "utils.h"

#include <iostream>
#include <algorithm>
#include <stack>
#include <deque>
#include <unordered_set>


int UP = 1 << 0;
int LEFT = 1 << 1;
int DOWN = 1 << 2;
int RIGHT = 1 << 3;


class GameMaze : public GameWrapper 
{
public:
    GameMaze(int maze_size=20, bool render=true) : GameWrapper(render),
        maze_size(maze_size),
        cell_size(WIN_SIZE / maze_size),
        circle_radius(cell_size / 3),
        player(circle_radius),
        dest(circle_radius),
        maze(maze_size * maze_size),
        rng(rand_int(0, INT_MAX))
    {
        player.setOrigin(circle_radius, circle_radius);
        player.setFillColor(sf::Color::Red);
        player.setOutlineThickness(2);
        player.setOutlineColor(sf::Color::Black);

        dest.setOrigin(circle_radius, circle_radius);
        dest.setFillColor(sf::Color::Cyan);
        dest.setOutlineThickness(2);
        dest.setOutlineColor(sf::Color::Black);

        default_background_color = sf::Color::Black;
    };

    void init() override {
        auto rand_pos = [=](){
            return rand_int(0, maze_size - 1);
        };
        player_pos.x = rand_pos();
        player_pos.y = rand_pos();
        do {
            dest_pos.x = rand_pos();
            dest_pos.y = rand_pos();
        }
        while (dist(player_pos, dest_pos) < maze_size / 3);
        player.setPosition(get_real_pos(player_pos));
        dest.setPosition(get_real_pos(dest_pos));

        // generate maze
        std::fill(maze.begin(), maze.end(), 0);

        std::stack<int> stack;
        std::unordered_set<int> visited;
        int starting_pos = player_pos.y * maze_size + player_pos.x;
        stack.push(starting_pos);
        
        while (!stack.empty()) {
            int current = stack.top();
            visited.insert(current);

            std::vector<std::tuple<int,int,int>> neighbors = { std::make_tuple(current - maze_size, UP, DOWN),
                               std::make_tuple(current - 1, LEFT, RIGHT), 
                               std::make_tuple(current + maze_size, DOWN, UP), 
                               std::make_tuple(current + 1, RIGHT, LEFT) };
            std::shuffle(neighbors.begin(), neighbors.end(), rng);
            
            for (auto [neighbor, dir, opposite_dir] : neighbors) 
            {
                if (visited.count(neighbor) == 0
                   && neighbor >= 0 && neighbor < maze_size * maze_size
                   && neighbor % maze_size >= current % maze_size - 1
                   && neighbor % maze_size <= current % maze_size + 1)
                {
                    maze[current] |= dir;
                    maze[neighbor] |= opposite_dir;
                    stack.push(neighbor);
                    break;
                }
            }

            if (current == stack.top()) {
                stack.pop();
            }
        }
    }

    void key_pressed(sf::Keyboard::Key key) override {
        int pos = player_pos.y * maze_size + player_pos.x;
        switch (key) {
            case sf::Keyboard::Up: case sf::Keyboard::Z:
                if(maze[pos] & UP) player_pos.y -= 1; break;
            case sf::Keyboard::Left: case sf::Keyboard::Q:
                if(maze[pos] & LEFT) player_pos.x -= 1; break;
            case sf::Keyboard::Down: case sf::Keyboard::S:
                if(maze[pos] & DOWN) player_pos.y += 1; break;
            case sf::Keyboard::Right: case sf::Keyboard::D:
                if(maze[pos] & RIGHT) player_pos.x += 1; break;
            default: break;
        }
    }

    bool is_over() override {
        return player_pos == dest_pos;
    }

private:
    void update(float dt) override {
        player.setPosition(get_real_pos(player_pos));
        dest.setPosition(get_real_pos(dest_pos));        
    }

    void draw(sf::RenderTarget* target) override {
        sf::RectangleShape bg_cover({cell_size * maze_size, cell_size * maze_size});
        bg_cover.setFillColor(sf::Color::White);
        target->draw(bg_cover);

        target->draw(dest);
        target->draw(player);

        // draw maze
        float wall_thickness = 2.0;

        sf::RectangleShape wall_up({cell_size, wall_thickness});
        sf::RectangleShape wall_left({wall_thickness, cell_size});
        sf::RectangleShape wall_down({cell_size, wall_thickness});
        sf::RectangleShape wall_right({wall_thickness, cell_size});

        wall_up.setFillColor(sf::Color::Black);
        wall_left.setFillColor(sf::Color::Black);
        wall_down.setFillColor(sf::Color::Black);
        wall_right.setFillColor(sf::Color::Black);

        for (int col = 0; col < maze_size; ++col) {
            for (int row = 0; row < maze_size; ++row) {
                float x = col * cell_size;
                float y = row * cell_size;
                int pos = row * maze_size + col;

                if (!(maze[pos] & UP)) {
                    wall_up.setPosition(x, y);
                    target->draw(wall_up);
                }
                if (!(maze[pos] & LEFT)) {
                    wall_left.setPosition(x, y);
                    target->draw(wall_left);
                }
                if (!(maze[pos] & DOWN)) {
                    wall_down.setPosition(x, y + cell_size - wall_thickness);
                    target->draw(wall_down);
                }
                if (!(maze[pos] & RIGHT)) {
                    wall_right.setPosition(x + cell_size - wall_thickness, y);
                    target->draw(wall_right);
                }
            }
        }
    }

    sf::Vector2f get_real_pos(sf::Vector2u maze_pos) {
        sf::Vector2f pos = { (maze_pos.x + 0.5f) * cell_size,
                             (maze_pos.y + 0.5f) * cell_size };
        return pos;
    }

private:
    int maze_size;
    float cell_size;
    float circle_radius;

    sf::CircleShape player;
    sf::CircleShape dest;

    sf::Vector2u player_pos;
    sf::Vector2u dest_pos;

    std::vector<int> maze;
    std::default_random_engine rng;
};