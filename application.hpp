#pragma once

#include "VWindow.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "engineSwapChain.hpp"
#include "gameObject.hpp"

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
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		VWindow vWindow{ WIDTH, HEIGHT, "Vulkan World!" };
		LveDevice lveDevice{ vWindow };
		std::unique_ptr<LveSwapChain> lveSwapChain;
		std::unique_ptr<PipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector <GameObject> gameObjects;
	};
}
