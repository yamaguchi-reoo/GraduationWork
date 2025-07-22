#include <DxLib.h>
#include "Scene/SceneManager.h"

#include "common.h"
#include "Utility/UtilityList.h"
#include <string>


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // ウインドウのサイズ
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* manager = nullptr;

	//fps制御
	FpsControl* FPSC = new FpsControl(FRAMERATE, 800);

	try {
		manager = new SceneManager();

		manager->Initialize();

		InputManager* input = InputManager::GetInstance();

		while (ProcessMessage() != -1)
		{
			//入力情報の更新
			input->Update();

			//画面の初期化
			ClearDrawScreen();

			//画面の更新
			manager->Update();

			//描画処理
			manager->Draw();


			FPSC->All();
#ifdef _DEBUG
			FPSC->Disp();
#endif
			ScreenFlip();
		}

	}
	catch (std::string& error_text)
	{
		OutputDebugString(error_text.c_str());

		return -1;
	}

	//インスタンスの削除
	InputManager::DeleteInstance();
	if (manager != nullptr)
	{
		manager->Finalize();
		delete manager;
	}

	DxLib_End(); // DXライブラリ使用の終了処理
	return 0; // ソフトの終了
}