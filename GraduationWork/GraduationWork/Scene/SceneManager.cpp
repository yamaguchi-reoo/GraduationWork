#include "SceneManager.h"
#include "DxLib.h"
#include <windows.h>

#include "GamaScene/InGame/InGameScene.h"
#include "GamaScene/InGame/TitleScene.h"
#include "GamaScene/InGame/GameOverScene.h"
#include "GamaScene/InGame/GameClear.h"

#include "../common.h"
#include "../Utility/UtilityList.h"

int SceneManager::font = -1;
int SceneManager::titleFont = -1;
int SceneManager::menuFont = -1;

SceneManager::SceneManager() :current_scene(nullptr), fps_control()
{
}

SceneManager::~SceneManager()
{

	this->Finalize();
}

void SceneManager::Initialize()
{
	//ウィンドウモードで起動
	ChangeWindowMode(TRUE);

	//デバッグ用の画面サイズ設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // デバッグ用の画面サイズ

	SetWaitVSyncFlag(FALSE); // VSync待ちを無効化（フレームレート制御のため）

	//DXライブラリの初期化
	if (DxLib_Init() == -1)
	{
		throw("DXライブラリが初期化できませんでした\n");
	}

	// ここでフォントファイルを登録する
	AddFontFile("Resource/Font/party-bold/party-bold.ttf");

	// 登録したフォントをハンドル化
	font = CreateFontToHandle("Party Bold", 32, 6, DX_FONTTYPE_ANTIALIASING_4X4);

	titleFont = CreateFontToHandle(
		"Party Bold",
		52,
		3,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	menuFont = CreateFontToHandle(
		"Party Bold",
		32,
		2,
		DX_FONTTYPE_ANTIALIASING_4X4
	);

	//裏画面から描画を始める
	SetDrawScreen(DX_SCREEN_BACK);

	//タイトル画面シーンから開始する
	ChangeScene(eSceneType::TITLE/*GAME_MAIN*/);

	fps_control.Initialize();

	InputManager::GetInstance();
	ResourceManager::GetInstance();
	SoundManager::GetInstance()->LoadSounds();
}

void SceneManager::Update()
{

	while (ProcessMessage() != -1)
	{
		fps_control.Update(); // フレームレート制御の更新

		//入力情報の更新
		InputManager::GetInstance()->Update();;

		// 更新＆描画
		eSceneType next_scene_type = current_scene->Update();
		this->Draw();

		// シーン切り替え
		if (next_scene_type != current_scene->GetNowSceneType())
		{
			if (next_scene_type == eSceneType::EXIT) {
				break; 
			}
			ChangeScene(next_scene_type);
		}

		if (InputManager::GetInstance()->GetKeyUp(KEY_INPUT_ESCAPE))
		{
			break;
		}

	}
}

void SceneManager::Finalize()
{
	//入力機能のインスタンスを削除
	InputManager::DeleteInstance();
	ResourceManager::DeleteInstance();
	SoundManager::DeleteInstance();

	//DXライブラリの終了処理
	DxLib_End();
}

void SceneManager::Draw()
{
	//画面の初期化
	ClearDrawScreen();

	//シーンの描画処理
	current_scene->Draw();

	fps_control.Draw(); // フレームレート制御の描画

	//裏画面の内容を表画面に反映する
	ScreenFlip();
}

void SceneManager::ChangeScene(eSceneType category)
{
	SceneBase* new_scene = CreateScene(category);

	//エラーチェック
	if (new_scene == nullptr)
	{
		throw("\n新しいシーンの生成が出来ませんでした\n");
	}

	if (DxLib_Init() == -1)
	{
		MessageBox(nullptr, "DXライブラリが初期化できませんでした", "Error", MB_ICONERROR);
		return;
	}

	//現在のシーンの終了処理
	if (current_scene != nullptr)
	{

		current_scene->Finalize();
		delete current_scene;
	}

	//新しいシーンの初期化
	new_scene->Initialize();
	current_scene = new_scene;
}

SceneBase* SceneManager::CreateScene(eSceneType category)
{
	switch (category)
	{
	case eSceneType::TITLE:
		return dynamic_cast<SceneBase*>(new TitleScene()); // タイトルシーンの生成
	case eSceneType::GAME_MAIN:
		return new InGameScene(); // ゲームメインシーンの生成
	case eSceneType::GAMEOVER:
		return new GameOverScene(); // ゲームメインシーンの生成
	case eSceneType::GAMECLEAR:
		return new GameClear(); // ゲームメインシーンの生成
	default:
		return nullptr; // 未知のシーンタイプの場合はnullptrを返す
	}
}

