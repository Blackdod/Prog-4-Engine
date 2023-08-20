#include "LevelCreator.h"
#include <rapidjson.h>
#include <document.h>
#include <stream.h>
#include <filereadstream.h>

#include <iostream>
#include "HelperStructs.h"
#include "Scene.h"
#include "Components.h"
#include "GameObject.h"
#include "GameComponents.h"

using namespace std;

bool dae::LevelCreator::CreateLevel(const std::wstring& filePath, Scene* scene)
{
	std::wstring fileName = filePath;
	rapidjson::Document jsonFile;
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, fileName.c_str(), L"rb");

	if (pFile != nullptr)
	{
		fseek(pFile, 0, SEEK_END);
		const size_t size = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		char* readBuffer = new char[size];
		rapidjson::FileReadStream inputStream(pFile, readBuffer, sizeof(readBuffer));
		jsonFile.ParseStream(inputStream);
		delete[] readBuffer;
		fclose(pFile);
	}

	const rapidjson::Value& tileSizeVal = jsonFile["tileSize"];
	const rapidjson::Value& nrRowsVal = jsonFile["nrRows"];
	const rapidjson::Value& nrColsVal = jsonFile["nrCols"];
	const rapidjson::Value& levelLayoutVal = jsonFile["levelLayout"];

	int tileSize = tileSizeVal.GetInt();
	int nrRows = nrRowsVal.GetInt();
	int nrCols = nrColsVal.GetInt();

	std::vector<int>levelLayOut;

	for (rapidjson::SizeType i = 0; i < levelLayoutVal.Size(); ++i)
	{
		// get values in the array
		levelLayOut.push_back(levelLayoutVal[i].GetInt());
	}

	// starting position of the grid for the level
	glm::vec3 startPos{90.f, 40.f, 0.f};


	for (int i = 0; i < nrRows; ++i)
	{
		float posY = startPos.y + (i * tileSize); //* tileSize;
		//std::cout << "col = " << i << "\n";

		for (int j = 0; j < nrCols; ++j)
		{
			float posX = startPos.x + (j * tileSize); //* tileSize;

			//std::cout << "row = " << j << "\n";

			switch (levelLayOut[i * nrCols + j])
			{
			case 1:
				GetInstance().CreateWall(scene, posX, posY);
				break;
			default:
				GetInstance().SpawnDot(scene, posX, posY);
				break;
			}
		}

	}
	return true;
}

void dae::LevelCreator::CreateWall(Scene* scene, float xPos, float yPos) const
{
	const auto wall = std::make_shared<GameObject>();
	Transform* wallTransform = &wall->AddComponent<Transform>();
	wallTransform->SetPosition(xPos, yPos, 0.f);
	RenderComponent* wallRenderer = &wall->AddComponent<RenderComponent>();
	wallRenderer->SetTexture("wall.png");
	ColliderComponent* wallCollider = &wall->AddComponent<ColliderComponent>("WALL");
	wallCollider->SetDimensions( 16.f, 16.f );
	wallCollider->SetPosition( xPos, yPos );

	const glm::vec3 pos{xPos, yPos, 0.f};
	wall.get()->SetLocalPosition(pos);

	scene->Add(wall);
}

void dae::LevelCreator::SpawnDot(Scene* scene, float xPos, float yPos) const
{
	const auto pDot = std::make_shared<GameObject>();
	Transform* dotTransform = &pDot->AddComponent<Transform>();
	dotTransform->SetPosition(xPos, yPos, 0.f);
	RenderComponent* dotRenderer = &pDot->AddComponent<RenderComponent>();
	dotRenderer->SetTexture("pill.png");
	ColliderComponent* dotCollider = &pDot->AddComponent<ColliderComponent>("PICKUP");
	dotCollider->SetDimensions(16.f, 16.f);
	dotCollider->SetPosition(xPos, yPos);
	pDot->AddComponent<PacDotComponent>();

	const glm::vec3 pos{xPos, yPos, 0.f};
	pDot.get()->SetLocalPosition(pos);

	scene->Add(pDot);
}