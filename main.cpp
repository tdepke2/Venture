#include "Game.h"

using namespace std;

int main() {
    Game::start();
    
    return 0;
}

/*
Notes:
    * May need to reconsider using static foreground/background colors on tiles and entities and just use the dynamic ones instead.
    * Paths are now just two rows where each is a palatte of tiles/entities. First row is n floor tiles and second is m wall tiles, level is max(n, m) by 2.
*/