#include "Render.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Render* render = new Render();
	HRESULT hr;

	if (!render)
	{
		return 0;
	}

	hr = render->Init(hInstance, nCmdShow, 800, 600);
	if (FAILED(hr))
		return 0;

	render->RunLoop();

	render->Release();
	delete render;
	render = nullptr;

	return 0;
}