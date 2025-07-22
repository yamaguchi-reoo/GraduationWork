#include <DxLib.h>
#include "Scene/SceneManager.h"

#include "common.h"
#include "Utility/UtilityList.h"
#include <string>


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // �E�C���h�E�̃T�C�Y
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* manager = nullptr;

	//fps����
	FpsControl* FPSC = new FpsControl(FRAMERATE, 800);

	try {
		manager = new SceneManager();

		manager->Initialize();

		InputManager* input = InputManager::GetInstance();

		while (ProcessMessage() != -1)
		{
			//���͏��̍X�V
			input->Update();

			//��ʂ̏�����
			ClearDrawScreen();

			//��ʂ̍X�V
			manager->Update();

			//�`�揈��
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

	//�C���X�^���X�̍폜
	InputManager::DeleteInstance();
	if (manager != nullptr)
	{
		manager->Finalize();
		delete manager;
	}

	DxLib_End(); // DX���C�u�����g�p�̏I������
	return 0; // �\�t�g�̏I��
}