#include "Mouse.h"
#include <Windows.h>

Mouse::Mouse():
	x{0},
	y{0},
	leftIsPressed{false},
	rightIsPressed{false},
	isInWindow{false},
	wheelDeltaCarry{0},
	buffer{}
{
}

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x, y };
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		auto e = buffer.front();
		buffer.pop();
		return e;
	}
	return {};
}

void Mouse::Flush() noexcept
{
	buffer = decltype(buffer){};
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;
	PushEvent(Event::Type::Move);
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	PushEvent(Event::Type::Leave);
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	PushEvent(Event::Type::Enter);
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	leftIsPressed = true;
	PushEvent(Event::Type::LPress);
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	leftIsPressed = false;
	PushEvent(Event::Type::LRelease);
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	rightIsPressed = true;
	PushEvent(Event::Type::RPress);
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	rightIsPressed = false;
	PushEvent(Event::Type::RRelease);
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	PushEvent(Event::Type::WheelUp);
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	PushEvent(Event::Type::WheelDown);
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::PushEvent(Event::Type type) noexcept
{
	buffer.push(Event{ type, *this });
	TrimBuffer();
}
