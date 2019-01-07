#pragma once

#include <DirectXMath.h>
#include <windows.h>

using namespace DirectX;

#define V_RETURN(hr) if(FAILED(hr)) return false;

struct Vertex
{
	XMFLOAT4 pos;
	XMFLOAT2 tex;
	XMFLOAT3 norm;
};

struct VS_OUT
{

};

struct Triangle3D 
{
	XMFLOAT4 pos[3];
	XMFLOAT2 tex[3];
	XMFLOAT3 norm;
};

struct LightInfo
{
	XMFLOAT3 Pos;
	XMFLOAT3 Color;
	XMMATRIX View;
	XMMATRIX Project;
	XMFLOAT3 Ambient;
	XMFLOAT3 Diffuse;
	XMFLOAT3 Specular;
	FLOAT Constant;
	FLOAT Linear;
	FLOAT Quadratic;
};

const float Pi = 3.1415926535f;

static DirectX::XMMATRIX LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	using namespace DirectX;
	XMFLOAT4X4 View;
	XMFLOAT3 Right, Up, Look;

	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(T, P));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(U, L));
	U = XMVector3Cross(L, R);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&Right, R);
	XMStoreFloat3(&Up, U);
	XMStoreFloat3(&Look, L);

	View(0, 0) = Right.x;
	View(1, 0) = Right.y;
	View(2, 0) = Right.z;
	View(3, 0) = x;

	View(0, 1) = Up.x;
	View(1, 1) = Up.y;
	View(2, 1) = Up.z;
	View(3, 1) = y;

	View(0, 2) = Look.x;
	View(1, 2) = Look.y;
	View(2, 2) = Look.z;
	View(3, 2) = z;

	View(0, 3) = 0.0f;
	View(1, 3) = 0.0f;
	View(2, 3) = 0.0f;
	View(3, 3) = 1.0f;

	return XMLoadFloat4x4(&View);
}

static DirectX::XMFLOAT4X4 Identity4x4()
{
	static DirectX::XMFLOAT4X4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}

static float pow_int(const float a, const int b)
{
	if (b == 1)
	{
		return a;
	}
	if (b == 0)
	{
		return 1;
	}
	float x = a * a;
	if (b % 2)
	{
		return a * pow_int(x, b / 2);
	}
	else
	{
		return pow_int(x, b / 2);
	}
}