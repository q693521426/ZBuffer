#include "Render.h"
#include <sstream>
#include <directxcolors.h>
#include <iostream>
#include <cassert>
#include <algorithm>

Vertex SquareVerts[]
{
	{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

	{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
};

int SquareIndices[]
{
	3,1,0,
	2,1,3,

	6,4,5,
	7,4,6,

	11,9,8,
	10,9,11,

	14,12,13,
	15,12,14,

	19,17,16,
	18,17,19,

	22,20,21,
	23,20,22
};

Render::Render()
{
}

Render::~Render()
{
}

HRESULT Render::Init(HINSTANCE hInstance, int nCmdShow, UINT screenWidth /* = 800 */, UINT screenHeight /* = 600 */)
{
	HRESULT hr = S_OK;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	m_f3ViewPos = XMFLOAT3(0.0f, 3.0f, -3.0f);
	XMVECTOR Eye = XMVectorSet(m_f3ViewPos.x, m_f3ViewPos.y, m_f3ViewPos.z, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, -0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_Camera.LookAt(Eye, At, Up);
	m_Camera.UpdateViewMatrix();

	m_Light.Pos = XMFLOAT3(0.0f, 1.0f, -1.5f);
	m_Light.Color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Light.View = LookAt(m_Light.Pos, XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.f, 1.0f, 0.f));
	m_Light.Project = XMMatrixPerspectiveFovLH(Pi * 2 / 3, screenWidth / (FLOAT)screenHeight, 1.0f, 100.0f);
	m_Light.Ambient = XMFLOAT3(0.05f, 0.05f, 0.05f);
	m_Light.Diffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Light.Specular = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Light.Constant = 1.0f;
	m_Light.Linear = 0.009;
	m_Light.Quadratic = 0.0032;

	// Square Test
	{
		int indexNum = ARRAYSIZE(SquareIndices);
		int triangleNum = indexNum / 3;
		for (int i = 0; i < triangleNum; i++)
		{
			Triangle3D t;
			for (int j = 0; j < 3; j++)
			{
				int index = SquareIndices[i * 3 + j];
				t.pos[j] = SquareVerts[index].pos;
				t.tex[j] = SquareVerts[index].tex;
			}
			XMVECTOR v0 = XMLoadFloat4(&t.pos[0]);
			XMVECTOR v1 = XMLoadFloat4(&t.pos[1]);
			XMVECTOR v2 = XMLoadFloat4(&t.pos[2]);

			XMVECTOR e0 = XMVectorSubtract(v1, v0);
			XMVECTOR e1 = XMVectorSubtract(v2, v0);

			XMStoreFloat3(&t.norm, XMVector3Normalize(XMVector3Cross(e0, e1)));

			m_TriangleWorld.push_back(t);
		}
	}

	hr = InitWindow(hInstance, nCmdShow);
	V_RETURN(hr);

	return hr;
}

HRESULT Render::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	g_ApplicationHandle = this;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"ZBuffer";
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_WINLOGO);
	if (!RegisterClassEx(&wc))
		return E_FAIL;

	// Create window
	m_hInst = hInstance;
	m_hWndCaption = L"ZBuffer";

	std::wostringstream outs;
	outs.precision(6);
	outs << m_hWndCaption << L"    "
		<< L"FPS: " << L"    " << L"    "
		<< L"Frame Time: " << L"    " << L" (ms)";

	if (bFullScreen)
	{
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		posX = posY = 0;
	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	m_hWnd = CreateWindow(m_hWndCaption.c_str(), outs.str().c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd)
	{
		DWORD i = GetLastError();
		std::cout << i << std::endl;
		return E_FAIL;
	}

	ShowWindow(m_hWnd, nCmdShow);

	// double buffer
	// 根据当前DC创建兼容的内存DC
	m_hdc = GetDC(m_hWnd);
	m_hdcMem = CreateCompatibleDC(m_hdc);

	BITMAPINFO bmp_info;
	bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//结构体的字节数
	bmp_info.bmiHeader.biWidth = screenWidth;//以像素为单位的位图宽
	bmp_info.bmiHeader.biHeight = -(int)screenHeight;//以像素为单位的位图高,若为负，表示以左上角为原点，否则以左下角为原点
	bmp_info.bmiHeader.biPlanes = 1;//目标设备的平面数，必须设置为1
	bmp_info.bmiHeader.biBitCount = 32; //位图中每个像素的位数
	bmp_info.bmiHeader.biCompression = BI_RGB;
	bmp_info.bmiHeader.biSizeImage = 0;
	bmp_info.bmiHeader.biXPelsPerMeter = 0;
	bmp_info.bmiHeader.biYPelsPerMeter = 0;
	bmp_info.bmiHeader.biClrUsed = 0;
	bmp_info.bmiHeader.biClrImportant = 0;

	// 创建内存DC对应位图
	m_hBitMap = CreateDIBSection(m_hdcMem, &bmp_info, DIB_RGB_COLORS, (void**)(&m_pColorBuffer), NULL, 0);

	return S_OK;
}

int Render::RunLoop()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//timer.Tick();
		//CalculateFrameStats();
		//Update(timer);
		Draw();
	}
	return (int)msg.wParam;	
}

