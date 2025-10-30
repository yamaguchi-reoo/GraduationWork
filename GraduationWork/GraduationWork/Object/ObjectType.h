#pragma once
#include <DxLib.h>

enum eObjectType
{
	NONE = 0,
	BLOCK,
	PLAYER,
	WALL,
	LIGHT,
	INVISIBLEFLOOR,
	PUSHBLOCK,
    ENEMY,
    REALENEMY,
	PLATE,
    HEAL,
    SHADOWHEAL,
    TEXT,
	OBJECTTYPE_COUNT // �^�C���̎�ސ��i�Ō�ɒǉ��j
};

struct TypeInfo
{
    eObjectType type;
    const char* name;
    int color;
};

// �^�C�����e�[�u���i�O���[�o���ł�OK�AStageEditor�N���X���� static �z��ł�OK�j
static TypeInfo TYPE_INFO[OBJECTTYPE_COUNT] = {
    { NONE,          "None",      static_cast<int>(GetColor(55, 55, 55)) },
    { BLOCK,         "Block",     static_cast<int>(GetColor(100, 200, 100)) },
    { PLAYER,        "Player",    static_cast<int>(GetColor(200, 100, 100)) },
    { WALL,          "Wall",      static_cast<int>(GetColor(150, 150, 150)) },
    { LIGHT,         "Light",     static_cast<int>(GetColor(255, 255, 0)) },
    { INVISIBLEFLOOR,"Invisible", static_cast<int>(GetColor(100, 100, 200)) },
    { PUSHBLOCK,     "Push",      static_cast<int>(GetColor(200, 100, 200)) },
    { ENEMY,         "Enemy",     static_cast<int>(GetColor(100, 100, 200)) },
    { REALENEMY,     "REnemy",    static_cast<int>(GetColor(50, 100, 200)) },
    { PLATE,         "Plate",     static_cast<int>(GetColor(200, 0, 200)) },
    { HEAL,          "Heal",      static_cast<int>(GetColor(0, 255, 0)) },
    { SHADOWHEAL,    "ShadowHeal",static_cast<int>(GetColor(0, 255, 100)) }
    { TEXT,          "Text",      static_cast<int>(GetColor(200, 200, 255)) },
};

// �w���p�[�֐�
inline const TypeInfo& GetTypeInfo(eObjectType type)
{
    int idx = static_cast<int>(type);
    if (idx < 0 || idx >= OBJECTTYPE_COUNT)
        return TYPE_INFO[0]; // NONE��Ԃ�
    return TYPE_INFO[idx];
}
