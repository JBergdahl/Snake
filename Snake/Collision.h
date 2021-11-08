#ifndef SNAKE_COLLISION_H
#define SNAKE_COLLISION_H

#include <SDL_rect.h>

class Collision
{
	Collision() = default;
	bool food_hit_{ false };
	bool snake_hit_{ false };
	bool wall_hit_{ false };
	bool tree_hit_{ false };
	int food_hit_type_{ 0 };
	void food_collision();
	void snake_collision();
	void wall_collision();
	void tree_collision();
	static bool check_collision(const SDL_Rect& a, const SDL_Rect& b);
public:
	static Collision& get_instance(); // Get instance of this class
	Collision(Collision const&) = delete; // Delete copy_node constructor
	Collision(Collision const&&) = delete; // Delete copy_node constructor
	void operator=(Collision const&) = delete; // Delete copy_node assignment operator
	void operator=(Collision const&&) = delete; // Delete move assignment operator
	[[nodiscard]] int food_hit_type() const { return food_hit_type_; }
	[[nodiscard]] bool food_hit() const { return food_hit_; }
	[[nodiscard]] bool snake_hit() const { return snake_hit_; }
	[[nodiscard]] bool wall_hit() const { return wall_hit_; }
	[[nodiscard]] bool tree_hit() const { return tree_hit_; }
	void init();
	void update();
};
#endif
