#pragma once

#include "device.hpp"

#include <string>
#include <vector>
#include "device.hpp"

using namespace std;

namespace lve {

	struct PipeLineConfigInfo
	{

	};

	class PipeLine
	{
	public:
		PipeLine(
			//	LveDevice& device,
			const std::string& vertFilePath,
			const std::string& fragFilePath);
			//const PipeLineConfigInfo& configInfo);

		~PipeLine() {}

		PipeLine(const PipeLine&) = delete;
		void operator=(const PipeLine&) = delete;

		static PipeLineConfigInfo defaultPipeLineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string filePath);


		void createGraphicsPipeline(
			const std::string& vertFilePath, 
			const std::string& fragFilePath);

		void createShaderModule(const vector<char>& code, VkShaderModule shaderModule);

		//only used when there is an implicit relationship that the 
		//member variable will outlive the containing class that depend on this
		//LveDevice& lveDevice;

		//typedef pointers
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}