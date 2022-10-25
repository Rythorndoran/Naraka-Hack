#pragma once
#include <Windows.h>
#include <thread>
#include "Nvidia.h"

namespace mouse {
	inline void right(bool down) {

		INPUT Input = { 0 };

		if (down)
		{
			// RIGHT down
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			SendInput(1, &Input, sizeof(INPUT));
		}
		else {
			// RIGHT up
			ZeroMemory(&Input, sizeof(INPUT));
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			SendInput(1, &Input, sizeof(INPUT));
		}
	}
	inline void left(bool down) {
		INPUT Input = { 0 };

		if (down)
		{
			// left down
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &Input, sizeof(INPUT));
		}
		else {
			// left up
			ZeroMemory(&Input, sizeof(INPUT));
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &Input, sizeof(INPUT));
		}
	}
}
