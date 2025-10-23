#include "Prerequisites.h"
#include "BaseApp.h"

int WINAPI 
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
LPWSTR lpCmdLine, int nCmdShow) {
	BaseApp app(hInstance, nCmdShow);
	return app.run(hInstance, nCmdShow);
}