void Render::Draw()
{
	// Clear Buffer
	m_zBuffer.clear();
	m_zBuffer.resize(screenWidth * screenHeight);
	int color = (int)(Colors::MidnightBlue[0] * 255) << 16
		| (int)(Colors::MidnightBlue[1] * 255) << 8
		| (int)(Colors::MidnightBlue[2] * 255);
	for (int i = 0; i < screenWidth * screenHeight; i++)
	{
		m_pColorBuffer[i] = color;
		m_zBuffer[i] = 1.f;
	}

	XMMATRIX view = m_Camera.GetView();
	XMMATRIX proj = m_Camera.GetProj();

	for (int i = 0; i < m_TriangleWorld.size(); i++)
	{
		bool bIsBackCulling = false;
		Triangle3D tri_world = m_TriangleWorld[i];
		Triangle3D tri_proj;

		XMVECTOR norm; 
		norm = XMVector4Transform(XMVectorSet(tri_world.norm.x, tri_world.norm.y, tri_world.norm.z, 0), view);

		float z_min = 1.f, z_max = 0.f;
		for (int j = 0; j < 3; j++)
		{
			XMVECTOR v;
			v = XMVector4Transform(XMVectorSet(tri_world.pos[j].x, tri_world.pos[j].y, tri_world.pos[j].z, tri_world.pos[j].w), view);
			bIsBackCulling = XMVectorGetX(XMVector3Dot(v, norm)) > 0;
			if (bIsBackCulling)
				break;

			v = XMVector4Transform(v, proj);

			tri_proj.pos[j] = XMFLOAT4(XMVectorGetX(v) / XMVectorGetW(v), XMVectorGetY(v) / XMVectorGetW(v), XMVectorGetZ(v) / XMVectorGetW(v), XMVectorGetW(v));
			tri_proj.pos[j].x = (tri_proj.pos[j].x + 1) * screenWidth / 2;
			tri_proj.pos[j].y = (tri_proj.pos[j].y + 1) * screenHeight / 2;
			tri_proj.tex[j] = tri_proj.tex[j];

			if (z_min > tri_proj.pos[j].z)
				z_min = tri_proj.pos[j].z;
			if (z_max < tri_proj.pos[j].z)
				z_max = tri_proj.pos[j].z;
		}
		if (bIsBackCulling)
			continue;

		Polygon_t poly;
		poly.norm_world = tri_world.norm;
		memcpy(poly.pos_world, &tri_world.pos, sizeof(XMFLOAT4) * 3);
		memcpy(poly.pos, &tri_proj.pos, sizeof(XMFLOAT4) * 3);
		memcpy(poly.tex, &tri_proj.pos, sizeof(XMFLOAT2) * 3);
		XMFLOAT4 p_dx = poly.pos[0]; p_dx.x += 1;
		XMFLOAT3 p_bary_dx = transBaryCentric(p_dx, poly.pos[0], poly.pos[1], poly.pos[2]);
		p_bary_dx.x -= 1; // pos[0] bary-coord (1,0,0)
		XMFLOAT4 p_dy = poly.pos[0]; p_dx.y += 1;
		XMFLOAT3 p_bary_dy = transBaryCentric(p_dy, poly.pos[0], poly.pos[1], poly.pos[2]);
		p_bary_dy.x -= 1; // pos[0] bary-coord (1,0,0)
		poly.bary_dx = p_bary_dx;
		poly.bary_dy = p_bary_dy;
		poly.flag = false;
		m_Polygons.push_back(poly);

		float y_min = FLT_MAX, y_max = FLT_MIN;
		for (int j = 0; j < 3; j++)
		{
			int index0 = j, index1 = (j + 1) % 3;
			XMFLOAT4 p0 = tri_proj.pos[index0];
			XMFLOAT4 p1 = tri_proj.pos[index1];
			
			if (p0.y == p1.y)
				continue;

			if (p0.y < p1.y)
			{
				std::swap(index0, index1);
			}

			p0 = tri_proj.pos[index0];
			p1 = tri_proj.pos[index1];

			Edge_t e;
			e.x = p0.x;
			e.y = p0.y;
			e.dy = p0.y - p1.y;
			e.dx = (p0.x - p1.x) / e.dy; 
			e.dt = 1 / e.dy;
			e.index[0] = index0;
			e.index[1] = index1;
			e.polyId = m_Polygons.size() - 1;
			e.pPoly = &m_Polygons[e.polyId];

			m_Edges.push_back(e);
		}
	}

	for (int y = screenHeight - 1; y >= 0; y--)
	{
		m_ActiveEdges.clear();
		for (auto e = m_Edges.begin(); e != m_Edges.end(); e++)
		{
			if (y > e->y || y < (e->y - e->dy))
				continue;

			e->pPoly->flag = false;

			Edge_t e_new = *e;
			float dy = e->y - y;
			e_new.dy -= dy;
			e_new.x -= e_new.dx * dy;

			// sort insert
			auto it = m_ActiveEdges.begin();
			while (it != m_ActiveEdges.end())
			{
				if (it->x > e_new.x)
				{
					it = m_ActiveEdges.insert(it, e_new);
					break;
				}
				else
					it++;
			}
			if (it == m_ActiveEdges.end())
			{
				m_ActiveEdges.push_back(e_new);
			}
		}

		m_ActivePolygonIds.clear();
		for (int i = 0; i < m_ActiveEdges.size() - 1; i++)
		{
			Edge_t el = m_ActiveEdges[i];
			Edge_t er = m_ActiveEdges[i + 1];
			
			if (el.pPoly->flag)
			{
				el.pPoly->flag = false;
				auto it = m_ActivePolygonIds.find(el.polyId);
				assert(it != m_ActivePolygonIds.end());
				m_ActivePolygonIds.erase(it);
			}
			else
			{
				el.pPoly->flag = true;
				auto it = m_ActivePolygonIds.find(el.polyId);
				assert(it == m_ActivePolygonIds.end());
				m_ActivePolygonIds.insert(el.polyId);
			}

			if (el.x < 0 && er.x < 0)
				continue;
			if (el.x > screenWidth - 1 && er.x > screenWidth - 1)
				continue;

			assert(m_ActivePolygonIds.size() >= 1);

			int xl = el.x < 0 ? 0 : round(el.x);
			int xr = er.x > screenWidth - 1 ? screenWidth - 1 : round(er.x);
			for (int x = xl; x < xr; x++)
			{
				int drawId = -1;
				XMFLOAT4 correct_bary;
				float depth;
				int pixelIndex = y * screenWidth + x;
				for (auto it = m_ActivePolygonIds.begin(); it != m_ActivePolygonIds.end(); it++)
				{
					int id = *it;
					Polygon_t& poly = m_Polygons[id];
					float dx = x - poly.pos[0].x; 
					float dy = y - poly.pos[0].y;
					XMFLOAT3 bary;
					bary.x = dx * poly.bary_dx.x + dy * poly.bary_dy.x + 1;
					bary.y = dx * poly.bary_dx.y + dy * poly.bary_dy.y;
					bary.z = dx * poly.bary_dx.z + dy * poly.bary_dy.z;
					XMFLOAT4 correct = transPerspectiveCorrect(bary, poly.pos[0].w, poly.pos[1].w, poly.pos[2].w);
					if (correct.w < m_Camera.GetNearZ() || correct.w > m_Camera.GetFarZ())
						continue;
					float z = (correct.w - m_Camera.GetNearZ()) / (m_Camera.GetFarZ() - m_Camera.GetNearZ());

					if (z < m_zBuffer[pixelIndex])
					{
						drawId = id;
						correct_bary = correct;
						depth = z;
					}
				}
				if (-1 == drawId)
					continue;

				// draw 
				Polygon_t& poly = m_Polygons[drawId];
				XMFLOAT3 v_world;
				v_world.x = correct_bary.x * poly.pos_world[0].x + correct_bary.y * poly.pos_world[1].x + correct_bary.z * poly.pos_world[2].x;
				v_world.y = correct_bary.x * poly.pos_world[0].y + correct_bary.y * poly.pos_world[1].y + correct_bary.z * poly.pos_world[2].y;
				v_world.z = correct_bary.x * poly.pos_world[0].z + correct_bary.y * poly.pos_world[1].z + correct_bary.z * poly.pos_world[2].z;

				//Light
				XMVECTOR world_pos = XMLoadFloat3(&v_world);
				XMVECTOR light_pos = XMLoadFloat3(&m_Light.Pos);

				XMVECTOR lightDir = XMVector3Normalize(XMVectorSubtract(light_pos, world_pos));
				XMVECTOR viewDir = XMVector3Normalize(XMVectorSubtract(m_Camera.GetPosition(), world_pos));
				XMVECTOR halfwayDir = XMVector3Normalize(lightDir + viewDir);					

				//attenuation
				XMFLOAT3 distance;
				XMStoreFloat3(&distance, XMVector3Length(XMVectorSubtract(light_pos, world_pos)));
				float attenuation = 1.0f / (m_Light.Constant + m_Light.Linear * distance.x + m_Light.Quadratic * (distance.x * distance.x));

				//ambient

				//diffuse
				float diff = max(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&poly.norm_world), lightDir)), 0.0);

				//specular
				int shininess = 20;
				float dot = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&poly.norm_world), halfwayDir));
				float spec = pow_int(dot, shininess);

				//float R = TexColor.x;
				//float G = TexColor.y;
				//float B = TexColor.z;
				float R = 1.f;
				float G = 1.f;
				float B = 1.f;

				R *= (m_Light.Ambient.x + diff* m_Light.Diffuse.x + spec* m_Light.Specular.x)*m_Light.Color.x*attenuation*255.0f;
				G *= (m_Light.Ambient.y + diff* m_Light.Diffuse.y + spec* m_Light.Specular.y)*m_Light.Color.y*attenuation*255.0f;
				B *= (m_Light.Ambient.z + diff* m_Light.Diffuse.z + spec* m_Light.Specular.z)*m_Light.Color.z*attenuation*255.0f;
			
				R = min(R, 255.0f);
				G = min(G, 255.0f);
				B = min(B, 255.0f);

				m_pColorBuffer[pixelIndex] = (int)R << 16 | (int)G << 8 | (int)B;
			}
			


		}
	}

}

