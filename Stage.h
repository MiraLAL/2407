#pragma once

#include "Graphics/Graphics.h"
#include "Collision.h"

// ステージ
class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

    // 更新処理
    virtual void Update(float elapsedTime) = 0;

    // 描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    // レイキャスト
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

};
