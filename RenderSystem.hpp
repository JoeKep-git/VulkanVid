#pragma once

#include "pipeline.hpp"
#include "device.hpp"
#include "gameObject.hpp"
#include "camera.hpp"
#include "frameInfo.hpp"

//std
#include <memory>
#include <vector>

namespace lve {
	class RenderSystem
	{
	public:
		RenderSystem(LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void renderGameObjects(FrameInfo &frameInfo);

	private:
		void createPipelinelayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		LveDevice &lveDevice;

		std::unique_ptr<PipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}
