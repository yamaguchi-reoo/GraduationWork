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

	//�G���[�`�F�b�N
	if (new_scene == nullptr)
	{
		throw("\n�V�����V�[���̐������o���܂���ł���\n");
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
		//return dynamic_cast<SceneBase*>(new TitleScene()); // �^�C�g���V�[���̐���
	case eSceneType::GAME_MAIN:
		//return new GameMainScene(); // �Q�[�����C���V�[���̐���
	default:
		return nullptr; // ���m�̃V�[���^�C�v�̏ꍇ��nullptr��Ԃ�
	}
}

