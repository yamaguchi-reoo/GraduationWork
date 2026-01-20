#include "SoundManager.h"
#include "ResourceManager.h"
#include <DxLib.h>

void SoundManager::LoadSounds()
{
	auto* rm = ResourceManager::GetInstance();
	// ƒvƒŒƒCƒ„[ŠÖ˜A‚Ì‰¹º“Ç‚Ýž‚Ý
	sounds[SoundID::SHADOW_ATTACK] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1620462__“‚ðU‚é‰¹_Žã.wav"),
		SoundType::SE,
		90,
		false
	};

	//sound_map[SoundType::REAL_ATTACK] = { rm->GetSE("Resource/Sounds/SE/Player/AS_1620462__“‚ðU‚é‰¹_Žã.wav") };
	sounds[SoundID::JUMP] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_952655_ƒWƒƒƒ“ƒv‰¹A‚O‚V.wav"),
		SoundType::SE,
		60,
		false
	};

	sounds[SoundID::DAMAGE] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1155632_ƒ_ƒ[ƒW‰¹.wav"),
		SoundType::SE,
		60,
		false
	};
	//sound_map[SoundType::WALK] = { rm->GetSE("") };

	sounds[SoundID::LAND] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_22106_‘I‘ði8bit•—j.wav"),
		SoundType::SE,
		60,
		false
	};
	//sound_map[SoundType::DEATH] = { rm->GetSE("") };
	sounds[SoundID::HEAL] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_968862_Šð‚µ‚¢Š´î‚Ì‚æ‚¤‚Èã¸‰¹‚Q^ƒn[ƒg^‰ñ•œ.wav"),
		SoundType::SE,
		80,
		false
	};

	sounds[SoundID::STATE_CHANGE] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1383683.wav"),
		SoundType::SE,
		90,
		false
	};
}

void SoundManager::Play(SoundID id)
{
	auto it = sounds.find(id);
	if (it == sounds.end()) return;

	auto& s = it->second;

	ChangeVolumeSoundMem(s.default_volume, s.handle);
	StopSoundMem(s.handle);

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
