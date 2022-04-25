#include "App.h"
#include "WStringConvert.h"

#include <sstream>
#include <iomanip>

App::App():
	wnd{800, 600, L"Window Box"}
{

}

int App::Go()
{
	MSG msg{};
	BOOL gResult;

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return ecode.value();
		}
		DoFrame();
	}

	if (gResult == -1)
	{
		throw CHWND_LAST_EXCEPT();
	}
	return static_cast<int>(msg.wParam); // potential loss of data
}

void App::DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().EndFrame();
}
