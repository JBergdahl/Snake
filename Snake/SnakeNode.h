#ifndef SNAKE_SNAKENODE_H
#define SNAKE_SNAKENODE_H

#include <SDL.h>

class SnakeNode
{
public:
	SnakeNode(SDL_Texture* node_texture, SDL_Rect& node_coords);
	SnakeNode();
	SDL_Rect current_position{};
	SDL_Rect previous_position{};
	SDL_Texture* texture{ nullptr };
};
#endif

