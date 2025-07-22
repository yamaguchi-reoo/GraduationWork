#include "ObjectManager.h"
#include <algorithm>
#include <DxLib.h>

#include "ObjectInfo.h"
#include "Character/Player/Player.h"
#include "Stage/Block.h"

#include <Windows.h>


ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager()
{
    Finalize(); 
}

void ObjectManager::Initialize()
{
	CreateObject<Player>(Vector2D(640.0f, 300.0f), Vector2D(50.0f, 50.0f)); // �v���C���[�I�u�W�F�N�g�𐶐�
	CreateObject<Block>(Vector2D(640.0f, 360.0f), Vector2D(BLOCK_SIZE)); // �v���C���[�I�u�W�F�N�g�𐶐�
}

void ObjectManager::Update()
{
    for (auto obj : objects)
    {
        if (obj) obj->Update();
    }

	GameObject* player = FindObjectType(eObjectType::PLAYER);

    if (player)
    {
        for (auto obj : objects)
        {
            if (obj && obj != player)
            {
                if (player->CheckBoxCollision(obj))
                {
                    player->OnHitCollision(obj);
                    obj->OnHitCollision(player);
                }
            }
        }
    }
}


void ObjectManager::Draw(Vector2D offset, double rate)
{
    for (auto obj : objects)
    {
        if (obj) obj->Draw(obj->GetLocation() - offset, rate);
    }
}

void ObjectManager::Finalize()
{
    //�I�u�W�F�N�g����̏ꍇ�͏������X�L�b�v
    if (objects.empty())
    {
        return;
    }
    for (auto obj : objects)
    {
        if (obj)
        {
            obj->Finalize();
            delete obj;
        }
    }
    objects.clear();
}

void ObjectManager::DeleteObject(GameObject* obj)
{
    if (obj == nullptr) return;

    auto it = std::find(objects.begin(), objects.end(), obj);

    if (it != objects.end()) {
        //���������ꍇ�A�I�u�W�F�N�g���폜
        (*it)->Finalize(); //�I������
        delete* it;         //���������
        objects.erase(it);  //���X�g����폜
    }
}

GameObject* ObjectManager::FindObjectType(eObjectType type)
{
    for (auto obj : objects)
    {
        if (obj && obj->GetObjectType() == type)
        {
            return obj;
        }
    }
    return nullptr;
}


