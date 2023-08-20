#include <stdexcept>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
//#include "GameObject.h"
#include "CollisionManager.h"

#include <chrono>
#include <numeric>

#pragma region Transform

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::SetPosition(glm::vec3 position)
{
	m_position = position;
}

#pragma endregion

#pragma region Render

void dae::RenderComponent::Update([[maybe_unused]] float deltaT)
{
	if (m_pOwnerTransform == nullptr) //Check if the transform pointer is set
	{
		m_pOwnerTransform = GetOwner()->GetComponent<Transform>(); //This will only happen the first time
	}
	//or every update if there's no transform, this need optimization but don't know how
}

void dae::RenderComponent::Render() const
{
	const float textureWidthOffset = m_texture->GetSize().x / 2.f;
	const float textureHeightOffset = m_texture->GetSize().y / 2.f;
	if (m_pOwnerTransform == nullptr) //Check if the transform pointer is set
	{
		Renderer::GetInstance().RenderTexture(*m_texture, -textureWidthOffset, -textureHeightOffset);
	}
	else
	{
		const auto& pos = m_pOwnerTransform->GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x - textureWidthOffset, pos.y - textureHeightOffset);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	m_texture = texture;
}
#pragma endregion

#pragma region Text

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font)
	:Component(pOwner)
	,m_needsUpdate(true)
	,m_text(text)
	,m_font(std::move(font))
{
}

