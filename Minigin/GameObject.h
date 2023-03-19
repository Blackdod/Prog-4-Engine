#pragma once
#include <memory>
#include <vector>
#include "Components.h"
//#include "Transform.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float dt);
		void FixedUpdate(float dt) const;
		void Render() const;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			m_pComponents.emplace_back(std::make_shared<T>(this, std::forward<Args>(args)...));

			if(typeid(T) == typeid(Transform))
			{
				m_localPos = dynamic_cast<Transform*>(m_pComponents.back().get())->GetPosition();
			}
			return *static_cast<T*>(m_pComponents.back().get());
		}
		template <typename T>
		void RemoveComponent()
		{
			for (auto it{ m_pComponents.begin() }; it != m_pComponents.end(); ++it)
			{
				if (dynamic_cast<T*>(it->get()))
				{
					m_pComponents.erase(it);
					break;
				}
			}
		}

		template <typename T>
		T* GetComponent() const
		{
			for (auto& component : m_pComponents)
			{
				T* CompPtr = dynamic_cast<T*>(component.get());
				if (CompPtr)
				{
					return CompPtr;
				}
			}
			return nullptr;
		}

		template <typename T>
		bool IsComponentPresent() const //Barely/Not used, just have it just in case I need it
		{
			for (auto& component : m_pComponents)
			{
				T* CompPtr = dynamic_cast<T*>(component.get());
				if (CompPtr)
				{
					return true;
				}
			}
			return false;
		}

		void SetParent(GameObject* pParent, bool keepWorldPosition = false);
		void SetLocalPosition(const glm::vec3& pos);
		void SetPositionDirty();
		const glm::vec3& GetLocalPosition() const { return m_localPos; };
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();

		const std::vector<GameObject*>& GetChildren() const { return m_pChildren; };

	private:
		std::vector<std::shared_ptr<Component>> m_pComponents{};
		glm::vec3 m_localPos{};
		glm::vec3 m_worldPos{};
		bool m_positionIsDirty{ false };
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren{}; //Change to shared

		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);
	};
}
