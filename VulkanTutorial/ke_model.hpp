#pragma once

#include "ke_device.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace ke {
	class KeModel
	{
	public:
		struct Vertex {
			glm::vec2 position;
			glm::vec3 color;

			static std::vector <VkVertexInputBindingDescription> getBindingDescriptions(); // in struct or class?
			static std::vector <VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		KeModel(KeDevice &device, const std::vector<Vertex>& vertices);
		~KeModel();

		KeModel(const KeModel&) = delete;
		KeModel& operator=(const KeModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		KeDevice &keDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}

