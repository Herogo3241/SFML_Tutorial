#include <SFML/Graphics.hpp>
#include "Game.h"
#include "MainMenu.h"
#include "GameOverScreen.h"

int main() {
    MainMenu mainMenu;
    Game game;
    GameOverScreen gameOverScreen;

    enum class GameState { MENU, PLAYING, GAME_OVER };
    GameState state = GameState::MENU;

    while (true) {
        switch (state) {
        case GameState::MENU:
            if (mainMenu.run(game.getWindow())) {
                state = GameState::PLAYING;
            }
            break;
        case GameState::PLAYING:
            if (!game.run()) { // Game over
                state = GameState::GAME_OVER;
            }
            break;
        case GameState::GAME_OVER:
            if (gameOverScreen.run(game.getWindow())) {
                state = GameState::MENU;
                game.reset(); // Reset the game before going back to the menu
            }
            break;
        }
    }

    return 0;
}
