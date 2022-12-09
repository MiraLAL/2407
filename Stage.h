#pragma once

#include "Graphics/Graphics.h"
#include "Collision.h"

// �X�e�[�W
class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

    // �X�V����
    virtual void Update(float elapsedTime) = 0;

    // �`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    // ���C�L���X�g
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

};