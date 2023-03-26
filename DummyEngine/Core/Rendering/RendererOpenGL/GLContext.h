#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Context.h"

struct GLFWwindow;

namespace DE {

	class GLContext : public Context
	{
	public:
		GLContext(GLFWwindow* window);

		virtual void Load() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_Window;
	};

}