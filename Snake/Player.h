#ifndef SNAKE_PLAYER_H
#define SNAKE_PLAYER_H

#include <deque>
#include <memory>
#include <SDL.h>
#include <vector>
#include "SnakeNode.h"

enum class Direction {up, down, right, left};

class Player
{
private:
	Player();
	SDL_Texture* head_alive_texture_{ nullptr };
	SDL_Texture* head_dead_texture_{ nullptr };
	std::shared_ptr<SnakeNode> head_{ nullptr };
	std::deque<std::shared_ptr<SnakeNode>> body_;
	std::vector<SDL_Texture*> body_textures_;
	int lives_left_{ 0 };
	int body_textures_counter_{ 0 };
	int snake_start_length_{ 3 };
	bool is_alive_{ false };
	bool is_init_{ false };
	bool has_moved_{ false };
	int move_speed_{ 192 };
	int length_since_last_node_{ 0 };
	int length_to_wait_until_moving_{ 0 };
	int head_rotation_{ 0 };
	double delta_time_{ 0 };
	int last_update_{ 0 };
	int snake_height_{ 0 };
	int snake_width_{ 0 };
	Direction direction_{ Direction::down };
	void set_head_rotation(Direction new_direction);
	void render() const;
	void move();
	double delta_time();
	void move_head(int delta_move);
	void move_tail(int delta_move);
	void add_body_node();
	void collisions();
	[[nodiscard]] bool check_direction(Direction new_direction) const;
	
public:
	static Player& get_instance(); // Get instance of this class
	Player(Player const&) = delete; // Delete copy_node constructor
	Player(Player const&&) = delete; // Delete copy_node constructor
	void operator=(Player const&) = delete; // Delete copy_node assignment operator
	void operator=(Player const&&) = delete; // Delete move assignment operator
	void init();
	void set_direction(Direction new_direction);
	void update();
	void clean();
	[[nodiscard]] std::deque<std::shared_ptr<SnakeNode>> get_body() const { return body_; }
	[[nodiscard]] std::shared_ptr<SnakeNode> get_head() const { return head_; }
	[[nodiscard]] bool is_alive() const { return is_alive_; }
	[[nodiscard]] bool is_init() const { return is_init_; }
	[[nodiscard]] int get_node_size() const { return snake_height_; }
	[[nodiscard]] int get_lives_left() const { return lives_left_; }
};
#endif

