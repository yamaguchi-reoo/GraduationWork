#include <DxLib.h>
#include "Scene/SceneManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	//ローカル変数定義
	SceneManager* scene_manager = nullptr;
	int result = 0;

	try
	{
		//オブジェクトの生成
		scene_manager = new SceneManager();

		//初期化処理
		scene_manager->Initialize();

		//更新処理
		scene_manager->Update();

		//描画処理
		//scene_manager->Draw();

		//終了時処理
		scene_manager->Finalize();
	}
	catch (const char* error_log)
	{
		//エラー情報を出力
		OutputDebugString(error_log);
		result = -1;
	}

	//シーンマネージャーを生成していたら、削除する
	if (scene_manager != nullptr)
	{
		delete scene_manager;
		scene_manager = nullptr;
	}

	//終了処理を通知
	return result;
}