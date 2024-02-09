#include "first_app.hpp"
#include "vertex_generator.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace ke {
	FirstApp::FirstApp()
	{
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
	}
	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ keDevice, keRenderer.getSwapChainRenderPass() };
		while (!keWindow.shouldClose()) {
			glfwPollEvents();
			if (auto commandBuffer = keRenderer.beginFrame()) {
				keRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				keRenderer.endSwapChainRenderPass(commandBuffer);
				keRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(keDevice.device());
	}
	void FirstApp::loadGameObjects()
	{
		std::vector<KeModel::Vertex> vertices{
			{{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}},
		};
		//std::vector<KeModel::Vertex> vertices = create_triangles({ {0.0f, -1.0f}, {1.0f, 0.0f, 0.0f } }, 2.0f, 3);

		auto keModel = std::make_shared<KeModel>(keDevice, vertices);

		auto triangle = KeGameObject::createGameObject();
		triangle.model = keModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform2d.translation.x = 0.0f;
		triangle.transform2d.scale = { 1.5f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));

	}
} //namespace ke
