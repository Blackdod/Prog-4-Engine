#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	object->SetParent(nullptr);
	//for(auto& child : object->GetChildren())
	//{
	//	
	//}
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}


void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update([[maybe_unused]] float dt)
{
	for(auto& object : m_objects)
	{
		object->Update(dt);
	}
}

void Scene::FixedUpdate([[maybe_unused]] float dt)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(dt);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