void Render::Release()
{
	//SelectObject(m_hdcMem, m_hBitMap);
	DeleteDC(m_hdcMem);
	ReleaseDC(m_hWnd, m_hdc);
}

LRESULT CALLBACK Render::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_ACTIVATE:
		return 0;
	case WM_PAINT:
	{
		HBITMAP hbmOld = (HBITMAP)::SelectObject(m_hdcMem, m_hBitMap);
		BitBlt(m_hdc, 0, 0, screenWidth, screenHeight, m_hdcMem, 0, 0, SRCCOPY);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// All other messages pass to the message handler in the system class.
	default:
	{
		return g_ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

XMFLOAT3 Render::transBaryCentric(const XMFLOAT4& p, const XMFLOAT4& p0, const XMFLOAT4& p1, const XMFLOAT4& p2)
{
	FLOAT det = (p1.y - p2.y)*(p0.x - p2.x) + (p2.x - p1.x)*(p0.y - p2.y);
	FLOAT a = ((p1.y - p2.y)*(p.x - p2.x) + (p2.x - p1.x)*(p.y - p2.y)) / det;
	FLOAT b = ((p2.y - p0.y)*(p.x - p2.x) + (p0.x - p2.x)*(p.y - p2.y)) / det;

	return XMFLOAT3(a, b, 1.0f - a - b);
}

XMFLOAT4 Render::transPerspectiveCorrect(const XMFLOAT3& p_bary, const FLOAT z0, const FLOAT z1, const FLOAT z2)
{
	FLOAT a = p_bary.x / z0;
	FLOAT b = p_bary.y / z1;
	FLOAT c = p_bary.z / z2;
	FLOAT d = a + b + c;

	FLOAT x = a / d;
	FLOAT y = b / d;
	FLOAT z = 1 - x - y;

	return XMFLOAT4(x, y, z, 1 / d);
}