#include "SceneBase.h"
#include "../common.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Initialize()
{
}

eSceneType SceneBase::Update()
{
	return GetNowSceneType();
}

void SceneBase::Draw()
{
}

void SceneBase::Finalize()
{
}

