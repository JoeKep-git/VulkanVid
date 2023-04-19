#include "application.hpp"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	FirstApplication::FirstApplication()
	{
		loadGameObjects();
		createPipelinelayout();
		createPipeline();
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

			if (auto commandBuffer = renderer.beginFrame())
			{
				//begin offscreen shadow pass
				//render shadow casting objects
				//end offscreen shadow pass

				renderer.beginSwapChainRenderPass(commandBuffer);
				renderGameObjects(commandBuffer);
				renderer.endSwapChainRenderPass(commandBuffer);
				renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApplication::loadGameObjects()
	{
		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f,0.0f,0.0f}},
			{{0.5f, 0.5f}, {0.0f,1.0f,0.0f}},
			{{-0.5f, 0.5f}, { 0.0f,0.0f,1.0f}}
		};
		auto model = std::make_shared<Model>(lveDevice, vertices);

		auto triangle = GameObject::createGameObject();
		triangle.model = model;
		triangle.color = { .1f,.8f,.1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
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
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		PipeLine::defaultPipeLineConfigInfo(
			pipelineConfig);
		pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<PipeLine>(
			lveDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApplication::renderGameObjects(VkCommandBuffer commandBuffer)
	{ 
		int i = 0;
		for (auto& obj : gameObjects)
		{
			i += 1;
			//for smooth roation you will need to be using v-sync, otherwise the amount you increment needs to be
			//proportional to the time between frames
			obj.transform2d.rotation = 
				glm::mod<float>(obj.transform2d.rotation + 0.001f * i, 2.f * glm::pi<float>());
		}

		lvePipeline->bind(commandBuffer);

		for (auto& obj : gameObjects)
		{
			SimplePushConstantData push{};

			push.offset = obj.transform2d.translation;
			push.color = obj.color;
			push.transform = obj.transform2d.mat2();

			vkCmdPushConstants(commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}
}


