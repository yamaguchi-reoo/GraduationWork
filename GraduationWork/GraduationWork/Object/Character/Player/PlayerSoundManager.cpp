#include "PlayerSoundManager.h"
#include "../../../Utility/ResourceManager.h"
#include <DxLib.h>

void SoundManager::LoadSounds()
{
	auto* rm = ResourceManager::GetInstance();

	//sound_map[SoundType::ATTACK] = { rm->GetSE("") };
	//sound_map[SoundType::JUMP] = { rm->GetSE("") };
	//sound_map[SoundType::DAMAGE] = { rm->GetSE("") };
	//sound_map[SoundType::WALK] = { rm->GetSE("") };
	//sound_map[SoundType::LAND] = { rm->GetSE("") };
	//sound_map[SoundType::DEATH] = { rm->GetSE("") };
	sound_map[SoundType::STATE_CHANGE] = { rm->GetSE("Resource/Sounds/AS_1383683.wav") };
}

void SoundManager::PlaySoundSE(SoundType sound_type, int volume, bool flg)
{
	auto it = sound_map.find(sound_type);
	if (it == sound_map.end() || it->second.empty()) return;

	int h = it->second[0];
	ChangeVolumeSoundMem(volume, h);
	// ループ再生をする前に明示的に停止
	StopSoundMem(h);

	if (flg)
	{
		// 1回だけ再生
		PlaySoundMem(h, DX_PLAYTYPE_BACK);
	}
	else
	{
		// ループ再生
		SetLoopStartTimePosSoundMem(0, h);  // ループ位置を最初に戻す
		PlaySoundMem(h, DX_PLAYTYPE_LOOP);
	}
}

void SoundManager::StopSound(SoundType sound_type)
{
	auto it = sound_map.find(sound_type);
	if (it == sound_map.end() || it->second.empty()) return;

	StopSoundMem(it->second[0]);
}
