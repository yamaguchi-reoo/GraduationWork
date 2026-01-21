#include "SoundManager.h"
#include "ResourceManager.h"
#include <DxLib.h>

SoundManager* SoundManager::instance = nullptr;
SoundManager* SoundManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SoundManager();
		instance->LoadSounds();
	}
	return instance;
}

void SoundManager::DeleteInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void SoundManager::LoadSounds()
{
	auto* rm = ResourceManager::GetInstance();

	// プレイヤー関連の音声読み込み
	sounds[SoundID::SHADOW_ATTACK] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1620462__刀を振る音_弱.wav"),
		SoundCategory::SE,
		90,
		false
	};
	//sound_map[SoundType::REAL_ATTACK] = { rm->GetSE("Resource/Sounds/SE/Player/AS_1620462__刀を振る音_弱.wav") };
	sounds[SoundID::JUMP] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_952655_ジャンプ音A０７.wav"),
		SoundCategory::SE,
		60,
		false
	};
	sounds[SoundID::DAMAGE] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1155632_ダメージ音.wav"),
		SoundCategory::SE,
		60,
		false
	};
	//sound_map[SoundType::WALK] = { rm->GetSE("") };
	sounds[SoundID::LAND] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_22106_選択（8bit風）.wav"),
		SoundCategory::SE,
		60,
		false
	};
	//sound_map[SoundType::DEATH] = { rm->GetSE("") };
	sounds[SoundID::HEAL] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_968862_嬉しい感情のような上昇音２／ハート／回復.wav"),
		SoundCategory::SE,
		80,
		false
	};
	sounds[SoundID::STATE_CHANGE] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1383683.wav"),
		SoundCategory::SE,
		90,
		false
	};

	//エネミー関連の音声読み込み


	// ゲームメイン関連の音声読み込み
	sounds[SoundID::GAME_MAIN_BGM] = {
		rm->GetSE("Resource/Sounds/BGM/GameMain/AS_69632_悲しげのある洞窟イメージのチップチューン.wav"),
		SoundCategory::BGM,
		90,
		true
	};

}

void SoundManager::Play(SoundID id)
{
	auto it = sounds.find(id);
	if (it == sounds.end()) return;

	auto& s = it->second;

	// すでに再生中なら何もしない（ループ音用）
	if (s.loop && CheckSoundMem(s.handle) == 1)
		return;

	ChangeVolumeSoundMem(s.default_volume, s.handle);

	PlaySoundMem(
		s.handle,
		s.loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK
	);
}


void SoundManager::Stop(SoundID id)
{
	auto it = sounds.find(id);
	if (it == sounds.end()) return;

	StopSoundMem(it->second.handle);
}

void SoundManager::StopByCategory(SoundCategory category)
{
	for (auto& pair : sounds)
	{
		if (pair.second.category == category)
		{
			StopSoundMem(pair.second.handle);
		}
	}
}
