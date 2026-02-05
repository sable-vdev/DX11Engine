#include "Frustum.hpp"

void Frustum::CreateFrustumFromMatrix(const Mat4x4& viewProj)
{
	DX::XMFLOAT4X4 mat;
	DX::XMStoreFloat4x4(&mat, viewProj);
	
	//Near
	planes[0].normal = DX::XMVectorSet(mat._13, mat._23, mat._33, mat._43);

	//Far
	planes[1].normal = DX::XMVectorSet(mat._14 - mat._13, mat._24 - mat._23, mat._34 - mat._33, mat._44 - mat._43);

	//Left
	planes[2].normal = DX::XMVectorSet(mat._14 + mat._11, mat._24 + mat._21, mat._34 + mat._31, mat._44 + mat._41);

	//right
	planes[3].normal = DX::XMVectorSet(mat._14 - mat._11, mat._24 - mat._21, mat._34 - mat._31, mat._44 - mat._41);

	//Top
	planes[4].normal = DX::XMVectorSet(mat._14 - mat._12, mat._24 - mat._22, mat._34 - mat._32, mat._44 - mat._42);

	//bottom
	planes[5].normal = DX::XMVectorSet(mat._14 + mat._12, mat._24 + mat._22, mat._34 + mat._32, mat._44 + mat._42);

	for (int i{}; i < 6; i++)
	{
		float len = DX::XMVectorGetX(DX::XMVector3Length(planes[i].normal));
		planes[i].normal = DX::XMVectorScale(planes[i].normal, 1.0f / len);
	}

}