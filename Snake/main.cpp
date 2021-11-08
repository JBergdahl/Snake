#define SDL_MAIN_HANDLED
#include "GameManager.h"

int main()
{
	auto& game_manager = GameManager::get_instance();
	game_manager.init();
	
	while (game_manager.is_running())
	{
		game_manager.update();
	}
	SDL_Delay(500);
	game_manager.clean();

	return 0;
}
