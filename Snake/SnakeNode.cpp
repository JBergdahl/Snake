#include "SnakeNode.h"

SnakeNode::SnakeNode(SDL_Texture* node_texture, SDL_Rect& node_coords)
{
	texture = node_texture;
	current_position = node_coords;
	previous_position = node_coords;
}

SnakeNode::SnakeNode()
= default;
