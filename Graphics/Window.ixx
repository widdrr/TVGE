module;
#include <GL/glfw3.h>

export module Graphics:Window;
import :Renderer;
import :Input;

import <memory>;
import <string_view>;

namespace TVGE
{
	export class Window
	{
	public:
		static Window& Initialize(const std::string_view& p_title,
								  const unsigned int p_windowHeight,
								  const unsigned int p_windowWidth);
		~Window();

		bool IsOpen() const;
		bool IsFocused() const;
		void Focus();
		void Unfocus();

		void InitializeTime();
		float ComputeDeltaTime();
		void ComputeFPS();
		float GetDeltaTime() const;

		Graphics::Renderer& GetRenderer() const;
		Input::Input& GetInput() const;

	private:
		Window(const std::string_view& p_title,
			   const unsigned int p_windowWidth,
			   const unsigned int p_windowHeight);


		static std::unique_ptr<Window> _instance;

		GLFWwindow* _window;
		std::unique_ptr<Graphics::Renderer> _renderer;
		std::unique_ptr<Input::Input> _input;

		unsigned int _windowHeight;
		unsigned int _windowWidth;
		bool _focused;
		double _deltaTime, _lastTime, _fpsDelta;
		unsigned int _frames;
	};
}