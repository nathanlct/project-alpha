#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include "Game.hpp"
#include "GameMaze.hpp"

int main() {
    bool render = true; // whether or not to display the game window
    Game game(render);

    do {
        game.init();
        do {
            // get observation
            auto state = game.get_state();

            // send action(s)
            // game.key_pressed(sf::Keyboard::Right);

            // execute one game step
            game.step();

            // compute reward
            float reward = -1;            
        }
        while(game.is_running() && !game.is_over());
    }
    while(game.is_running());
}