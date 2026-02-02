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
	const unsigned int COLOR_SUBTEXT = GetColor(185, 185, 185);
	const unsigned int COLOR_SELECT = GetColor(255, 255, 0);
	const unsigned int COLOR_LINE = GetColor(70, 70, 70);
	const unsigned int COLOR_BG = GetColor(20, 20, 20);
	const unsigned int COLOR_FRAME = GetColor(60, 60, 60);
}

// ==============================
// ctor / dtor
// ==============================
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
	AddFontFile("Resource/Font/NotoSansJP/NotoSansJP-Regular.otf");

	help_font = CreateFontToHandle(
		"Noto Sans JP",
		24,
		2,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	cursor_index = 0;
	current_category = HelpCategory::Control;
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

	// BACK 選択中に B でタイトルへ
	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		if (static_cast<HelpCategory>(cursor_index) == HelpCategory::Back)
		{
			return eSceneType::TITLE;
		}
	}

	// BACK のときは一つ上（ITEM）を表示
	if (static_cast<HelpCategory>(cursor_index) == HelpCategory::Back)
	{
		current_category = HelpCategory::Item;
	}
	else
	{
		current_category = static_cast<HelpCategory>(cursor_index);
	}

	return eSceneType::HELP;
}

void HelpScene::Draw()
{
	// ===== タイトル =====
	DrawFormatStringToHandle(
		120, 50,
		COLOR_TITLE,
		SceneManager::titleFont,
		"HELP"
	);

	// ===== 左メニュー =====
	const int baseY = 200;
	const int stepY = 70;
	const int backY = 520;

	DrawFormatStringToHandle(
		120, baseY,
		cursor_index == 0 ? COLOR_SELECT : COLOR_TEXT,
		SceneManager::menuFont,
		"CONTROL"
	);

	DrawFormatStringToHandle(
		120, baseY + stepY,
		cursor_index == 1 ? COLOR_SELECT : COLOR_TEXT,
		SceneManager::menuFont,
		"GIMMICK"
	);

	DrawFormatStringToHandle(
		120, baseY + stepY * 2,
		cursor_index == 2 ? COLOR_SELECT : COLOR_TEXT,
		SceneManager::menuFont,
		"ITEM"
	);

	DrawFormatStringToHandle(
		120, backY,
		cursor_index == 3 ? COLOR_SELECT : COLOR_TEXT,
		SceneManager::menuFont,
		"BACK"
	);

	// カーソル位置
	int cursorY = (cursor_index <= 2)
		? baseY + cursor_index * stepY
		: backY;

	DrawFormatStringToHandle(
		90,
		cursorY,
		COLOR_SELECT,
		SceneManager::menuFont,
		">"
	);

	// ===== 区切り線 =====
	DrawLine(400, 120, 400, 600, COLOR_LINE);

	// ===== 説明パネル =====
	DrawBox(530, 120, 1160, 590, COLOR_BG, TRUE);
	DrawBox(530, 120, 1160, 590, COLOR_FRAME, FALSE);

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

	default:
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

// ==============================
// Help Contents
// ==============================
void HelpScene::DrawControlHelp()
{
	const int x = 560;
	const int y = 150;

	DrawFormatStringToHandle(x, y, COLOR_TEXT, SceneManager::menuFont, "MOVE");
	DrawFormatStringToHandle(x + 20, y + 30, COLOR_SUBTEXT, help_font, "← →");

	DrawFormatStringToHandle(x, y + 80, COLOR_TEXT, SceneManager::menuFont, "JUMP");
	DrawFormatStringToHandle(x + 20, y + 110, COLOR_SUBTEXT, help_font, "A");

	DrawFormatStringToHandle(x, y + 160, COLOR_TEXT, SceneManager::menuFont, "ATTACK");
	DrawFormatStringToHandle(x + 20, y + 190, COLOR_SUBTEXT, help_font, "B");

	DrawFormatStringToHandle(x, y + 240, COLOR_TEXT, SceneManager::menuFont, "SHADOW");
	DrawFormatStringToHandle(x + 20, y + 270, COLOR_SUBTEXT, help_font, "RT");
}

void HelpScene::DrawGimmickHelp()
{
	const int x = 560;
	const int y = 150;
	const int blockHeight = 90;
	int currentY = y;

	auto drawBlock = [&](const char* title, const char* text)
		{
			DrawFormatStringToHandle(x, currentY, COLOR_TEXT, SceneManager::menuFont, title);
			DrawFormatStringToHandle(x, currentY + 35, COLOR_SUBTEXT, help_font, text);
			currentY += blockHeight;
		};

	drawBlock("WALL", "影状態のみ通れる障害物");
	drawBlock("PRESS PLATE", "踏むことで装置が起動する");
	drawBlock("LIGHT TRAP", "影では侵入できない");
	drawBlock("PUSH BLOCK", "押して移動させることができる");
	drawBlock("INVISIBLE FLOOR", "影状態でないと見えない床");
}

void HelpScene::DrawItemHelp()
{
	const int x = 560;
	const int y = 150;

	DrawFormatStringToHandle(x, y, COLOR_TEXT, SceneManager::menuFont, "HEAL POTION");
	DrawFormatStringToHandle(x, y + 35, COLOR_SUBTEXT, help_font, "体力を回復する");

	DrawFormatStringToHandle(x, y + 95, COLOR_TEXT, SceneManager::menuFont, "SHADOW POTION");
	DrawFormatStringToHandle(x, y + 125, COLOR_SUBTEXT, help_font, "影状態の制限時間を延ばす");
}

// ==============================
eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::HELP;
}
