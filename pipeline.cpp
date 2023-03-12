#include "pipeline.hpp"

using namespace std;

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve 
{
	PipeLine::PipeLine(
		LveDevice& device,
		const std::string& vertFilePath,
		const std::string& fragFilePath,
		const PipeLineConfigInfo& configInfo) 
		: lveDevice{device}
	{
		createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
	}

	std::vector<char> PipeLine::readFile(const std::string filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			//filepath is incorrect or do not have permissions
			throw runtime_error("failed to open file: " + filePath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());

		vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	/*void PipeLine::createGraphicsPipeline(
		const std::string& vertFilePath, 
		const std::string& fragFilePath, 
		const PipeLineConfigInfo& configInfo)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);
	}*/

	void PipeLine::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipeLineConfigInfo& configInfo)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
		cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
	}

	void PipeLine::createShaderModule(const vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module");
		}
	}

	PipeLineConfigInfo PipeLine::defaultPipeLineConfigInfo(uint32_t width, uint32_t height)
	{
		PipeLineConfigInfo configInfo{};

		return configInfo;
	}

}


