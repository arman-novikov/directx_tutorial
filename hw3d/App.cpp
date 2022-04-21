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
	return msg.wParam;
}

void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time Elapsed: " << std::setprecision(1) << std::fixed << t;
	wnd.SetTitle(wstring_convert::to_wstring(oss.str()));
}
