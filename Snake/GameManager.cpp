#include "GameManager.h"

#include <algorithm>
#include <SDL.h>
#include <SDL_keyboard.h>

#include "Collision.h"
#include "Food.h"
#include "GameBoard.h"
#include "Player.h"

GameManager::GameManager()
{
	keys_ = SDL_GetKeyboardState(&num_keys_);
}

GameManager& GameManager::get_instance()
{
	static GameManager instance;
	return instance;
}

bool GameManager::is_key_down(const unsigned int key) const
{
	return keys_[key];
}

void GameManager::init()
{
	// Start game
	is_running_ = true;
	is_playing_ = false;
}

void GameManager::update()
{
	if(is_playing_)
	{
		// Wait for Collision-thread to acquire lock
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
		std::lock_guard<std::mutex> lock(mtx_);
		
		auto& player = Player::get_instance();
		auto& game_board = GameBoard::get_instance();
		
		if (player.is_alive())
		{
			const auto frame_time = SDL_GetTicks();
			// Get input
			SDL_PumpEvents();
			player_input();

			// Update game board
			game_board.update();

			const auto frame_elapsed_time = SDL_GetTicks() - frame_time;
			if (ticks_per_frame_ > frame_elapsed_time)
			{
				// Delay for game FPS
				fps_delay_ = ticks_per_frame_ - frame_elapsed_time;
				SDL_Delay(fps_delay_);
			}
		}
		else
		{
			// Delay when player dies
			SDL_Delay(500);

			// Clean board
			game_board.clean();

			if(player.get_lives_left() > 0)
			{
				// Reset game
				setup_game();
			}
			else
			{
				// Back to start screen
				is_playing_ = false;
			}
		}
	}
	else
	{
		// Get input
		SDL_PumpEvents();
		player_input();

		auto& game_board = GameBoard::get_instance();
		if(game_board.is_init())
		{
			game_board.update();
		}
		else
		{
			is_running_ = false;
		}
	}
}

void GameManager::player_input()
{
	// ESC Exit
	if (is_key_down(SDL_SCANCODE_ESCAPE) != 0)
	{
		is_running_ = false;
	}
	
	// Window button Exit
	if (SDL_HasEvent(SDL_QUIT))
	{
		is_running_ = false;
	}

	if(is_playing_)
	{
		auto& player = Player::get_instance();
		
		// Move inputs
		if (is_key_down(SDL_SCANCODE_W) != 0)
		{
			player.set_direction(Direction::up);
		}
		else if (is_key_down(SDL_SCANCODE_S) != 0)
		{
			player.set_direction(Direction::down);
		}
		else if (is_key_down(SDL_SCANCODE_D) != 0)
		{
			player.set_direction(Direction::right);
		}
		else if (is_key_down(SDL_SCANCODE_A) != 0)
		{
			player.set_direction(Direction::left);
		}
	}
	else
	{
		// Start screen
		if (is_key_down(SDL_SCANCODE_1) != 0)
		{
			// New game
			setup_game();
		}

		if (is_key_down(SDL_SCANCODE_2) != 0)
		{
			// Exit game
			is_playing_ = false;
			is_running_ = false;
		}
	}
}

void GameManager::setup_game()
{
	auto& game_board = GameBoard::get_instance();

	// Set up game board
	game_board.init();

	if (game_board.is_init())
	{
		is_playing_ = true;

		if (game_threads_.empty())
		{
			start_threads();
		}
	}
	else
	{
		// Exit game on init failure
		is_playing_ = false;
		is_running_ = false;
	}
}

void GameManager::clean()
{
	auto& renderer = Renderer::get_instance();
	
	renderer.clean();

	// Join all threads on game exit
	std::for_each(game_threads_.begin(), game_threads_.end(), [](std::thread& thread) { thread.join(); });
}

void GameManager::start_threads()
{
	auto& player = Player::get_instance();
	auto& collision = Collision::get_instance();
	
	game_threads_.emplace_back(std::thread([&player, &collision, this]
	{
		while(is_running())
		{
			// Delay waiting for game to start
			SDL_Delay(50);
			
			while (player.is_alive() && is_running())
			{
				// Wait for Main-thread to acquire lock
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(2ms);
				std::lock_guard<std::mutex> lock(mtx_);

				collision.update();
			}
		}
	}));
}
