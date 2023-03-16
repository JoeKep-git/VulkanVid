#include "application.hpp"
#include <stdexcept>

namespace lve 
{
	FirstApplication::FirstApplication()
	{
		createPipelinelayout();
		createPipeline();
		createCommandBuffers();
	}

	FirstApplication::~FirstApplication()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApplication::run()
	{
		while (!vWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}

	void FirstApplication::createPipelinelayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void FirstApplication::createPipeline()
	{
		PipelineConfigInfo pipelineConfig{};
		PipeLine::defaultPipeLineConfigInfo(
			pipelineConfig,
			lveSwapChain.width(),
			lveSwapChain.height());
		pipelineConfig.renderPass = lveSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<PipeLine>(
			lveDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApplication::createCommandBuffers()
	{

	}

	void FirstApplication::drawFrame()
	{

	}
}


