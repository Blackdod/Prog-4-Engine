#pragma once
#include <queue>
#include <thread>
#include <condition_variable>
#include <SDL_mixer.h>
#include <unordered_map>


using sound_id = unsigned short;

class SoundSystem
{
public:
	SoundSystem() = default;
	virtual ~SoundSystem() = default;
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&&) = delete;
	SoundSystem& operator= (const SoundSystem&) = delete;
	SoundSystem& operator= (const SoundSystem&&) = delete;

	virtual void Update() = 0;
	virtual void PlaySound(int soundID) = 0;
	virtual void AddSound(const std::string& filename) = 0;
};


class NullSoundSystem : public SoundSystem
{
public:
	NullSoundSystem() = default;
	~NullSoundSystem() override = default;
	NullSoundSystem(const NullSoundSystem&) = delete;
	NullSoundSystem(NullSoundSystem&&) = delete;
	NullSoundSystem& operator= (const NullSoundSystem&) = delete;
	NullSoundSystem& operator= (const NullSoundSystem&&) = delete;

	void Update() override;
	void PlaySound(int soundId) override;
};

class SDLSoundSystem final : public SoundSystem
{
public:
	SDLSoundSystem();
	~SDLSoundSystem() override;
	SDLSoundSystem(const SDLSoundSystem&) = delete;
	SDLSoundSystem(SDLSoundSystem&&) = delete;
	SDLSoundSystem& operator= (const SDLSoundSystem&) = delete;
	SDLSoundSystem& operator= (const SDLSoundSystem&&) = delete;

	void Update() override;
	void PlaySound(int soundID) override;
	void AddSound(const std::string& filename) override;

private:
	enum class SoundType {
		Sound,
		Music
	};

	struct Event {
		SoundType type;
		int soundId;
	};

	std::unordered_map<int, Mix_Chunk*> m_SoundList{};
	std::vector<Mix_Chunk*> m_SoundsToPlay;
	std::mutex m_Mutex;
	std::condition_variable m_ConditionVariable;
	std::queue<Event> m_EventQueue{};
	bool m_Quit = false;
	std::jthread m_Thread;
	int m_Volume{ 10 };
};