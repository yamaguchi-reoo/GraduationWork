#include "SceneManager.h"
#include "DxLib.h"
#include <windows.h>

#include "GamaScene/InGame/InGameScene.h"
#include "GamaScene/InGame/TitleScene.h"

#include "../common.h"
#include "../Utility/UtilityList.h"

SceneManager::SceneManager() :current_scene(nullptr), fps_control()
{
}

SceneManager::~SceneManager()
{

	this->Finalize();
}

void SceneManager::Initialize()
{
	//�E�B���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);

#ifdef _DEBUG
	//�f�o�b�O�p�̉�ʃT�C�Y�ݒ�
	SetGraphMode(DEBUG_SCREEN_WIDTH, DEBUG_SCREEN_HEIGHT, 32); // �f�o�b�O�p�̉�ʃT�C�Y
#else
	//��ʃT�C�Y�ݒ�
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // �E�C���h�E�̃T�C�Y

#endif // DEBUG

	SetWaitVSyncFlag(FALSE); // VSync�҂��𖳌����i�t���[�����[�g����̂��߁j

	//DX���C�u�����̏�����
	if (DxLib_Init() == -1)
	{
		throw("DX���C�u�������������ł��܂���ł���\n");
	}

	//����ʂ���`����n�߂�
	SetDrawScreen(DX_SCREEN_BACK);

	//�^�C�g����ʃV�[������J�n����
	ChangeScene(eSceneType::TITLE);

	fps_control.Initialize();;
}

void SceneManager::Update()
{

	while (ProcessMessage() != -1)
	{
		fps_control.Update(); // �t���[�����[�g����̍X�V

		//���͏��̍X�V
		InputManager::GetInstance()->Update();

		// �X�V���`��
		eSceneType next_scene_type = current_scene->Update();
		this->Draw();

		// �V�[���؂�ւ�
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
	//���͋@�\�̃C���X�^���X���폜
	InputManager::DeleteInstance();
	//DX���C�u�����̏I������
	DxLib_End();
}

void SceneManager::Draw()
{
	//��ʂ̏�����
	ClearDrawScreen();

	//�V�[���̕`�揈��
	current_scene->Draw();

	fps_control.Draw(); // �t���[�����[�g����̕`��

	//����ʂ̓��e��\��ʂɔ��f����
	ScreenFlip();
}

void SceneManager::ChangeScene(eSceneType type)
{
	SceneBase* new_scene = CreateScene(type);

	//�G���[�`�F�b�N
	if (new_scene == nullptr)
	{
		throw("\n�V�����V�[���̐������o���܂���ł���\n");
	}

	if (DxLib_Init() == -1)
	{
		MessageBox(nullptr, "DX���C�u�������������ł��܂���ł���", "Error", MB_ICONERROR);
		return;
	}

	//���݂̃V�[���̏I������
	if (current_scene != nullptr)
	{

		current_scene->Finalize();
		delete current_scene;
	}

	//�V�����V�[���̏�����
	new_scene->Initialize();
	current_scene = new_scene;
}

SceneBase* SceneManager::CreateScene(eSceneType type)
{
	switch (type)
	{
	case eSceneType::TITLE:
		return dynamic_cast<SceneBase*>(new TitleScene()); // �^�C�g���V�[���̐���
	case eSceneType::GAME_MAIN:
		return new InGameScene(); // �Q�[�����C���V�[���̐���
	default:
		return nullptr; // ���m�̃V�[���^�C�v�̏ꍇ��nullptr��Ԃ�
	}
}

