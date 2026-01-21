#pragma once
#include <map> 
#include <string> 
#include <vector>

enum class SoundType
{
	SE,
	BGM,
};

enum class SoundID
{
	// --- Player ---
	SHADOW_ATTACK,
	REAL_ATTACK,
	JUMP,
	DAMAGE,
	WALK,
	LAND,
	DEATH,
	HEAL,
	STATE_CHANGE,
};
struct SoundData
{
	int handle;
	SoundType type;
	int default_volume;
	bool loop;
};

class SoundManager
{
private:
	// 音声データを管理するマップ
	std::map<SoundID, SoundData> sounds;

public:
	void LoadSounds();

	void Play(SoundID id);
	void Stop(SoundID id);

	void SetVolume(SoundID id, int volume);
};

