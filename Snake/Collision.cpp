#include "Collision.h"

#include "Food.h"
#include "GameBoard.h"
#include "Player.h"
#include "Renderer.h"

Collision& Collision::get_instance()
{
	static Collision instance;
	return instance;
}

void Collision::init()
{
	food_hit_ = false;
	snake_hit_ = false;
	wall_hit_ = false;
	tree_hit_ = false;
}

void Collision::update()
{
	auto& player = Player::get_instance();
	if (player.is_alive())
	{
		food_collision();
		snake_collision();
		wall_collision();
		tree_collision();
	}
}

void Collision::food_collision()
{
	auto& food = Food::get_instance();
	auto& player = Player::get_instance();
	
	// Head is colliding with food node
	if (check_collision(player.get_head()->current_position, food.get_position()))
	{
		// Hit
		food_hit_ = true;
		food_hit_type_ = food.get_current_food_type();
		
		auto new_position = false;
		while (!new_position)
		{
			food.set_random_position_and_type();
			new_position = true;

			for (const auto& body : player.get_body())
			{
				if (check_collision(body->current_position, food.get_position()) || check_collision(player.get_head()->current_position, food.get_position()))
				{
					// Restart loop if new position is inside snake body or head
					new_position = false;
					break;
				}
			}
		}
	}
	else
	{
		// No hit
		food_hit_ = false;
	}
}

void Collision::snake_collision()
{
	auto& player = Player::get_instance();
	
	for (const auto& body : player.get_body())
	{
		// Check if head is colliding with body
		if (check_collision(player.get_head()->current_position, body->current_position))
		{
			// Snake heads prev position == second nodes current position -> ignore first node in body
			// This node is placed on head position
			if (!check_collision(player.get_head()->previous_position, body->current_position))
			{
				// Hit
				snake_hit_ = true;
				break;
			}

			// No hit
			snake_hit_ = false;
		}
	}
}

void Collision::wall_collision()
{
	auto& player = Player::get_instance();
	auto& renderer = Renderer::get_instance();
	
	// If snake is NOT inside window
	const auto node_size = player.get_node_size();
	const SDL_Rect window{ node_size,node_size*2, renderer.get_window_w() - node_size*2, renderer.get_window_h() - node_size*3 };
	if (!check_collision(player.get_head()->current_position, window))
	{
		// Outside
		wall_hit_ = true;
	}
	else
	{
		// Inside
		wall_hit_ = false;
	}
}

void Collision::tree_collision()
{
	auto& player = Player::get_instance();
	auto& game_board = GameBoard::get_instance();
	
	for (const auto& tree : game_board.get_trees())
	{
		if(check_collision(player.get_head()->current_position, tree->current_position))
		{
			// If snake collides with a tree
			tree_hit_ = true;
			break;
		}
		tree_hit_ = false;
	}
}

bool Collision::check_collision(const SDL_Rect& a, const SDL_Rect& b)
{
	// a-node is completely ABOVE b-node
	if (a.y + a.h <= b.y)
	{
		return false;
	}

	// a-node is completely BELOW b-node
	if (a.y >= b.y + b.h)
	{
		return false;
	}

	// a-node is completely to the LEFT of b-node
	if (a.x + a.w <= b.x)
	{
		return false;
	}

	// a-node is completely to the RIGHT of b-node
	if (a.x >= b.x + b.w)
	{
		return false;
	}

	// a-node and b-node are touching
	return true;
}
