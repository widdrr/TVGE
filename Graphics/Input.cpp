module;

#include <GL/glfw3.h>

module Graphics:Input;

import <algorithm>;
import <iostream>;

void Input::ProcessInput()
{
	glfwPollEvents();

	for (auto&& [key, callbacks] : _keyCallbacks) {
		if (IsKeyPressed(key)) {
			if (!_previousKeyPressed[key]) {
				for (auto&& callback : callbacks) {
					callback();
				}
			}
			_previousKeyPressed[key] = true;
		}
		else {
			_previousKeyPressed[key] = false;
		}
	}

	for (auto&& [mouseButton, callbacks] : _clickCallbacks) {
		if (IsMouseButtonPressed(mouseButton)) {
			if (!_previousClickPressed[mouseButton]) {
				for (auto&& callback : callbacks) {
					callback();
				}
				_previousClickPressed[mouseButton] = true;
			}
		}
		else {
			_previousClickPressed[mouseButton] = false;
		}
	}

	for (auto&& callback : _genericCallbacks) {
		callback();
	}
}

void Input::AddKeyEvent(Keys p_key, Callback p_callback)
{
	if (_keyCallbacks.contains(p_key)) {
		_keyCallbacks[p_key].push_back(p_callback);
	}
	else {
		_keyCallbacks[p_key] = { p_callback };
	}
}

void Input::AddMouseButtonEvent(MouseButtons p_mouseButton, Callback p_callback)
{
	if (_clickCallbacks.contains(p_mouseButton)) {
		_clickCallbacks[p_mouseButton].push_back(p_callback);
	}
	else {
		_clickCallbacks[p_mouseButton] = { p_callback };
	}
}

void Input::AddGenericEvent(Callback p_callback)
{
	_genericCallbacks.push_back(p_callback);
}

void Input::AddCursorPositionEvent(CursorPositionCallback p_callback)
{
	_cursorPositionCallbacks.push_back(p_callback);
}

bool Input::IsKeyPressed(Keys p_key)
{
	return glfwGetKey(_window, p_key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseButtons p_mouseButton)
{
	return glfwGetMouseButton(_window, p_mouseButton) == GLFW_PRESS;
}

std::pair<double, double> Input::GetCursorPosition() const
{
	std::pair<double, double> position;
	glfwGetCursorPos(_window, &position.first, &position.second);
	return position;
}

Input::Input(GLFWwindow* p_window) :
	_window(p_window),
	_keyCallbacks(),
	_clickCallbacks(),
	_cursorPositionCallbacks(),
	_genericCallbacks(),
	_previousClickPressed(),
	_previousKeyPressed()
{
}