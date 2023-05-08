#pragma once

#include "camera.hpp"
#include "gameObject.hpp"

//lib
#include <vulkan/vulkan.h>

namespace lve
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
		GameObject::Map &gameObjects;
	};
}