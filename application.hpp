#pragma once

#include "vulkanWindow.hpp"
#include "pipeline.hpp"

namespace lve {
	class FirstApplication
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		VWindow vWindow{ WIDTH, HEIGHT, "Vulkan World!" };
		PipeLine pipeLine{ "simple_shader.vert.spv", "simple_shader.frag.spv"};
	};
}
