#include "SoundSystems.h"
#include <iostream>

void NullSoundSystem::Update()
{
}

void NullSoundSystem::PlaySound(int)
{
}

SDLSoundSystem::SDLSoundSystem()
	:m_SoundList{}
	, m_EventQueue{}
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, MIX_CHANNELS);

	m_Thread = std::jthread(&SDLSoundSystem::Update, this);
	std::cout << "Sound system initialized.\n";
}

SDLSoundSystem::~SDLSoundSystem()
{
	m_Quit = true;
	m_ConditionVariable.notify_one();

	for (auto& sound : m_SoundList) {
		Mix_FreeChunk(sound.second);
		sound.second = nullptr;
	}

	Mix_CloseAudio();
	Mix_Quit();

}


void SDLSoundSystem::Update()
{
	while (!m_Quit) {

		std::unique_lock<std::mutex> lock(m_Mutex);
		m_ConditionVariable.wait(lock, [this]() { return !m_EventQueue.empty() || m_Quit; });
		if (m_Quit) {
			break;
		}

		auto event = m_EventQueue.front();
		m_EventQueue.pop();


		switch (event.type) {
		case SoundType::Sound:
			if (m_SoundList.count(event.soundId)) {
				auto chunk = m_SoundList[event.soundId];
				Mix_PlayChannel(-1, chunk, 0);
				m_SoundsToPlay.push_back(chunk);
			}
			break;
		default:
			break;
		}
	}

}

void SDLSoundSystem::PlaySound(int soundId)
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	m_EventQueue.push(Event{ SoundType::Sound, soundId });
	m_ConditionVariable.notify_one();
}

void SDLSoundSystem::AddSound(const std::string& filename)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
	if (sound != nullptr) {
		int soundId = static_cast<int>(m_SoundList.size());
		m_SoundList[soundId] = sound;
	}
}