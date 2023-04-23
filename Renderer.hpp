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
		float getAspectRatio() const { return lveSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const 
		{ 
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex]; 
		}

		int getFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endFrame();

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		VWindow &vWindow;
		LveDevice &lveDevice;
		std::unique_ptr<LveSwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool isFrameStarted{ false };
	};
}
