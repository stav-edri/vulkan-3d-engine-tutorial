#pragma once

#include "ke_model.hpp"

#include <memory>

namespace ke {
	struct Transform2dComponent {
		glm::vec2 translation{}; // (position, offset)
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;

		glm::mat2 mat2() { 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c,s}, {-s,c} };

			glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
			
			return rotMatrix * scaleMat;
		}
	};

	class KeGameObject
	{
	public:
		using id_t = unsigned int;

		static KeGameObject createGameObject()
		{
			static id_t currentId = 0;
			return KeGameObject{ currentId++ };
		}

		KeGameObject(const KeGameObject&) = delete;
		KeGameObject(KeGameObject&&) = default;
		KeGameObject &operator=(const KeGameObject&) = delete;
		KeGameObject &operator=(KeGameObject&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<KeModel> model{};
		glm::vec3 color{};
		Transform2dComponent transform2d{};

	private:
		KeGameObject(id_t objId) : id{objId} {}

		id_t id;
	};
}

