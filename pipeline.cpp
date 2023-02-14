#include "pipeline.hpp"

using namespace std;

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve 
{
	PipeLine::PipeLine(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath);
	}

	std::vector<char> PipeLine::readFile(const std::string filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			//filepath is incorrect or do not have permissions
			throw runtime_error("failed to open file: " + filePath);

			size_t fileSize = static_cast<size_t>(file.tellg());

			vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();
			return buffer;
		}
	}

	void PipeLine::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
		cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
	}
}

