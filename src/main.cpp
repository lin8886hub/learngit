#include "Game.h"


int main(int , char **) {
   Game& game = Game::getInstance();
  
   if(!game.init()) {
    std::cerr << "Failed to initialize the game!" << std::endl;
       return -1;
   }

    game.run();
    
    return 0;
}