#pragma once
#include <map> 
#include <string> 
#include <vector>

enum class SoundCategory
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

	// --- Enemy ---

	// --- GameMain ---
	GAME_MAIN_BGM,


};
struct SoundData
{
	int handle;
	SoundCategory category;
	int default_volume;
	bool loop;
};

class SoundManager
{
private:
	// 音声データを管理するマップ
	std::map<SoundID, SoundData> sounds;

	static SoundManager* instance;

	SoundManager() = default;
	~SoundManager() = default;

public:
	static SoundManager* GetInstance();
	static void DeleteInstance();

	void LoadSounds();

	void Play(SoundID id);
	void Stop(SoundID id);

	void StopByCategory(SoundCategory category);
};

