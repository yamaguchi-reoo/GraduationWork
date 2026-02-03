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

	// --- プレイヤー関連の音声読み込み --- 

	//影状態での攻撃音
	sounds[SoundID::SHADOW_ATTACK] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1620462__刀を振る音_弱.wav"),
		SoundCategory::SE,
		140,
		false
	};
	//実状態での攻撃音
	sounds[SoundID::REAL_ATTACK] = {
		rm->GetSE("Resource/Sounds/SE/Player/AS_372332__風切り音.wav"),
		SoundCategory::SE,
		180,
		false
	};

	//ジャンプ音
	sounds[SoundID::JUMP] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_952655_ジャンプ音A０７.wav"),
		SoundCategory::SE,
		80,
		false
	};
	//ダメージ音
	sounds[SoundID::DAMAGE] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_109147_ぽよん_選択_ジャンプ_踏む_アクション.wav"),
		SoundCategory::SE,
		120,
		false
	};
	//歩行音
	sounds[SoundID::WALK] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_115562_かわいい足音_動作.wav"),
		SoundCategory::SE,
		40,
		false
	};
	 
	//着地音
	sounds[SoundID::LAND] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_172804_ピョイ（ジャンプ・アクション・攻撃回避）.wav"),
		SoundCategory::SE,
		70,
		false
	};

	//影状態での死亡音
	sounds[SoundID::SHADOW_DEATH] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1090444_倒した敵・魔物が消える音.wav"),
		SoundCategory::SE,
		70,
		false
	};

	//実状態での死亡音
	sounds[SoundID::REAL_DEATH] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_964708_小動物の鳴き声_01.wav"),
		SoundCategory::SE,
		140,
		false
	};

	//回復音
	sounds[SoundID::HEAL] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_968862_嬉しい感情のような上昇音２／ハート／回復.wav"),
		SoundCategory::SE,
		160,
		false
	};
	//状態変化音
	sounds[SoundID::STATE_CHANGE] = { 
		rm->GetSE("Resource/Sounds/SE/Player/AS_1383683.wav"),
		SoundCategory::SE,
		140,
		false
	};

	// --- エネミー関連の音声読み込み ---
	//エネミー歩行音
	sounds[SoundID::ENEMY_WALK] = {
		rm->GetSE("Resource/Sounds/SE/Enemy/AS_411966_ヘルハウンド_ステップ01.wav"),
		SoundCategory::SE,
		80,
		false
	};

	//エネミー攻撃音
	sounds[SoundID::ENEMY_ATTACK] = {
		rm->GetSE("Resource/Sounds/SE/Enemy/AS_104552_犬が吠える（ワン）.wav"),
		SoundCategory::SE,
		120,
		false
	};

	//エネミー死亡音
	sounds[SoundID::ENEMY_DEATH] = {
		rm->GetSE("Resource/Sounds/SE/Enemy/AS_770085_犬の泣き声b.wav"),
		SoundCategory::SE,
		90,
		false
	};

	// --- ギミック関連の音声読み込み ---
	//感圧板ON音
	sounds[SoundID::PLATE_ON] = {
		rm->GetSE("Resource/Sounds/SE/Gimmick/AS_308257_スイッチ音_4.wav"),
		SoundCategory::SE,
		200,
		false
	};


	// --- ゲームメイン関連の音声読み込み ---
	// ゲームメインBGM
	sounds[SoundID::GAME_MAIN_BGM] = {
		rm->GetSE("Resource/Sounds/BGM/GameMain/AS_69632_悲しげのある洞窟イメージのチップチューン.wav"),
		SoundCategory::BGM,
		150,
		true
	};

	// ゲームオーバーBGM
	sounds[SoundID::GAME_OVER_BGM] = {
		rm->GetSE("Resource/Sounds/BGM/GameMain/AS_205350_8bit系＿勇壮なゲームオーバー.wav"),
		SoundCategory::BGM,
		100,
		false
	};

	// ゲームクリアBGM
	sounds[SoundID::GAME_CLEAR_BGM] = {
		rm->GetSE("Resource/Sounds/BGM/GameMain/AS_1487479_かわいいちょっとミステリアポップなbgm.wav"),
		SoundCategory::BGM,
		140,
		true
	};

	//ゲームクリアSE
	sounds[SoundID::GAME_CLEAR_SE] = {
		rm->GetSE("Resource/Sounds/BGM/GameMain/AS_1026732_ファミコン風ゲームクリアの効果音.wav"),
		SoundCategory::SE,
		70,
		false
	};

	//ゲームクリア選択音
	sounds[SoundID::GAME_CLEAR_SELECT] = {
	rm->GetSE("Resource/Sounds/BGM/GameMain/AS_890902_決定／クリック／選択音（ピコンッ）.wav"),
		SoundCategory::BGM,
		85,
		false
	};

	// タイトルBGM
	sounds[SoundID::TITLE_BGM] = {
		rm->GetSE("Resource/Sounds/BGM/Title/Title.wav"),
		SoundCategory::BGM,
		140,
		true
	};

	// カーソル移動SE
	sounds[SoundID::CURSOR] = {
		rm->GetSE("Resource/Sounds/SE/Title/AS_150317_カチッ（装着／スイッチ／セット）.wav"),
		SoundCategory::BGM,
		110,
		false
	};

	// 決定SE
	sounds[SoundID::PUSH] = {
		rm->GetSE("Resource/Sounds/SE/Title/AS_889621_ホワン⑤（魔法・テロップ・スタート音）.wav"),
		SoundCategory::BGM,
		120,
		false
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
