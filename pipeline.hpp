#pragma once

#include "device.hpp"

#include <string>
#include <vector>
#include <cassert>

using namespace std;

namespace lve {

	struct PipelineConfigInfo 
	{
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class PipeLine
	{
	public:
		PipeLine(
			LveDevice& device,
			const std::string& vertFilePath,
			const std::string& fragFilePath,
			const PipelineConfigInfo& configInfo);

		~PipeLine();

		PipeLine(const PipeLine&) = delete;
		PipeLine &operator=(const PipeLine&) = delete;

		static void defaultPipeLineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string filePath);


		void createGraphicsPipeline(
			const std::string& vertFilePath, 
			const std::string& fragFilePath,
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const vector<char>& code, VkShaderModule* shaderModule);

		//only used when there is an implicit relationship that the 
		//member variable will outlive the containing class that depend on this
		LveDevice& lveDevice;

		//typedef pointers
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}