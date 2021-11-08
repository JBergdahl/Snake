#include "GameBoard.h"

#include <ctime>

#include "Collision.h"
#include "Food.h"
#include "GameManager.h"
#include "Player.h"
#include "Renderer.h"

GameBoard::GameBoard()
{
	// Create textures
	auto& renderer = Renderer::get_instance();
	play_screen_texture_ = renderer.create_texture("../images/PlayBackground.bmp");
	start_screen_texture_ = renderer.create_texture("../images/StartBackground.bmp");
	head_alive_texture_ = renderer.create_texture("../images/headAlive.bmp");
	head_dead_texture_ = renderer.create_texture("../images/headDead.bmp");
	tree_texture_ = renderer.create_texture("../images/tree.bmp");

	if (play_screen_texture_ == nullptr || start_screen_texture_ == nullptr
		|| head_alive_texture_ == nullptr || head_dead_texture_ == nullptr || tree_texture_ == nullptr)
	{
		is_init_ = false;
		return;
	}

	is_init_ = true;
}

GameBoard& GameBoard::get_instance()
{
	static GameBoard instance;
	return instance;
}

void GameBoard::init()
{
	auto& collision = Collision::get_instance();
	auto& player = Player::get_instance();
	auto& food = Food::get_instance();
	
	create_trees();

	if(player.is_init())
	{
		player.init();
	}
	else
	{
		is_init_ = false;
	}

	collision.init();

	if(food.is_init())
	{
		food.set_random_position_and_type();
	}
	else
	{
		is_init_ = false;
	}
}

void GameBoard::create_trees()
{
	auto& renderer = Renderer::get_instance();
	
	srand(static_cast<unsigned int>(time(nullptr)));

	auto coords_tree = SDL_Rect{ 0,0,32,32 };
	
	// Add between 5 and 10 trees
	for (auto i = 0; i < (rand() % 5 + 5); i++)
	{
		auto new_position = false;
		while (!new_position)
		{
			new_position = true;
			const auto window_w = renderer.get_window_w() / 32 - 4;
			const auto window_h = renderer.get_window_h() / 32 - 4;
			// At least 2 nodes from all walls
			coords_tree.x = (rand() % window_w) * 32 + 64;
			coords_tree.y = (rand() % window_h) * 32 + 96;

			// Don't spawn in snakes start path
			while (coords_tree.x == renderer.get_window_w() / 2)
			{
				coords_tree.x = (rand() % window_w) * 32 + 64;
			}

			// Don't spawn a tree inside another tree
			if (!tree_nodes_.empty())
			{
				for (const auto& tree : get_trees())
				{
					if (SDL_RectEquals(&tree->current_position, &coords_tree))
					{
						new_position = false;
					}
				}
			}
		}

		tree_nodes_.emplace_back(std::make_shared<SnakeNode>(tree_texture_, coords_tree));
	}
}

void GameBoard::render_game_background() const
{
	auto& renderer = Renderer::get_instance();
	auto& player = Player::get_instance();

	// Background
	auto coords_background = SDL_Rect{ 0,0, renderer.get_window_w(), renderer.get_window_h() };
	renderer.copy(play_screen_texture_, coords_background);

	// Render trees
	for (const auto& tree : get_trees())
	{
		renderer.copy_node(tree, 0);
	}

	/* Render player lives left */
	// Right texture
	auto coords_lives = SDL_Rect{ renderer.get_window_w() - 48,0, 32, 32 };
	if (player.get_lives_left() < 1)
	{
		renderer.copy(head_dead_texture_, coords_lives);
	}
	else
	{
		renderer.copy(head_alive_texture_, coords_lives);
	}

	// Middle texture
	coords_lives.x -= 48;
	if (player.get_lives_left() < 2)
	{
		renderer.copy(head_dead_texture_, coords_lives);
	}
	else
	{
		renderer.copy(head_alive_texture_, coords_lives);
	}

	// Left texture
	coords_lives.x -= 48;
	if (player.get_lives_left() < 3)
	{
		renderer.copy(head_dead_texture_, coords_lives);
	}
	else
	{
		renderer.copy(head_alive_texture_, coords_lives);
	}
}

void GameBoard::start_screen() const
{
	auto& renderer = Renderer::get_instance();
	
	// Slow down thread
	SDL_Delay(50);

	// Update screen
	auto coords = SDL_Rect{ 0,0, renderer.get_window_w(), renderer.get_window_h() };
	renderer.copy(start_screen_texture_, coords);
}

void GameBoard::update() const
{
	auto& game_manager = GameManager::get_instance();
	auto& player = Player::get_instance();
	auto& food = Food::get_instance();
	auto& renderer = Renderer::get_instance();
	
	if(game_manager.play_game())
	{

		// Render background
		render_game_background();

		// Update nodes
		player.update();
		food.update();
	}
	else
	{
		start_screen();
	}

	// Render screen
	renderer.render();
}

void GameBoard::clean()
{
	auto& player = Player::get_instance();
	
	player.clean();
	tree_nodes_.clear();
}
