#include "Light.h"
#include "DxLib.h"
#include <cmath>

void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);

	pivot = _location;

	length = _box_size.y;
	Draw_length = 500.0f;

	angle = 0.5f;           // �����͍��΂�
	target_angle = 0.5f;    // �����ڕW������
	angle_speed = 0.05f;    // �������ω��i�������قǂ������j

	beam_width = 28.0f;
	hit_box = Vector2D(beam_width, length);
}

void Light::Update()
{
	__super::Update();

	// �p�x���
	float diff = target_angle - angle;
	if (fabs(diff) > 0.001f) {
		angle += diff * angle_speed;
	}

	// --- ���̐�[�����߂� ---
	float halfWidth = beam_width * 0.5f;
	Vector2D widthDir = { cosf(angle), sinf(angle) };
	Vector2D lengthDir = { -sinf(angle), cosf(angle) };

	Vector2D topLeft = pivot - widthDir * halfWidth;
	Vector2D topRight = pivot + widthDir * halfWidth;
	Vector2D bottomLeft = topLeft + lengthDir * Draw_length;
	Vector2D bottomRight = topRight + lengthDir * Draw_length;

	// ��[�̒������W
	Vector2D tipCenter = (bottomLeft + bottomRight) * 0.5f;
}

void Light::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);

	// pivot�̃X�N���[�����W
	float px = pivot.x - offset.x;
	float py = pivot.y - offset.y;

	// �����Ŏl�p�`�̕��E�����E�p�x���w�肵�ĕ`��
	DrawLightColumn({ px, py }, beam_width, Draw_length, angle);

#ifdef _DEBUG
	//DrawBoxAA(390, 290, 410, 310, GetColor(255, 0, 0), FALSE); // �ڈ�̎l�p
#endif
}

void Light::Finalize()
{
	__super::Finalize();
}

void Light::OnHitCollision(GameObject* hit_object)
{
	int type = hit_object->GetObjectType();
	if (type != PLAYER) return; // ���̓v���C���[��������

	Vector2D playerPos = hit_object->GetLocation();
	Vector2D playerSize = hit_object->GetBoxSize();

	// ���̐�[�ʒu���v�Z
	float halfWidth = beam_width * 0.5f;
	Vector2D widthDir = { cosf(angle), sinf(angle) };
	Vector2D lengthDir = { -sinf(angle), cosf(angle) };

	Vector2D topLeft = pivot - widthDir * halfWidth;
	Vector2D topRight = pivot + widthDir * halfWidth;
	Vector2D bottomLeft = topLeft + lengthDir * Draw_length;
	Vector2D bottomRight = topRight + lengthDir * Draw_length;

	// ��[�̒������W
	Vector2D tipCenter = (bottomLeft + bottomRight) * 0.5f;

	// �����蔻��T�C�Y�i�̌��݂��Q�l�Ɂj
	float hitWidth = 50.0f;
	float hitHeight = 20.0f;

	// �v���C���[�Ɛ�[�̋�`��AABB����
	bool hit = !(playerPos.x + playerSize.x < tipCenter.x - hitWidth / 2 ||
		playerPos.x > tipCenter.x + hitWidth / 2 ||
		playerPos.y + playerSize.y < tipCenter.y - hitHeight / 2 ||
		playerPos.y > tipCenter.y + hitHeight / 2);

	if (hit)
	{
		// �����ŉ����N����
	}
}

void Light::DrawLightColumn(Vector2D pivot, float width, float length, float angle_rad)
{
}

void Light::DrawParallelogramFilled(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, unsigned int color)
{
	
}



