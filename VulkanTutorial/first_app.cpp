#include "first_app.hpp"
#include "vertex_generator.hpp"
#include "simple_render_system.hpp"
#include "ke_camera.hpp"

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
		KeCamera camera{};
		// camera.setViewDirection(glm::vec3(0.f), glm::vec3(.5f, 0.f, 1.f));
		camera.setViewTarget(glm::vec3(01.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 1.5f));

		while (!keWindow.shouldClose()) {
			glfwPollEvents();
			float aspect = keRenderer.getAspectRatio();

			camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

			if (auto commandBuffer = keRenderer.beginFrame()) {
				keRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				keRenderer.endSwapChainRenderPass(commandBuffer);
				keRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(keDevice.device());
	}
	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<KeModel> keModel = createCubeModel(keDevice, { .0f, .0f, .0f });

		auto cube = KeGameObject::createGameObject();
		cube.model = keModel;
		cube.transform.translation = { .0f, .0f, 1.5f };
		cube.transform.scale = { .5f, .5f, .5f };
		gameObjects.push_back(std::move(cube));
	}
} //namespace ke
