#ifndef SNAKE_GAMEBOARD_H
#define SNAKE_GAMEBOARD_H
#include <memory>
#include <SDL_render.h>
#include <vector>

#include "SnakeNode.h"

class GameBoard
{
private:
	GameBoard();
	SDL_Texture* play_screen_texture_{ nullptr };
	SDL_Texture* start_screen_texture_{ nullptr };
	SDL_Texture* head_alive_texture_{ nullptr };
	SDL_Texture* head_dead_texture_{ nullptr };
	SDL_Texture* tree_texture_{ nullptr };
	std::vector<std::shared_ptr<SnakeNode>> tree_nodes_;
	bool is_init_;
	void render_game_background() const;
	void start_screen() const;
	void create_trees();
public:
	static GameBoard& get_instance(); // Get instance of this class
	GameBoard(GameBoard const&) = delete; // Delete copy_node constructor
	GameBoard(GameBoard const&&) = delete; // Delete copy_node constructor
	void operator=(GameBoard const&) = delete; // Delete copy_node assignment operator
	void operator=(GameBoard const&&) = delete; // Delete move assignment operator
	void init();
	void update() const;
	void clean();
	[[nodiscard]] std::vector<std::shared_ptr<SnakeNode>> get_trees() const { return tree_nodes_; };
	[[nodiscard]] bool is_init() const { return is_init_; }
};
#endif
