#include "Collision.h"


// ÆÌð·»è
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	// BšAÌPÊxNgðZo
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// £»è
	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false;
	}

	// AªBðµo·
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);
	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true;
}

// ÆÌð·»è(Ex)
bool Collision::IntersectSphereVsSphereEx(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB,
	DirectX::XMFLOAT3& outPositionA)
{
	// BšAÌPÊxNgðZo
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// £»è
	float range = radiusA + radiusB;
	float sublength = range - lengthSq;

	if (sublength < 0.0f)
	{
		return false;
	}

	// AÆBÌÔ_
	DirectX::XMVECTOR nVec = DirectX::XMVector3Normalize(Vec);
	PositionB = DirectX::XMVectorAdd(PositionB, DirectX::XMVectorScale(nVec, 0.5f * sublength));
	PositionA = DirectX::XMVectorAdd(PositionA, DirectX::XMVectorScale(nVec, -0.5f * sublength));

	// AÆBªÝ¢ðµ€
	DirectX::XMStoreFloat3(&outPositionB, PositionB);
	DirectX::XMStoreFloat3(&outPositionA, PositionA);

	return true;
}

// ~Æ~Ìð·»è
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	// AÌ«³ªBÌªæèãÈçœÁÄ¢È¢
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	// AÌªªBÌ«³æèºÈçœÁÄ¢È¢
	if (positionA.y + heightA < positionB.y)
	{
		return false;
	}

	// XZœÊÅÌÍÍ`FbN
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float range = radiusA + radiusB;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range)
	{
		return false;
	}

	// AªBðµo·
	vx /= distXZ;
	vz /= distXZ;
	outPositionB.x = positionA.x + (vx * range);
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + (vz * range);

	return true;
}

// ~Æ~Ìð·»è(Ex)
bool Collision::IntersectCylinderVsCylinderEx(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB,
	DirectX::XMFLOAT3& outPositionA)
{
	// AÌ«³ªBÌªæèãÈçœÁÄ¢È¢
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}

	// AÌªªBÌ«³æèºÈçœÁÄ¢È¢
	if (positionA.y + heightA < positionB.y)
	{
		return false;
	}

	// XZœÊÅÌÍÍ`FbN
	// BšAÌPÊxNgðZo
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// £»è
	float range = radiusA + radiusB;
	float sublength = range - lengthSq;

	if (sublength < 0.0f)
	{
		return false;
	}

	// AÆBÌÔ_
	DirectX::XMVECTOR nVec = DirectX::XMVector3Normalize(Vec);
	PositionB = DirectX::XMVectorAdd(PositionB, DirectX::XMVectorScale(nVec, 0.5f * sublength));
	PositionA = DirectX::XMVectorAdd(PositionA, DirectX::XMVectorScale(nVec, -0.5f * sublength));

	// AÆBªÝ¢ðµ€
	DirectX::XMStoreFloat3(&outPositionB, PositionB);
	DirectX::XMStoreFloat3(&outPositionA, PositionA);

	return true;
}

// Æ~Ìð·»è
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	// ³`FbN
	if (spherePosition.y + sphereRadius < cylinderPosition.y) return false;
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight) return false;

	// XZœÊÅÌÍÍ`FbN
	float vx = cylinderPosition.x - spherePosition.x;
	float vz = cylinderPosition.z - spherePosition.z;
	float range = sphereRadius + cylinderRadius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range) return false;

	// ª~ðµo·
	vx /= distXZ;
	vz /= distXZ;
	outCylinderPosition.x = spherePosition.x + (vx * range);
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z + (vz * range);

	return true;
}

// CÆfÌð·»è
bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3 & start,
	const DirectX::XMFLOAT3 & end,
	const Model * model,
	HitResult & result)
{
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	// [hóÔÌCÌ·³
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// bVm[hæŸ
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		// Cð[hóÔ©ç[JóÔÖÏ·
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
		DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

		// CÌ·³
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		// Op`(Ô)ÆÌð·»è
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;

		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition;
		DirectX::XMVECTOR HitNormal;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndex + i;

				// Op`Ìž_ðo
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				// Op`ÌOÓxNgðZo
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				// Op`Ì@üxNgðZo
				DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);

				// à€ÌÊªvXÈçÎ\ü«
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, Normal);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot >= 0) continue;

				// CÌœÊÌð_ðZo
				DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);
				DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(Normal, V), Dot);
				float t;
				DirectX::XMStoreFloat(&t, T);
				if (t < 0.0f || t>neart) continue;  // ð_ÜÅÌ£ª¡ÜÅÉvZµœÅß£æè
												    // å«¢Æ«ÍXLbv
				DirectX::XMVECTOR Position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(Dir, T), Start);

				// ð_ªOp`Ìà€É é©»è
				// 1Âß
				DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(A, Position);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(V1, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, Normal);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f) continue;

				// 2Âß
				DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(B, Position);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(V2, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, Normal);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f) continue;

				// 3Âß
				DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(C, Position);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(V3, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, Normal);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f) continue;

				// Åß£ðXV
				neart = t;

				// ð_Æ@üðXV
				HitPosition = Position;
				HitNormal = Normal;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex >= 0)
		{
			// [JóÔ©ç[hóÔÖÏ·
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			// qbgîñÛ¶
			if (result.distance > distance)
			{
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}
	}

	return hit;
}