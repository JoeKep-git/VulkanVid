#include "RenderSystem.hpp"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

namespace lve
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.f };
		alignas(16) glm::vec3 color;
	};

	RenderSystem::RenderSystem(LveDevice &device, VkRenderPass renderPass) : lveDevice{device}
	{
		createPipelinelayout();
		createPipeline(renderPass);
	}

	RenderSystem::~RenderSystem()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void RenderSystem::createPipelinelayout()
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

	void RenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		PipeLine::defaultPipeLineConfigInfo(
			pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<PipeLine>(
			lveDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void RenderSystem::renderGameObjects(
		VkCommandBuffer commandBuffer,
		std::vector<GameObject>& gameObjects,
		const Camera& camera)
	{
		lvePipeline->bind(commandBuffer);

		//float fovy = glm::radians(45.f);
		//float aspectRatio = 800.f / 600.f;
		//float near = 0.001f;
		//float far = 10.f;
		//auto perspective = perspectiveProjection(fovy, aspectRatio, near, far);

		for (auto& obj : gameObjects)
		{
			//for smooth roation you will need to be using v-sync, otherwise the amount you increment needs to be
			//proportional to the time between frames
			obj.transform.rotation.y =
				glm::mod<float>(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
			obj.transform.rotation.x =
				glm::mod<float>(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());

			SimplePushConstantData push{};
			push.color = obj.color;
			push.transform = camera.getProjection() * obj.transform.mat4();

			vkCmdPushConstants(
				commandBuffer,
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


