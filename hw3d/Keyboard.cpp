#include "Keyboard.h"

bool Keyboard::KeyIsPressed(code_t keycode) const noexcept
{
    return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
    if (keybuffer.size() > 0u)
    {
        Keyboard::Event e = keybuffer.front();
        keybuffer.pop();
        return e;
    }
    return Event();
}

bool Keyboard::KeyIsEmpty() const noexcept
{
    return keybuffer.empty();
}

bool Keyboard::CharIsEmpty() const noexcept
{
    return charbuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    keybuffer = decltype(keybuffer){};
}

char Keyboard::ReadChar() noexcept
{
    if (charbuffer.size() > 0u)
    {
        code_t charcode = charbuffer.front();
        charbuffer.pop();
        return charcode;
    }
    return 0;
}

void Keyboard::FlushChar() noexcept
{
    charbuffer = decltype(charbuffer){};
}

void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
    autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(code_t keycode) noexcept
{
    keystates[keycode] = true;
    keybuffer.push(Event{ Event::Type::Press, keycode });
    TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(code_t keycode) noexcept
{
    keystates[keycode] = false;
    keybuffer.push(Event{ Event::Type::Press, keycode });
    TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
    charbuffer.push(character);
    TrimBuffer(charbuffer);
}

void Keyboard::CleareState() noexcept
{
    keystates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}