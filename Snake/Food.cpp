#include "Food.h"

#include <ctime>

#include "Collision.h"
#include "GameBoard.h"
#include "Renderer.h"

Food::Food()
{
	// Random seed
	srand(static_cast<unsigned int>(time(nullptr)));
	
	auto& renderer = Renderer::get_instance();

	// Create food node
	auto coords = SDL_Rect{ 0,0, 32, 32 };
	food_red_ = renderer.create_texture("../images/foodRed.bmp");
	food_green_ = renderer.create_texture("../images/foodGreen.bmp");
	food_yellow_ = renderer.create_texture("../images/foodYellow.bmp");

	if(food_red_ == nullptr || food_green_ == nullptr || food_yellow_ == nullptr)
	{
		is_init_ = false;
		return;
	}

	food_nodes_.emplace_back(std::make_shared<SnakeNode>(food_red_, coords));
	food_nodes_.emplace_back(std::make_shared<SnakeNode>(food_green_, coords));
	food_nodes_.emplace_back(std::make_shared<SnakeNode>(food_yellow_, coords));

	is_init_ = true;
}

Food& Food::get_instance()
{
	static Food instance;
	return instance;
}

void Food::update() const
{
	render();
}

void Food::render() const
{
	auto& renderer = Renderer::get_instance();
	renderer.copy_node(food_nodes_[current_food_type_]);
}

void Food::set_random_position_and_type()
{
	auto new_position = false;
	while (!new_position)
	{
		new_position = true;

		// Random type
		current_food_type_ = rand() % 3;

		// Random position in window
		food_nodes_[current_food_type_]->current_position.x = (rand() % 20) * 32;
		food_nodes_[current_food_type_]->current_position.y = (rand() % 15) * 32 + 32;
		
		auto& game_board = GameBoard::get_instance();
		for (const auto& tree : game_board.get_trees())
		{
			if (SDL_RectEquals(&tree->current_position, &food_nodes_[current_food_type_]->current_position))
			{
				// Restart loop if new position is inside a tree
				new_position = false;
			}
		}
	}
}