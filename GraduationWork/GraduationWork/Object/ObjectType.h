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
	OBJECTTYPE_COUNT // �^�C���̎�ސ��i�Ō�ɒǉ��j

	ENEMY,
	REALENEMY,
};

struct TypeInfo
{
    eObjectType type;
    const char* name;
    int color;
};

// �^�C�����e�[�u���i�O���[�o���ł�OK�AStageEditor�N���X���� static �z��ł�OK�j
static TypeInfo TILE_INFO[OBJECTTYPE_COUNT] = {
    { NONE,          "None",      GetColor(55, 55, 55) },
    { BLOCK,         "Block",     GetColor(100, 200, 100) },
    { PLAYER,        "Player",    GetColor(200, 100, 100) },
    { WALL,          "Wall",      GetColor(150, 150, 150) },
    { LIGHT,         "Light",     GetColor(255, 255, 0) },
    { INVISIBLEFLOOR,"Invisible", GetColor(100, 100, 200) },
    { PUSHBLOCK,     "Push",      GetColor(200, 100, 200) }
};

// �w���p�[�֐�
inline const TypeInfo& GetTypeInfo(eObjectType type)
{
    int idx = static_cast<int>(type);
    if (idx < 0 || idx >= OBJECTTYPE_COUNT)
        return TILE_INFO[0]; // NONE��Ԃ�
    return TILE_INFO[idx];
}
