#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
    friend class Window;
    typedef unsigned char code_t;
public:
    class Event
    {
    public:
        enum class Type {
            Press,
            Release,
            Invalid
        };
        Event() noexcept :
            type{ Type::Invalid }, code{ 0 }{}
        Event(Type type, code_t code) noexcept :
            type{ type }, code{ code }{}
        bool IsRelase() const noexcept
        {
            return type == Type::Release;
        }
        bool IsPress() const noexcept
        {
            return type == Type::Press;
        }
        bool IsValid() const noexcept
        {
            return type != Type::Invalid;
        }
        code_t GetCode() const noexcept
        {
            return code;
        }
    private:
        Type type;
        code_t code;
    };
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool KeyIsPressed(code_t keycode) const noexcept;
    Event ReadKey() noexcept;
    bool KeyIsEmpty() const noexcept;
    void FlushKey() noexcept;

    char ReadChar() noexcept;
    bool CharIsEmpty() const noexcept;
    void FlushChar() noexcept;
    void Flush() noexcept;

    void EnableAutorepeat() noexcept;
    void DisableAutorepeat() noexcept;
    bool AutorepeatIsEnabled() const noexcept;
private:
    void OnKeyPressed(code_t keycode) noexcept;
    void OnKeyReleased(code_t keycode) noexcept;
    void OnChar(char character) noexcept;
    void CleareState() noexcept;
    template<typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
    static constexpr size_t nKeys = 256u;
    static constexpr size_t bufferSize = 16u;
    bool autorepeatEnabled;
    std::bitset<nKeys> keystates;
    std::queue<Event> keybuffer;
    std::queue<char> charbuffer;
};

