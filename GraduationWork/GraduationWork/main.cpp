#include <DxLib.h>
#include "Scene/SceneManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	// DxLib������
	if (DxLib_Init() == -1) return -1;

	// �����Ńt�H���g�t�@�C����o�^����
	AddFontFile("Resource/font/party-bold/party-bold.ttf");

	// �o�^�����t�H���g���n���h����
	int font = CreateFontToHandle("MyFont Regular", 32, 6, DX_FONTTYPE_ANTIALIASING_4X4);

	//���[�J���ϐ���`
	SceneManager* scene_manager = nullptr;
	int result = 0;



	try
	{
		//�I�u�W�F�N�g�̐���
		scene_manager = new SceneManager();

		//����������
		scene_manager->Initialize();

		//�X�V����
		scene_manager->Update();

		//�`�揈��
		//scene_manager->Draw();

		//�I��������
		scene_manager->Finalize();
	}
	catch (const char* error_log)
	{
		//�G���[�����o��
		OutputDebugString(error_log);
		result = -1;
	}

	//�V�[���}�l�[�W���[�𐶐����Ă�����A�폜����
	if (scene_manager != nullptr)
	{
		delete scene_manager;
		scene_manager = nullptr;
	}

	//�I��������ʒm
	return result;
}