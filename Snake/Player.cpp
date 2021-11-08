#include "Player.h"
#include "Collision.h"
#include "Food.h"
#include "Renderer.h"

Player::Player()
{
	auto& renderer = Renderer::get_instance();
	head_alive_texture_ = renderer.create_texture("../images/headAlive.bmp");
	head_dead_texture_ = renderer.create_texture("../images/headDead.bmp");

	body_textures_.emplace_back(renderer.create_texture("../images/body01.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body02.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body03.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body04.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body05.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body06.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body07.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body08.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body09.bmp"));
	body_textures_.emplace_back(renderer.create_texture("../images/body10.bmp"));

	if (head_alive_texture_ == nullptr || head_dead_texture_ == nullptr)
	{
		is_init_ = false;
		return;
	}

	for (auto* texture : body_textures_)
	{
		if (texture == nullptr)
		{
			is_init_ = false;
			return;
		}
	}

	snake_width_ = 32;
	snake_height_ = 32;

	is_init_ = true;
}

void Player::init()
{
	auto& renderer = Renderer::get_instance();

	// Set size and starting position
	const auto start_pos_x = renderer.get_window_w() / 2; // Close to middle of board
	const auto start_pos_y = renderer.get_window_h() / 2; // Close to middle of board
	auto coords = SDL_Rect{ start_pos_x, start_pos_y, snake_width_, snake_height_ };

	length_since_last_node_ = get_node_size(); // Start with placing a node

	if(lives_left_ <= 0)
	{
		lives_left_ = 3;
	}

	// Set start variables
	move_speed_ = 192;
	body_textures_counter_ = 0;
	length_to_wait_until_moving_ = 0;
	head_rotation_ = 0;
	delta_time_ = 0;
	last_update_ = 0;
	direction_ = Direction::down;
	
	// Create head
	head_ = std::make_shared<SnakeNode>(head_alive_texture_, coords);

	// Create body
	for (auto i = 0; i < snake_start_length_; i++)
	{
		coords.y = start_pos_y - get_node_size() - get_node_size() * i; // Position nodes one after another
		body_.emplace_back(std::make_shared<SnakeNode>(body_textures_.at(body_textures_counter_), coords));
		body_textures_counter_++;
	}
	
	is_alive_ = true;
}


Player& Player::get_instance()
{
	static Player instance;
	return instance;
}

void Player::set_direction(const Direction new_direction)
{
	if (has_moved_)
	{
		if (check_direction(new_direction))
		{
			// Change direction if new direction is valid
			direction_ = new_direction;

			// Change texture rotation
			set_head_rotation(new_direction);

			// Place node at heads current position(node to fill corner when turning)
			body_.emplace_front(std::make_shared<SnakeNode>(body_textures_.at(body_textures_counter_), head_->current_position));

			// Save heads current position to previous position
			head_->previous_position = head_->current_position;
			
			// New node is added to snake
			length_since_last_node_ = 0;

			// Wait for next move
			has_moved_ = false;
		}
	}
}

bool Player::check_direction(const Direction new_direction) const
{
	// Check if new direction is valid
	switch (new_direction)
	{
	case Direction::up:
	case Direction::down:
		if (direction_ == Direction::up || direction_ == Direction::down)
		{
			// new direction is NOT valid
			return false;
		}
		break;
	case Direction::right:
	case Direction::left:
		if (direction_ == Direction::right || direction_ == Direction::left)
		{
			// new direction is NOT valid
			return false;
		}
		break;
	}
	// new direction is valid
	return true;
}

void Player::set_head_rotation(const Direction new_direction)
{
	switch (new_direction)
	{
	case Direction::down:
		head_rotation_ = 0;
		break;
	case Direction::left:
		head_rotation_ = 90;
		break;
	case Direction::up:
		head_rotation_ = 180;
		break;
	case Direction::right:
		head_rotation_= 270;
		break;
	}
}


double Player::delta_time()
{
	if (last_update_ == 0)
	{
		last_update_ = SDL_GetTicks();
	}

	const int get_time = SDL_GetTicks();
	delta_time_ = (get_time - last_update_) / 1000.0;
	last_update_ = get_time;
	return delta_time_;
}