void dae::TextComponent::Update([[maybe_unused]] float deltaT)
{
	if (m_needsUpdate)
	{
		const SDL_Color color{ 255,255,255 };
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		GetOwner()->GetComponent<RenderComponent>()->SetTexture(std::make_shared<Texture2D>(texture));
		m_needsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

//void dae::TextComponent::SetColor(Uint8 r, Uint8 g, Uint8 b)
//{
//	m_color.r = r;
//	m_color.g = g;
//	m_color.b = b;
//}

#pragma endregion

#pragma region FPS

void dae::FPSComponent::Update(float deltaT)
{
	if(m_pOwnerText == nullptr) //Check if the text pointer is set
	{
		m_pOwnerText = GetOwner()->GetComponent<TextComponent>(); //This will only happen the first time
	}
	else
	{
		m_NrOfFrames = static_cast<int>(1 / deltaT);
		const std::string printString = std::to_string(m_NrOfFrames) + " FPS";
		m_pOwnerText->SetText(printString);
	}
}

#pragma endregion

#pragma region Encircle

void dae::Encircle::Update(float deltaT)
{
	float multiplier{ 1 };//+ for anti-clockwise rotation, + for clockwise, could be variable
	if(m_isRotatingClockwise)
	{
		multiplier = -1;
	}

	m_Time += deltaT;
	if(m_Time >= m_SecPerRotation)
	{
		m_Time -= m_SecPerRotation;
	}

	m_Offset.x =  m_Distance * cosf(multiplier * ( static_cast<float>(2 * M_PI) / m_SecPerRotation) * m_Time);
	m_Offset.y =  m_Distance * sinf(multiplier * (static_cast<float>(2 * M_PI) / m_SecPerRotation) * m_Time);
	
	GetChangeableOwner()->SetLocalPosition(glm::vec3(m_Offset.x, m_Offset.y, 0.f));
	GetChangeableOwner()->SetPositionDirty();
}

#pragma endregion

#pragma region ImGUI

//void dae::ImGUIComponenent::RenderUI() const
//{
//	//GUI here
//	//Static because stuff didn't work as member variable
//	static int nrOfSamples{ 10 };
//	static bool showIntGraph{ false };
//	ImGui::PlotConfig intPlotConfig{};
//
//	ImGui::Begin("Exercise 2"); //Create a window called "Exercise 2"
//	ImGui::InputInt("samples", &nrOfSamples);
//	if (ImGui::Button("Trash the Cache") && !showIntGraph)
//	{
//		showIntGraph = true;
//		TrashTheCacheInts(intPlotConfig, nrOfSamples);
//		*m_pIntPlotConfig = ImGui::PlotConfig{ intPlotConfig };
//	}
//
//	if (showIntGraph)
//	{
//		ImGui::Plot("Trash the Cache Ints", *m_pIntPlotConfig);
//	}
//	ImGui::End();
//
//	static bool showGameObjectGraph{ false };
//	ImGui::PlotConfig gameObjectPlotConfig{};
//	ImGui::PlotConfig gameObjectAltPlotConfig{};
//
//	ImGui::Begin("Exercise 3"); //Create a window called "Exercise 3"
//	ImGui::InputInt("samples", &nrOfSamples);
//	if (ImGui::Button("Trash the Cache") && !showGameObjectGraph)
//	{
//		showGameObjectGraph = true;
//		TrashTheCacheGameObjects(gameObjectPlotConfig, nrOfSamples);
//		*m_pGameObjectPlotConfig = ImGui::PlotConfig{ gameObjectPlotConfig };
//
//		TrashTheCacheGameObjectAlts(gameObjectAltPlotConfig, nrOfSamples);
//		*m_pGameObjectAltPlotConfig = ImGui::PlotConfig{ gameObjectAltPlotConfig };
//	}
//
//	if (showGameObjectGraph)
//	{
//		ImGui::Plot("Trash the Cache GameObjects", *m_pGameObjectPlotConfig);
//		ImGui::Plot("Trash the Cache GameObjectAlts", *m_pGameObjectAltPlotConfig);
//	}
//
//	ImGui::End();
//}
//
//void dae::ImGUIComponenent::TrashTheCacheInts(ImGui::PlotConfig& plotConfig, int samples) const
//{
//	const size_t size = 10'000'000;
//	auto chungus = new std::vector<int>(size, 1);
//
//	delete plotConfig.values.ys;
//
//	std::vector<float> averages{};
//	std::vector<float> xData{};
//
//	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
//	{
//		std::vector<long long> durations{};
//		for (int j{}; j < samples; ++j)
//		{
//			auto start = std::chrono::high_resolution_clock::now();
//
//			for (int i{}; i < size; i += stepsize)
//			{
//				chungus->at(i) *= 2;
//			}
//
//			auto end = std::chrono::high_resolution_clock::now();
//			auto deltaT = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//			durations.push_back(deltaT);
//		}
//		durations.erase(std::max_element(durations.begin(), durations.end()));
//		durations.erase(std::min_element(durations.begin(), durations.end()));
//
//		long long longSize{ long long(durations.size()) };
//
//		float average{ (float)std::accumulate(durations.begin(), durations.end(), 0LL) / longSize };
//		averages.push_back(average);
//
//		xData.push_back((float)stepsize);
//		durations.clear();
//	}
//	const int dataBufferSize{ (int)averages.size() };
//	static auto pDataX = std::make_unique<float[]>(dataBufferSize);
//	static auto pDataY = std::make_unique<float[]>(dataBufferSize);
//
//	for (size_t i{}; i < dataBufferSize; ++i)
//	{
//		pDataX[i] = xData[i];
//	}
//
//	for (size_t i{}; i < dataBufferSize; ++i)
//	{
//		pDataY[i] = averages[i];
//	}
//
//	plotConfig.values.xs = pDataX.get(); // this line is optional
//	plotConfig.values.ys = pDataY.get();
//	plotConfig.values.count = dataBufferSize;
//	plotConfig.scale.min = 0;
//	plotConfig.scale.max = pDataY[0];
//	plotConfig.tooltip.show = true;
//	plotConfig.tooltip.format = "x=%.2f, y=%.2f";
//	plotConfig.grid_x.show = true;
//	plotConfig.grid_y.show = true;
//	plotConfig.frame_size = ImVec2(200, 200);
//	plotConfig.line_thickness = 2.f;
//	plotConfig.values.color = ImColor{ 200, 150, 0 };
//
//	chungus->clear();
//	delete chungus;
//}
//
//void dae::ImGUIComponenent::TrashTheCacheGameObjects(ImGui::PlotConfig& plotConfig, int samples) const
//{
//	const size_t size = 10'000'000;
//	auto chungus = new std::vector<GameObject3D>(size);
//
//	delete plotConfig.values.ys;
//
//	std::vector<float> averages{};
//	std::vector<float> xData{};
//
//	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
//	{
//		std::vector<long long> durations{};
//		for (int j{}; j < samples; ++j)
//		{
//			auto start = std::chrono::high_resolution_clock::now();
//
//			for (int i{}; i < size; i += stepsize)
//			{
//				chungus->at(i).ID *= 2;
//			}
//
//			auto end = std::chrono::high_resolution_clock::now();
//			auto deltaT = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//			durations.push_back(deltaT);
//		}
//		durations.erase(std::max_element(durations.begin(), durations.end()));
//		durations.erase(std::min_element(durations.begin(), durations.end()));
//
//		long long longSize{ long long(durations.size()) };
//
//		float average{ (float)std::accumulate(durations.begin(), durations.end(), 0LL) / longSize };
//		averages.push_back(average);
//
//		xData.push_back((float)stepsize);
//
//		durations.clear();
//	}
//	const int dataBufferSize{ (int)averages.size() };
//	static auto pDataX = std::make_unique<float[]>(dataBufferSize);
//	static auto pDataY = std::make_unique<float[]>(dataBufferSize);
//
//	for (size_t i{}; i < dataBufferSize; ++i)
//	{
//		pDataX[i] = xData[i];
//	}
//
//	for (size_t i{}; i < dataBufferSize; ++i)
//	{
//		pDataY[i] = averages[i];
//	}
//
//	plotConfig.values.xs = pDataX.get(); // this line is optional
//	plotConfig.values.ys = pDataY.get();
//	plotConfig.values.count = dataBufferSize;
//	plotConfig.scale.min = 0;
//	plotConfig.scale.max = pDataY[0];
//	plotConfig.tooltip.show = true;
//	plotConfig.tooltip.format = "x=%.2f, y=%.2f";
//	plotConfig.grid_x.show = true;
//	plotConfig.grid_y.show = true;
//	plotConfig.frame_size = ImVec2(200, 200);
//	plotConfig.line_thickness = 2.f;
//	plotConfig.values.color = ImColor{ 200, 150, 0 };
//
//	chungus->clear();
//	delete chungus;
//}
//
//void dae::ImGUIComponenent::TrashTheCacheGameObjectAlts(ImGui::PlotConfig& plotConfig, int samples) const
//{
//	const size_t size = 10'000'000;
//	auto chungus = new std::vector<GameObject3DAlt>(size);
//
//	delete plotConfig.values.ys;
//
//	std::vector<float> averages{};
//	std::vector<float> xData{};
//
//	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
//	{
//		std::vector<long long> durations{};
//		for (int j{}; j < samples; ++j)
//		{
//			auto start = std::chrono::high_resolution_clock::now();
//
//			for (int i{}; i < size; i += stepsize)
//			{
//				chungus->at(i).ID *= 2;
//			}
//
//			auto end = std::chrono::high_resolution_clock::now();
//			auto deltaT = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//			durations.push_back(deltaT);
//		}
//		durations.erase(std::max_element(durations.begin(), durations.end()));
//		durations.erase(std::min_element(durations.begin(), durations.end()));
//
//		long long longSize{ long long(durations.size()) };
//
//		float average{ (float)std::accumulate(durations.begin(), durations.end(), 0LL) / longSize };
//		averages.push_back(average);
//
//		xData.push_back((float)stepsize);
//		durations.clear();
//	}
//	const int dataBufferSize{ (int)averages.size() };
//	static auto pDataX = std::make_unique<float[]>(dataBufferSize);
//	static auto pDataY = std::make_unique<float[]>(dataBufferSize);
//
//	for (size_t i{}; i < dataBufferSize; ++i)
//	{
//		pDataX[i] = xData[i];
//	}
//
//	for (size_t i{}; i < dataBufferSize; ++i)
//	{
//		pDataY[i] = averages[i];
//	}
//
//	plotConfig.values.xs = pDataX.get(); // this line is optional
//	plotConfig.values.ys = pDataY.get();
//	plotConfig.values.count = dataBufferSize;
//	plotConfig.scale.min = 0;
//	plotConfig.scale.max = pDataY[0];
//	plotConfig.tooltip.show = true;
//	plotConfig.tooltip.format = "x=%.2f, y=%.2f";
//	plotConfig.grid_x.show = true;
//	plotConfig.grid_y.show = true;
//	plotConfig.frame_size = ImVec2(200, 200);
//	plotConfig.line_thickness = 2.f;
//	plotConfig.values.color = ImColor{ 200, 150, 0 };
//
//
//	chungus->clear();
//	delete chungus;
//}

#pragma endregion

#pragma region ColliderComponent

dae::ColliderComponent::ColliderComponent(GameObject* go, std::string tag)
	: Component(go), m_Tag{ tag }
{
	CollisionManager::GetInstance().AddCollider(this);
	//std::cout << "Added collider to manager!\n";
}

bool dae::ColliderComponent::IsColliding(ColliderComponent* otherCollider) const
{
	// if has the same tag, do not compare! ignore collision
	std::string otherTag = otherCollider->GetTag();
	if (m_Tag == otherTag)
	{
		return false;
	}

	float xMax = m_ColliderBox.xMin + m_ColliderBox.width;
	float yMax = m_ColliderBox.yMin + m_ColliderBox.height;

	Collider otherBox = otherCollider->m_ColliderBox;

	if (xMax < otherBox.xMin || (otherBox.xMin + otherBox.width) < m_ColliderBox.xMin)
	{
		return false;
	}

	if ((m_ColliderBox.yMin > otherBox.yMin + otherBox.height) || otherBox.yMin > yMax)
	{
		return false;
	}

	return true;
}

void dae::ColliderComponent::Update([[maybe_unused]] float deltaT)
{
	const auto myPosition = GetOwner()->GetComponent<Transform>()->GetPosition();

	m_ColliderBox.xMin = myPosition.x;
	m_ColliderBox.yMin = myPosition.y;
}

void dae::ColliderComponent::SetDimensions(float width, float height)
{
	// Sets the dimensions of the bounding box
	m_ColliderBox.width = width;
	m_ColliderBox.height = height;
}

void dae::ColliderComponent::SetPosition(float xPos, float yPos)
{
	m_ColliderBox.xMin = xPos;
	m_ColliderBox.yMin = yPos;
}

#pragma endregion