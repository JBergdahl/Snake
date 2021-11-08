#include "Renderer.h"

#include <iostream>

#include "Player.h"

Renderer::Renderer()
{
	SDL_Init(SDL_INIT_VIDEO);

	// Game window and renderer settings
	window_width_  = 640;
	window_height_ = 480 + 32; // 32 = node size
	window_ = SDL_CreateWindow(
		"Snake C++ Exam",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width_,
		window_height_,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	if (window_ == nullptr)
	{
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window_);
		SDL_Quit();
	}
	else
	{
		renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
		if (renderer_ == nullptr)
		{
			std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window_);
			SDL_Quit();
		}
	}
}

Renderer& Renderer::get_instance()
{
	static Renderer instance;
	return instance;
}

SDL_Texture* Renderer::create_texture(const char* file)
{
	// Create texture from file
	auto* image = SDL_LoadBMP(file);
	if (image == nullptr)
	{
		std::cerr << "Failed to load head image: " << SDL_GetError() << std::endl;
		clean();
	}
	auto* texture = SDL_CreateTextureFromSurface(renderer_, image);
	
	textures_.emplace_back(texture);
	
	SDL_FreeSurface(image);
	return texture;
}

void Renderer::copy_node(const std::shared_ptr<SnakeNode>& node, const int rotation) const
{
	// Copy texture, coords and rotation to renderer
	SDL_RenderCopyEx(renderer_, node->texture, nullptr, &node->current_position, rotation, nullptr, SDL_FLIP_NONE);
}

void Renderer::copy(SDL_Texture* texture, SDL_Rect& coords) const
{
	SDL_RenderCopy(renderer_, texture, nullptr, &coords);
}

void Renderer::clean() const
{
	for (auto* texture : textures_)
	{
		// Destroy textures used in game
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void Renderer::render() const
{
	SDL_RenderPresent(renderer_);
	SDL_RenderClear(renderer_);
}