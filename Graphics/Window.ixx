module;

#define GLFW_DLL
#include <GL/glfw3.h>

export module Graphics:Window;
import :Renderer;
import :Input;

import <memory>;
import <string_view>;

export class Window
{
public:
	static Window& Initialize(const std::string_view& p_title,
							  const unsigned int p_windowHeight,
							  const unsigned int p_windowWidth);
	~Window();

	bool IsOpen() const;
	Renderer& GetRenderer() const;

private:
	Window(const std::string_view& p_title,
		   const unsigned int p_windowWidth,
		   const unsigned int p_windowHeight);


	static std::unique_ptr<Window> _instance;

	GLFWwindow* _window;
	std::unique_ptr<Renderer> _renderer;
	std::unique_ptr<Input> _input;

	unsigned int _windowHeight;
	unsigned int _windowWidth;
};