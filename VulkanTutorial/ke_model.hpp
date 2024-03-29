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
			glm::vec3 position;
			glm::vec3 color;

			static std::vector <VkVertexInputBindingDescription> getBindingDescriptions(); // in struct or class?
			static std::vector <VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
		};


		KeModel(KeDevice& device, const KeModel::Builder& builder);
		~KeModel();

		KeModel(const KeModel&) = delete;
		KeModel& operator=(const KeModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		KeDevice &keDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;
	};
}

