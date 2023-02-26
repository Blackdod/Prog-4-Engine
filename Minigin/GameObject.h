#pragma once
#include <memory>
#include <vector>
#include "Components.h"
//#include "Transform.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject final
	{
	public:
		void Update(float dt);
		void Render() const;

		void AddComponent(Component* component);
		void RemoveComponent(Component::ComponentType type);
		Component* GetComponent() const;
		Transform* GetTransform() const;
		TextureComponent* GetTexture() const;
		TextComponent* GetText() const;
		FPSComponent* GetFPSComponent() const;
		bool IsComponentPresent(Component::ComponentType type) const;

		void SetPosition(float x = 0.f, float y = 0.f, float z = 0.f); //Set position of all components that could have it


		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;


	private:
		std::vector<Component*> m_pComponents{};
	};
}
