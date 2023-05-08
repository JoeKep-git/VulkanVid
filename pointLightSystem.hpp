#pragma once

#include "pipeline.hpp"
#include "device.hpp"
#include "gameObject.hpp"
#include "camera.hpp"
#include "frameInfo.hpp"

//std
#include <memory>
#include <vector>

namespace lve 
{
	class PointLightSystem
	{
	public:
		PointLightSystem(LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void render(FrameInfo& frameInfo);

	private:
		void createPipelinelayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		LveDevice& lveDevice;

		std::unique_ptr<PipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}
