#pragma once

#include "pipeline.hpp"
#include "device.hpp"
#include "gameObject.hpp"

//std
#include <memory>
#include <vector>

namespace lve {
	class RenderSystem
	{
	public:
		RenderSystem(LveDevice &device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects);

	private:
		void createPipelinelayout();
		void createPipeline(VkRenderPass renderPass);

		LveDevice &lveDevice;

		std::unique_ptr<PipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}
