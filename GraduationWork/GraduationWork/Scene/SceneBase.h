#pragma once
#include <vector>
#include "../Object/GameObject.h"

enum class eSceneType
{
	TITLE,
	GAME_MAIN,
};

class SceneBase
{
protected:
	std::vector<GameObject*>objects;
	Vector2D camera_location;
public:
	//�R���X�g���N�^
	SceneBase();
	//�f�X�g���N�^
	virtual ~SceneBase();

	virtual void Initialize();		//����������
	virtual eSceneType Update();	//�X�V����
	virtual void Draw();		//�`�揈��
	virtual void Finalize();		//�I��������

	//���݂̃V�[���^�C�v���擾����
	virtual eSceneType GetNowSceneType()const = 0;

	void DeleteObject(GameObject* obj);

protected:
	//�Q�[���I�u�W�F�N�g�𐶐�
	template <class T>
	T* CreateObject(const Vector2D& _location, const Vector2D& _box_size);
};


