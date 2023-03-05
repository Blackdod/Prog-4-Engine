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
		void Update(float dt) const;
		void FixedUpdate(float dt) const;
		void Render() const;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			m_pComponents.emplace_back(std::make_shared<T>(this, std::forward<Args>(args)...));
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
		bool IsComponentPresent() const
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

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;


	private:
		std::vector<std::shared_ptr<Component>> m_pComponents{};
	};
}
