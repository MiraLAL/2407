#include "StageMoveFloor.h"

// �R���X�g���N�^
StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;

    // �X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
    // �X�e�[�W���f����j��
    delete model;
}

void StageMoveFloor::Update(float elapsedTime)
{
    // �O��̏���ۑ�
    oldTransform = transform;
    oldAngle = angle;

    // �X�^�[�g����S�[���܂ł̋������Z�o����
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

    float length;
    DirectX::XMStoreFloat(&length, Length);

    // �X�^�[�g����S�[���܂ł̊Ԃ̈�b�ԂŐi�ޏꍇ(0.0~1.0)���Z�o����
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;

    // �S�[���ɓ��B�A�܂��̓X�^�[�g�ɖ߂����ꍇ�A�ړ������𔽓]������
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    // ���`�⊮�ňʒu���Z�o����
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    // ��]
    angle.x += torque.x * elapsedTime;
    angle.y += torque.y * elapsedTime;
    angle.z += torque.z * elapsedTime;

    // �s��X�V
    UpdateTransform();

    // ���f���s��X�V
    //model->UpdateTransform(transform);

    // ���C�L���X�g�p�Ƀ��f����ԍs��ɂ��邽�ߒP�ʍs���n��
    const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
    model->UpdateTransform(transformIdentity);
}

// �`�揈��
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // �\���p�̂��߃��[���h�s��ɍX�V����
    model->UpdateTransform(transform);
    shader->Draw(dc, model);
}

// ���C�L���X�g
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    // �O��̃��[���h�s��Ƌt�s������߂�
    DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&oldTransform);
    
    // �t�s��
    DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

    // �O��̃��[�J����Ԃł̃��C�ɕϊ�
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    
    // ���[�J����Ԃł̍��W�ɕϊ�����
    DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
    DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);

    // ���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, LocalStart);
    DirectX::XMStoreFloat3(&localEnd, LocalEnd);


    HitResult localHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
    {
        // �O��̃��[�J����Ԃ��獡��̃��[���h��Ԃ֕ϊ�
        // �O�񂩂獡��ɂ����ĕύX���ꂽ���e������Ă���I�u�W�F�N�g�ɔ��f�����B
        //�@���[���h��ԍs����擾
        WorldTransform = DirectX::XMLoadFloat4x4(&transform);
        // ��_��VECTOR�^�ɕϊ�
        DirectX::XMVECTOR LocalPosition = DirectX::XMLoadFloat3(&localHit.position);
        // ���[�J�����W�̌�_�����[���h��ԍ��W�ɕϊ�
        DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(LocalPosition, WorldTransform);

        // �@����VECTOR�^�ɕϊ�
        DirectX::XMVECTOR LocalNormal = DirectX::XMLoadFloat3(&localHit.normal);
        // ���[�J����ԍ��W�̖@�������[���h��ԍ��W�ɕϊ�
        DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(LocalNormal, WorldTransform);

        // ��_�܂ł̋������擾
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
        DirectX::XMVECTOR Distance = DirectX::XMVector3Length(Vec);
        
        // ���[���h�ϊ��s��ɕϊ�����HitResult��p��
        DirectX::XMStoreFloat3(&hit.position, WorldPosition);
        DirectX::XMStoreFloat3(&hit.normal, WorldNormal);
        DirectX::XMStoreFloat(&hit.distance, Distance);
        hit.materialIndex = localHit.materialIndex;

        // ��]�������Z�o
        hit.rotation.x = angle.x - oldAngle.x;
        hit.rotation.y = angle.y - oldAngle.y;
        hit.rotation.z = angle.z - oldAngle.z;

        return true;
    }
    return false;
}

// �s��X�V����
void StageMoveFloor::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}
