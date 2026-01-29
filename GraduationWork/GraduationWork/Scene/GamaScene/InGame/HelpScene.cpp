#include "HelpScene.h"
#include "../../../Utility/InputManager.h"
#include "../../SceneManager.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
	// カラー定義（ダーク寄り）
	const unsigned int COLOR_TITLE = GetColor(230, 230, 230);
	const unsigned int COLOR_TEXT = GetColor(250, 250, 250);
	const unsigned int COLOR_SUBTEXT = GetColor(140, 140, 140);
	const unsigned int COLOR_SELECT = GetColor(255, 255, 0);
	const unsigned int COLOR_LINE = GetColor(70, 70, 70);
	const unsigned int COLOR_BG = GetColor(20, 20, 20);
	const unsigned int COLOR_FRAME = GetColor(60, 60, 60);
}

HelpScene::HelpScene()
	: current_category(HelpCategory::Control)
	, cursor_index(0)
	, help_font(-1)
{
}

HelpScene::~HelpScene()
{
}

void HelpScene::Initialize()
{
	// 日本語説明用フォント（ダーク向け）
	AddFontFile("Resource/Font/NotoSansJP/NotoSansJP-Regular.otf");

	help_font = CreateFontToHandle(
		"Noto Sans JP",
		24,
		2,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	current_category = HelpCategory::Control;
	cursor_index = 0;
}

eSceneType HelpScene::Update()
{
	InputManager* input = InputManager::GetInstance();

	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		cursor_index = max(0, cursor_index - 1);
	}

	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		cursor_index = min(
			static_cast<int>(HelpCategory::Max) - 1,
			cursor_index + 1
		);
	}

	current_category = static_cast<HelpCategory>(cursor_index);

	// 戻る
	if (input->GetButtonDown(XINPUT_BUTTON_B))
	{
		return eSceneType::TITLE;
	}

	return eSceneType::HELP;
}

void HelpScene::Draw()
{
	// ===== タイトル =====
	DrawFormatStringToHandle(
		500, 50,
		COLOR_TITLE,
		SceneManager::titleFont,
		"HELP"
	);

	// ===== 左メニュー =====
	const int baseY = 150;

	DrawFormatStringToHandle(
		120, baseY,
		cursor_index == 0 ? COLOR_SELECT : COLOR_TEXT,
		SceneManager::menuFont,
		"CONTROL"
	);

	DrawFormatStringToHandle(
		120, baseY + 70,
		cursor_index == 1 ? COLOR_SELECT : COLOR_TEXT,
		SceneManager::menuFont,
		"GIMMICK"
	);

	DrawFormatStringToHandle(
		120, baseY + 140,
		cursor_index == 2 ? COLOR_SELECT : COLOR_TEXT,
		SceneManager::menuFont,
		"ITEM"
	);

	// カーソル
	DrawFormatStringToHandle(
		90, baseY + cursor_index * 70,
		COLOR_SELECT,
		SceneManager::menuFont,
		">"
	);

	// ===== 区切り線 =====
	DrawLine(400, 120, 400, 600, COLOR_LINE);

	// ===== 説明パネル =====
	DrawBox(530, 120, 1060, 560, COLOR_BG, TRUE);
	DrawBox(530, 120, 1060, 560, COLOR_FRAME, FALSE);

	// ===== 中身 =====
	switch (current_category)
	{
	case HelpCategory::Control:
		DrawControlHelp();
		break;

	case HelpCategory::Gimmick:
		DrawGimmickHelp();
		break;

	case HelpCategory::Item:
		DrawItemHelp();
		break;
	}
}

void HelpScene::Finalize()
{
	if (help_font != -1)
	{
		DeleteFontToHandle(help_font);
		help_font = -1;
	}
}

void HelpScene::DrawControlHelp()
{
	const int x = 560;
	const int y = 150;

	int lineY = y;

	// MOVE
	DrawFormatStringToHandle(x, lineY, COLOR_TEXT, SceneManager::menuFont, "MOVE");
	DrawFormatStringToHandle(x + 20, lineY + 30, COLOR_SUBTEXT, help_font, "← →");

	// JUMP
	DrawFormatStringToHandle(x, lineY + 80, COLOR_TEXT, SceneManager::menuFont, "JUMP");
	DrawFormatStringToHandle(x + 20, lineY + 110, COLOR_SUBTEXT, help_font, "A");

	// ATTACK
	DrawFormatStringToHandle(x, lineY + 160, COLOR_TEXT, SceneManager::menuFont, "ATTACK");
	DrawFormatStringToHandle(x + 20, lineY + 190, COLOR_SUBTEXT, help_font, "B");

	// SHADOW
	DrawFormatStringToHandle(x, lineY + 240, COLOR_TEXT, SceneManager::menuFont, "SHADOW");
	DrawFormatStringToHandle(x + 20, lineY + 270, COLOR_SUBTEXT, help_font, "RT");
}

void HelpScene::DrawGimmickHelp()
{
	const int x = 560;
	const int y = 150;

	// スイッチプレート
	DrawFormatStringToHandle(
		x, y,
		COLOR_TEXT,
		SceneManager::menuFont,
		"PRESS PLATE"
	);

	DrawFormatStringToHandle(
		x, y + 30,
		COLOR_SUBTEXT,
		help_font,
		"踏むことで装置が起動する"
	);

	// 光トラップ
	DrawFormatStringToHandle(
		x, y + 90,
		COLOR_TEXT,
		SceneManager::menuFont,
		"LIGHT TRAP"
	);

	DrawFormatStringToHandle(
		x, y + 125,
		COLOR_SUBTEXT,
		help_font,
		"影では侵入できない"
	);
}

void HelpScene::DrawItemHelp()
{
	const int x = 560;
	const int y = 150;

	// 回復アイテム
	DrawFormatStringToHandle(
		x, y,
		COLOR_TEXT,
		SceneManager::menuFont,
		"HEAL POTION"
	);

	// 回復アイテム
	DrawFormatStringToHandle(
		x, y + 35,
		COLOR_SUBTEXT,
		help_font,
		"体力を回復する"
	);

	// シャドウ回復
	DrawFormatStringToHandle(
		x, y + 95,
		COLOR_TEXT,
		SceneManager::menuFont,
		"SHADOW POTION"
	);


	DrawFormatStringToHandle(
		x, y + 155,
		COLOR_SUBTEXT,
		help_font,
		"影状態の制限時間を延ばす"
	);
}

eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::HELP;
}
