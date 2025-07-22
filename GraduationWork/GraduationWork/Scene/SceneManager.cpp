#include "SceneManager.h"
#include "DxLib.h"

SceneManager::SceneManager() :current_scene(nullptr)
{
}

SceneManager::~SceneManager()
{
	this->Finalize();
}

void SceneManager::Initialize()
{
	ChangeScene(eSceneType::TITLE);
}

void SceneManager::Update()
{
	if (current_scene == nullptr) return;

	eSceneType next_scene_type = current_scene->Update();

	if (next_scene_type != current_scene->GetNowSceneType())
	{
		ChangeScene(next_scene_type);
	}
}

void SceneManager::Finalize()
{
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

void SceneManager::Draw()
{
	if (current_scene) current_scene->Draw();
}

void SceneManager::ChangeScene(eSceneType type)
{
	SceneBase* new_scene = CreateScene(type);

	//エラーチェック
	if (new_scene == nullptr)
	{
		throw("\n新しいシーンの生成が出来ませんでした\n");
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

SceneBase* SceneManager::CreateScene(eSceneType type)
{
	switch (type)
	{
	case eSceneType::TITLE:
		//return dynamic_cast<SceneBase*>(new TitleScene()); // タイトルシーンの生成
	case eSceneType::GAME_MAIN:
		//return new GameMainScene(); // ゲームメインシーンの生成
	default:
		return nullptr; // 未知のシーンタイプの場合はnullptrを返す
	}
}

