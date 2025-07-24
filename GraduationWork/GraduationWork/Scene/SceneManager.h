#pragma once
#include "SceneBase.h"

class SceneManager
{
private:
    SceneBase* current_scene;   //���݃V�[�����̃|�C���^

public:
    SceneManager();
    ~SceneManager();


    void Initialize();  //����������
    void Update();      //�X�V����
    void Finalize();    //�I��������
    void Draw();  //�`�揈��

private:
    //�V�[���ؑ֏���
    void ChangeScene(eSceneType type);

    //�V�[����������
    SceneBase* CreateScene(eSceneType type);

};


