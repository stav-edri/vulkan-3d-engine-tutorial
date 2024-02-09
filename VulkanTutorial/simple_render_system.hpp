#pragma once
#include "ke_device.hpp"
#include "ke_game_object.hpp"
#include "ke_pipeline.hpp"
#include "ke_camera.hpp"

namespace ke
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(KeDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<KeGameObject> &gameObjects, const KeCamera &camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		KeDevice& keDevice;

		std::unique_ptr<KePipeline> kePipeline;
		VkPipelineLayout pipelineLayout;
	};

}
