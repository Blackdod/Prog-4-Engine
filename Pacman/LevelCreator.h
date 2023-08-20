#pragma once
#include <string>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class GameObject;

	class LevelCreator : public Singleton<LevelCreator>
	{
	public:
		static bool CreateLevel(const std::wstring& filePath, Scene* scene);
		const GameObject* GetPlayer1() const { return m_pPlayer1; };
		void SetPlayer1(GameObject* player1) { m_pPlayer1 = player1; };
	private:
		void CreateWall(Scene* scene, float xPos, float yPos) const;
		void SpawnDot(Scene* scene, float xPos, float yPos) const;
		GameObject* m_pPlayer1 = nullptr;
	};
}