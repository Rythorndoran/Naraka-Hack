#include "includes.h"
#include "Hack.h"


void _renderer(Nvidia* overlay)
{

	// Initialize the window
	if (!overlay->window_init())
		return;

	// D2D Failed to initialize?
	if (!overlay->init_d2d())
		return;

	while (true)
	{
		overlay->begin_scene();
		overlay->clear_scene();


		draw_menu(overlay);

		draw_esp(overlay);

		update();

		entity_loop();

		overlay->end_scene();
		Sleep(10);
	}
	overlay->d2d_shutdown();
	return;
}

int _init()
{

	std::thread renderer_thread
	(
		[]()
		{
			Nvidia overlay;
			_renderer(&overlay);
		}
	);

	renderer_thread.detach();

	while (true)
	{
		Sleep(10000);
		auto NarakaWindow = FindWindowA("UnityWndClass", "Naraka");
		if (!NarakaWindow)
		{
			ExitProcess(0);
		}
	}

}
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	InitializeAddress();

	return _init();
}