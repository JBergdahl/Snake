#ifndef SNAKE_FOOD_H
#define SNAKE_FOOD_H

#include <memory>
#include <vector>

#include "SnakeNode.h"

class Food
{
private:
	Food(); // Only this class can create itself
	std::vector<std::shared_ptr<SnakeNode>> food_nodes_;
	SDL_Texture* food_green_{ nullptr };
	SDL_Texture* food_red_{ nullptr };
	SDL_Texture* food_yellow_{ nullptr };
	int current_food_type_{ 0 };
	bool is_init_;
	void render() const;
public:
	static Food& get_instance(); // Get instance of this class
	Food(Food const&) = delete; // Delete copy_node constructor
	Food(Food const&&) = delete; // Delete copy_node constructor
	void operator=(Food const&) = delete; // Delete copy_node assignment operator
	void operator=(Food const&&) = delete; // Delete move assignment operator
	[[nodiscard]] SDL_Rect get_position() const { return food_nodes_[current_food_type_]->current_position; }
	[[nodiscard]] bool is_init() const { return is_init_; }
	[[nodiscard]] int get_current_food_type() const { return current_food_type_; }
	void set_random_position_and_type();
	void update() const;
};

#endif
