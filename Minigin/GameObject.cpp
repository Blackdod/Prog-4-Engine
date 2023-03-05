#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject()
{

};

void dae::GameObject::Update(float dt) const
{
	for(auto& component :m_pComponents)
	{
		component->Update(dt);
	}
}

void dae::GameObject::FixedUpdate([[maybe_unused]] float dt) const
{
	
}

void dae::GameObject::Render() const
{
	for(const auto& component : m_pComponents)
	{
		component->Render();
	}
}