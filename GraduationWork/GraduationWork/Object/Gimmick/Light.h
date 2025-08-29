#pragma once
#include "../GameObject.h"
class Light :
    public GameObject
{
private:
	Vector2D apex;
	float width;
	float height;

public:
	//����������
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//�X�V����
	virtual void Update()override;
	//�`�揈��
	virtual void Draw(Vector2D offset, double rate)  override;
	//�I��������
	virtual void Finalize()override;

	virtual void OnHitCollision(GameObject* hit_object)override;

private:
	bool PointInTriangle(const Vector2D& p, const Vector2D& a, const Vector2D& b, const Vector2D& c);

	bool CheckLightCollision(GameObject* obj);
	
};

