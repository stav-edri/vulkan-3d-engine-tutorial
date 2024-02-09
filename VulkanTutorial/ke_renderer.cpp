#include "ke_renderer.hpp"


#include <stdexcept>
#include <array>

namespace ke {
	KeRenderer::KeRenderer(KeWindow& keWindow, KeDevice& keDevice) : keWindow{ keWindow }, keDevice{ keDevice }
	{
		recreateSwapChain();
		createCommandBuffers();
	}
	KeRenderer::~KeRenderer()
	{
		freeCommandBuffers();
	}

	VkCommandBuffer KeRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Can't call beginFrame when already in progress");
		auto result = keSwapChain->acquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin command buffer " + currentImageIndex);
		}

		return commandBuffer;
	}

	void KeRenderer::endFrame()
	{
		assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer");
		}

		auto result = keSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || keWindow.wasWindowResized())
		{
			keWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % KeSwapChain::MAX_FRAMES_IN_FLIGHT;

	}

	void KeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call beginSwapChainRenderpass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = keSwapChain->getRenderPass();
		renderPassInfo.framebuffer = keSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = keSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(keSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(keSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0,0}, keSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void KeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call beginSwapChainRenderpass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void KeRenderer::createCommandBuffers()
	{
		commandBuffers.resize(KeSwapChain::MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = keDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(keDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}


	}
	void KeRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(keDevice.device(), keDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void KeRenderer::recreateSwapChain()
	{
		auto extent = keWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = keWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(keDevice.device());

		if (keSwapChain == nullptr) {
			keSwapChain = std::make_unique<KeSwapChain>(keDevice, extent);
		}
		else {
			std::shared_ptr<KeSwapChain> oldSwapChain = std::move(keSwapChain);
			keSwapChain = std::make_unique<KeSwapChain>(keDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*keSwapChain))
			{
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}

		}

		
	}

} //namespace ke
