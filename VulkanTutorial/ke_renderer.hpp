#pragma once

#include "ke_window.hpp"
#include "ke_device.hpp"
#include "ke_swap_chain.hpp"
#include "ke_game_object.hpp"

namespace ke 
{
	class KeRenderer
	{
	public:
		KeRenderer(KeWindow& keWindow, KeDevice& keDevice);
		~KeRenderer();

		KeRenderer(const KeRenderer&) = delete;
		KeRenderer& operator=(const KeRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return keSwapChain->getRenderPass(); }
		float getAspectRatio() const { return keSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}


		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		KeWindow& keWindow;
		KeDevice& keDevice;
		std::unique_ptr<KeSwapChain> keSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex = 0;
		bool isFrameStarted = false;
	};
}

