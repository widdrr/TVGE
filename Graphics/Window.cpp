module;

#include <GL/glew.h>
#include <GL/glfw3.h>

module Graphics:Window;

import <iostream>;

using namespace TVGE;

std::unique_ptr<Window> Window::_instance = nullptr;

Window& Window::Initialize(const std::string_view& p_title, const unsigned int p_windowHeight, const unsigned int p_windowWidth)
{
	if (_instance == nullptr) {
		_instance = std::unique_ptr<Window>(new Window(p_title, p_windowHeight, p_windowWidth));
	}

	return *_instance;
}

Graphics::Renderer& Window::GetRenderer() const
{
	return *_renderer;
}

Input::Input& Window::GetInput() const
{
	return *_input;
}

bool Window::IsOpen() const
{
	return !glfwWindowShouldClose(_window);
}

bool Window::IsFocused() const
{
	return _focused;
}

void Window::Focus()
{
	if (IsFocused()) {
		return;
	}
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_focused = true;
}

void Window::Unfocus()
{
	if (!IsFocused()) {
		return;
	}
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	_focused = false;
}

void Window::InitializeTime()
{
	_lastTime = glfwGetTime();
	_frames = 0;
}

float Window::ComputeDeltaTime()
{
	double currentTime = glfwGetTime();
	_deltaTime = currentTime - _lastTime;

	_lastTime = currentTime;

	return GetDeltaTime();
}

void Window::ComputeFPS()
{
	_fpsDelta += _deltaTime;
	++_frames;

	if (_fpsDelta >= 1.0) {
		double fps = static_cast<double>(_frames) / _fpsDelta;
		std::cout << "Frame Rate: " << fps << " FPS\n";

		_frames = 0;
		_fpsDelta = 0;
	}
}

float Window::GetDeltaTime() const
{
	return static_cast<float>(_deltaTime);
}

Window::Window(const std::string_view& p_title, const unsigned int p_windowWidth, const unsigned int p_windowHeight)
	:
	_windowWidth(p_windowWidth),
	_windowHeight(p_windowHeight),
	_lastTime(),
	_deltaTime(),
	_frames(),
	_fpsDelta()
{
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
			_instance->_renderer->SetPerspective(90, 0.1f, 100);
			_instance->_renderer->RenderScene();
		}
	);
	
	glfwSetCursorPosCallback(_window,
							 [](GLFWwindow* _window, double p_x, double p_y) {
								 for (auto&& callback : _instance->_input->_cursorPositionCallbacks) {
									 callback(p_x, p_y);
								 }
							 }
	);

	_renderer = std::unique_ptr<Graphics::Renderer>(new Graphics::Renderer(_window));
	_input = std::unique_ptr<Input::Input>(new Input::Input(_window));
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}