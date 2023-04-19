#pragma once

#include "VWindow.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "gameObject.hpp"
#include "Renderer.hpp"

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
		void createPipelinelayout();
		void createPipeline();
		void renderGameObjects(VkCommandBuffer commandBuffer);

		VWindow vWindow{ WIDTH, HEIGHT, "Vulkan World!" };
		LveDevice lveDevice{ vWindow };
		Renderer renderer{ vWindow, lveDevice };

		std::unique_ptr<PipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector <GameObject> gameObjects;
	};
}
