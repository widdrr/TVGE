module;

#include <Windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>

module Graphics:Window;

import <iostream>;


std::unique_ptr<Window> Window::_instance = nullptr;

Window& Window::Initialize(const std::string_view& p_title, const unsigned int p_windowHeight, const unsigned int p_windowWidth)
{
	if (_instance == nullptr) {
		_instance = std::unique_ptr<Window>(new Window(p_title, p_windowHeight, p_windowWidth));
	}

	return *_instance;
}

Renderer& Window::GetRenderer() const
{
	return *_renderer;
}

bool Window::IsOpen() const
{
	return !glfwWindowShouldClose(_window);
}

Window::Window(const std::string_view& p_title, const unsigned int p_windowWidth, const unsigned int p_windowHeight)
	:
	_windowWidth(p_windowWidth),
	_windowHeight(p_windowHeight)
{

#ifdef _WIN32
	timeBeginPeriod(1);
#endif

	GLenum res = glfwInit();
	if (res == GLFW_FALSE) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//setup and create window
	_window = glfwCreateWindow(_windowWidth, _windowHeight, p_title.data(), nullptr, nullptr);
	if (_window == nullptr) {
		std::cerr << "Failed to create GLFW window \n";
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(_window);

	//init glew
	res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << "\n";
		exit(1);
	}

	//we register a callback when the window is resized to always keep the viewport accurate
	glfwSetFramebufferSizeCallback(_window,
		[](GLFWwindow* p_window, int p_width, int p_height) {
			glViewport(0, 0, p_width, p_height);
			_instance->_renderer->RenderFrame();
		}
	);

	glfwSetCursorPosCallback(_window,
	[](GLFWwindow* _window, double _crtX, double _crtY) {
		_instance->_renderer->MouseCallback(_window, _crtX, _crtY);
	}
	);

	_renderer = std::unique_ptr<Renderer>(new Renderer(_window));
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
#ifdef _WIN32
	timeBeginPeriod(0);
#endif
}