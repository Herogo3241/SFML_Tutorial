#include <SFML/Graphics.hpp>
#include "Game.h"
#include "MainMenu.h"
#include "GameOverScreen.h"

int main() {
    
    Game game;
    GameOverScreen gameOverScreen(game.getWindow());
    MainMenu mainMenu(game.getWindow());

    enum class GameState { MENU, PLAYING, GAME_OVER };
    GameState state = GameState::MENU;

	std::string stateString = "Main Menu";

    while (true) {
        switch (state) {
        case GameState::MENU:
            if (mainMenu.run(game.getWindow(), game, gameOverScreen)) {
                state = GameState::PLAYING;
                game.reset();
            }
            else {
				return 0; 
            }
            break;
        case GameState::PLAYING:
            stateString = "Playing";
            if (!game.run(stateString)) { // Game over
                stateString = "Game Over";
                state = GameState::GAME_OVER;
            }
            
            break;
        case GameState::GAME_OVER:
            if (gameOverScreen.run(game.getWindow(), game)) {
                state = GameState::MENU;
                game.reset(); // Reset the game before going back to the menu
            }
            else {

                return 0;
            }
            break;
        }
    }

    return 0;
}
