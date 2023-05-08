#pragma once

#include "VWindow.hpp"
#include "device.hpp"
#include "gameObject.hpp"
#include "Renderer.hpp"
#include "descriptors.hpp"

//std
#include <memory>
#include <vector>

namespace lve {
	class FirstApplication
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApplication();
		~FirstApplication();

		FirstApplication(const FirstApplication&) = delete;
		FirstApplication &operator=(const FirstApplication&) = delete;

		void run();

	private:
		void loadGameObjects();

		VWindow vWindow{ WIDTH, HEIGHT, "Vulkan World!" };
		LveDevice lveDevice{ vWindow };
		Renderer renderer{ vWindow, lveDevice };

		//order of declaration matters here
		std::unique_ptr<LveDescriptorPool> globalPool{};
		std::vector <GameObject> gameObjects;
	};
}
