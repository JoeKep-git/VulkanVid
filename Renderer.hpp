#pragma once

#include "VWindow.hpp"
#include "device.hpp"
#include "engineSwapChain.hpp"

//std
#include <memory>
#include <vector>
#include <cassert>

namespace lve {
	class Renderer
	{
	public:
		Renderer(VWindow &vWindow, LveDevice &lveDevice);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const 
		{ 
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentImageIndex]; 
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		VWindow &vWindow;
		LveDevice &lveDevice;
		std::unique_ptr<LveSwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		bool isFrameStarted;
	};
}
