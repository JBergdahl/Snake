#ifndef SNAKE_RENDERER_H
#define SNAKE_RENDERER_H

#include <memory>
#include <SDL.h>
#include <vector>

#include "SnakeNode.h"

class Renderer
{
private:
	Renderer();
	SDL_Renderer* renderer_{ nullptr };
	SDL_Window* window_{ nullptr };
	std::vector<SDL_Texture*> textures_;
	int window_width_{ 0 };
	int window_height_{ 0 };
public:
	static Renderer& get_instance(); // Get instance of this class
	Renderer(Renderer const&) = delete; // Delete copy_node constructor
	Renderer(Renderer const&&) = delete; // Delete copy_node constructor
	void operator=(Renderer const&) = delete; // Delete copy_node assignment operator
	void operator=(Renderer const&&) = delete; // Delete move assignment operator
	SDL_Texture* create_texture(const char* file);
	void copy_node(const std::shared_ptr<SnakeNode>& node, int rotation = 0) const;
	void copy(SDL_Texture* texture, SDL_Rect &coords) const;
	void clean() const;
	void render() const;
	[[nodiscard]] int get_window_w() const { return window_width_; }
	[[nodiscard]] int get_window_h() const { return window_height_; }
	[[nodiscard]] SDL_Renderer* get_renderer() const { return renderer_; }
};

#endif

