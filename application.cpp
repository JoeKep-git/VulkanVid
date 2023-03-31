#include "application.hpp"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve
{
	struct SimplePushConstantData
	{
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	FirstApplication::FirstApplication()
	{
		loadModels();
		createPipelinelayout();
		recreateSwapChain();
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
			drawFrame();
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApplication::loadModels()
	{
		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f,0.0f,0.0f}},
			{{0.5f, 0.5f}, {0.0f,1.0f,0.0f}},
			{{-0.5f, 0.5f}, { 0.0f,0.0f,1.0f}}
		};
		model = std::make_unique<Model>(lveDevice, vertices);
	}

	void FirstApplication::createPipelinelayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void FirstApplication::createPipeline()
	{
		assert(lveSwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		PipeLine::defaultPipeLineConfigInfo(
			pipelineConfig);
		pipelineConfig.renderPass = lveSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<PipeLine>(
			lveDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApplication::recreateSwapChain()
	{
		auto extent = vWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = vWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(lveDevice.device());

		
		if (lveSwapChain == nullptr)
		{
			lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
		}
		else
		{
			lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, std::move(lveSwapChain));
			if (lveSwapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		//if render pass compatible do nothing else
		createPipeline();
	}

	void FirstApplication::createCommandBuffers()
	{
		commandBuffers.resize(lveSwapChain->imageCount());
		
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = lveDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void FirstApplication::freeCommandBuffers()
	{
		vkFreeCommandBuffers(lveDevice.device(), lveDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void FirstApplication::recordCommandBuffer(int imageIndex)
	{
		static int frame = 0;
		frame = (frame + 1) % 1000;

		VkCommandBufferBeginInfo beginInfo{};

		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to bein recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = lveSwapChain->getRenderPass();
		renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		
		VkRect2D scissor{ {0,0}, lveSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);


		lvePipeline->bind(commandBuffers[imageIndex]);
		model->bind(commandBuffers[imageIndex]);

		for (int i = 0; i < 4; i++)
		{
			SimplePushConstantData push{};

			push.offset = { -0.5f + frame * 0.002f, -0.4f + i * 0.25f };
			push.color = { 0.0f, 0.0f, 0.2f + 0.2f * i };

			vkCmdPushConstants(commandBuffers[imageIndex], 
				pipelineLayout, 
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
				0, 
				sizeof(SimplePushConstantData), 
				&push);

			model->draw(commandBuffers[imageIndex]);

		}


		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	void FirstApplication::drawFrame()
	{
		uint32_t imageIndex;
		auto result = lveSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);

		result = lveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vWindow.wasWindowResized())
		{
			vWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}
	}
}