void Player::move()
{
	// Calculate frame independent move length
	const int delta_move = round(move_speed_ * delta_time());

	move_head(delta_move);
	move_tail(delta_move);

	has_moved_ = true;
}

void Player::move_head(const int delta_move)
{
	switch (direction_)
	{
	case Direction::up:
		head_->current_position.y -= delta_move;
		break;
	case Direction::down:
		head_->current_position.y += delta_move;
		break;
	case Direction::right:
		head_->current_position.x += delta_move;
		break;
	case Direction::left:
		head_->current_position.x -= delta_move;
		break;
	}

	// Update total length
	length_since_last_node_ += delta_move;
	
	// Head has moved a full node in a straight line
	if (length_since_last_node_ >= get_node_size())
	{
		// Save heads current position to previous position
		head_->previous_position = head_->current_position;

		add_body_node();
	}
}

void Player::add_body_node()
{
	auto coords_first_body_node = get_body().at(0)->current_position;

	// Add new node directly after first node depending on direction
	switch (direction_)
	{
	case Direction::up:
		coords_first_body_node.y -= get_node_size();
		break;
	case Direction::down:
		coords_first_body_node.y += get_node_size();
		break;
	case Direction::right:
		coords_first_body_node.x += get_node_size();
		break;
	case Direction::left:
		coords_first_body_node.x -= get_node_size();
		break;
	}

	// Put in front of body_, behind head
	body_.emplace_front(std::make_shared<SnakeNode>(body_textures_.at(body_textures_counter_), coords_first_body_node));

	// Change texture of next body node
	body_textures_counter_++;

	if(body_textures_counter_ >= (body_textures_.size()))
	{
		// Reset when reached end of vector
		body_textures_counter_ = 0;
	}

	length_since_last_node_ -= get_node_size();
}

void Player::move_tail(const int delta_move)
{
	if(length_to_wait_until_moving_ > 0)
	{
		// Stop tail from moving after eating food
		length_to_wait_until_moving_ -= delta_move;
		return;
	}
	
	// Move 1 pixel for each delta move
	for (auto i = 0; i < delta_move; i++)
	{
		auto& tail = body_.back()->current_position; // Tail node
		auto& second_last_node = body_.at(body_.size() - 2)->current_position; // Second last node
		
		if (tail.y > second_last_node.y) // Tail is below
		{
			tail.y --; //Move up
		}

		if (tail.y < second_last_node.y) // Tail is above
		{
			tail.y ++; // Move down
		}

		if (tail.x > second_last_node.x) // Tail is to the right
		{
			tail.x --; // Move left
		}

		if (tail.x < second_last_node.x) // Tail is to the left
		{
			tail.x ++; // Move right
		}

		// Tail-node has caught up to second last node
		if (SDL_RectEquals(&tail, &second_last_node))
		{
			body_.pop_back();
		}
	}
}

void Player::render() const
{
	auto& renderer = Renderer::get_instance();
	for (const auto& body : get_body())
	{
		renderer.copy_node(body, 0);
	}
	
	renderer.copy_node(head_, head_rotation_);
}

void Player::update()
{
	collisions();
	if(is_alive())
	{
		move();
	}
	render();
}

void Player::collisions()
{
	auto& collider = Collision::get_instance();
	if (collider.food_hit())
	{
		const auto food_type = collider.food_hit_type();
		if(food_type == 0)
		{
			// Red apple eaten -> Increase speed + add one node
			move_speed_ += 32;
			length_to_wait_until_moving_ += get_node_size();
		}
		else if(food_type == 1)
		{
			// Green apple eaten -> add two nodes
			length_to_wait_until_moving_ += get_node_size() *2;
		}
		else
		{
			// Yellow apple eaten -> add three nodes
			length_to_wait_until_moving_ += get_node_size() * 3;
		}
	}
	
	if (collider.snake_hit() || collider.wall_hit() || collider.tree_hit())
	{
		get_head()->texture = head_dead_texture_;
		lives_left_--;
		is_alive_ = false;
	}
}

void Player::clean()
{
	body_.clear();
	head_ = nullptr;
}
