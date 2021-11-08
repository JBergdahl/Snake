#ifndef SNAKE_GAMEMANAGER_H
#define SNAKE_GAMEMANAGER_H

#include <mutex>
#include <SDL_stdinc.h>
#include <thread>

#include "Renderer.h"
class GameManager
{
private:
	GameManager(); // Only this class can create itself
	const Uint8* keys_{ nullptr };
	int num_keys_{};
	bool is_running_{ false };
	bool is_playing_{ false };
	int frames_per_second_{ 120 };
	int ticks_per_frame_{ 1000 / frames_per_second_ };
	int fps_delay_{ 15 };
	std::vector<std::thread> game_threads_{};
	std::mutex mtx_;
	void start_threads();
	void player_input();
	void setup_game();
	[[nodiscard]] bool is_key_down(unsigned int key) const;
public:
	static GameManager& get_instance(); // Get instance of this class
	GameManager(GameManager const&) = delete; // Delete copy_node constructor
	GameManager(GameManager const&&) = delete; // Delete move constructor
	void operator=(GameManager const&) = delete; // Delete copy_node assignment operator
	void operator=(GameManager const&&) = delete; // Delete move assignment operator
	void init();
	void update();
	void clean();
	[[nodiscard]] bool play_game() const { return is_playing_; }
	[[nodiscard]] bool is_running() const { return is_running_; }
};

#endif