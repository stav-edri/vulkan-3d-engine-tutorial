#pragma once

#include "ke_window.hpp"
#include "ke_device.hpp"
#include "ke_model.hpp"
#include "ke_game_object.hpp"
#include "ke_renderer.hpp"

// std
#include <vector>
#include <memory>

namespace ke {
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		void loadGameObjects();

		KeWindow keWindow {WIDTH, HEIGHT, "Hello Vulkan!"};
		KeDevice keDevice{ keWindow };
		KeRenderer keRenderer{ keWindow, keDevice };
		std::vector<KeGameObject> gameObjects;
	};
} // namespace ke
