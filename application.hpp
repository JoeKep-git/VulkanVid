#pragma once

#include "VWindow.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "engineSwapChain.hpp"

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
		void createPipelinelayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		VWindow vWindow{ WIDTH, HEIGHT, "Vulkan World!" };
		LveDevice lveDevice{ vWindow };
		LveSwapChain lveSwapChain{ lveDevice, vWindow.getExtent() };
		std::unique_ptr<PipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}
