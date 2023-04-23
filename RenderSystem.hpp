#pragma once

#include "pipeline.hpp"
#include "device.hpp"
#include "gameObject.hpp"
#include "camera.hpp"

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

		void renderGameObjects(
			VkCommandBuffer commandBuffer, 
			std::vector<GameObject> &gameObjects, 
			const Camera &camera);

	private:
		void createPipelinelayout();
		void createPipeline(VkRenderPass renderPass);

		LveDevice &lveDevice;

		std::unique_ptr<PipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}
