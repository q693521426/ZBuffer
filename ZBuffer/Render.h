#pragma once
#include "Common.h"
#include "Camera.h"
#include <vector>
#include <set>

struct Polygon_t
{ 
	XMFLOAT4 pos_world[3];
	XMFLOAT3 norm_world;
	XMFLOAT4 pos[3];
	XMFLOAT2 tex[3];
	XMFLOAT3 bary_dx;
	XMFLOAT3 bary_dy;
	float z_max;
	float z_min;
	bool flag;
};

struct Edge_t
{
	float x;	// start x
	float y;	// max y
	float dx;	// slope
	float dy;	// max y range 
	float dt;	// for bary-coord
	int index[2];
	Polygon_t* pPoly;
	int polyId;
};

struct ActiveEdgePair_t
{
	Edge_t* el, er;
	float xl;
	float dxl;
	float dyl;
	float xr;
	float dxr;
	float dyr;

	float zl;
	float dzx;
	float dzy;
};


class Render
{
public:
	Render();
	~Render();

	HRESULT Init(HINSTANCE hInstance, int nCmdShow, UINT screenWidth = 800, UINT screenHeight = 600);
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	int RunLoop();
	void Draw();
	void Release();

public:
	int screenWidth;
	int screenHeight;

	bool bFullScreen;
	
	LightInfo m_Light;
	Camera m_Camera;
	XMFLOAT3 m_f3ViewPos;

	std::vector<Triangle3D> m_TriangleWorld;
	std::vector<Polygon_t> m_Polygons;
	std::set<int> m_ActivePolygonIds;
	std::vector<Edge_t> m_Edges;
	std::vector<Edge_t> m_ActiveEdges;
	std::vector<ActiveEdgePair_t> m_ActiveEdgePairs;

	std::vector<float> m_zBuffer;
	int* m_pColorBuffer;
private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

	XMFLOAT3 transBaryCentric(const XMFLOAT4& p, const XMFLOAT4& p0, const XMFLOAT4& p1, const XMFLOAT4& p2);
	XMFLOAT4 transPerspectiveCorrect(const XMFLOAT3& p_bary, const FLOAT z0, const FLOAT z1, const FLOAT z2);

	std::wstring m_hWndCaption;
	HANDLE* m_hThread;
	HWND m_hWnd;
	HINSTANCE m_hInst;

	// double buffer
	HDC m_hdc, m_hdcMem;
	HBITMAP m_hBitMap;

	PAINTSTRUCT m_paint;
};
static Render* g_ApplicationHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);