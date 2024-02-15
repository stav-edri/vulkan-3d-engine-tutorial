#include "first_app.hpp"
#include "vertex_generator.hpp"
#include "simple_render_system.hpp"
#include "ke_camera.hpp"
#include "keyboard_movement_controller.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <chrono>

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
		camera.setViewTarget(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.5f));

		auto viewerObject = KeGameObject::createGameObject();
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();
		const float MAX_FRAME_TIME = 1.f;

		while (!keWindow.shouldClose()) {
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.moveInPlaneXZ(keWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

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
