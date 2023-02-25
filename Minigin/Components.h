#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Component
	{
	public:
		enum class ComponentType
		{
			none,
			transform
		};
		ComponentType m_type{ ComponentType::none };

		Component() = default;
		virtual ~Component() {};

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		ComponentType GetType() const { return m_type; };
	private:
	};

	class Transform : public Component
	{
	public:
		Transform(float x = 0.f, float y = 0.f, float z = 0.f) : m_position{ x, y, z }
		{
			Component::m_type = ComponentType::transform;
		};

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_position;
	};
}