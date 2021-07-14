#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include <cmath>

#include <string>
#include <chrono>
#include <optional>
#include <iostream>

namespace Progress {

struct Style {
    std::string _begin = "[";
    std::string _empty = " ";
    std::string _full  = "=";
    std::string _end   = "]";
};

const struct Style solid_utf8 = { "", "\u2593", "\u2591", "" };
const struct Style line_utf8 = { "", "\u2576", "\u2501", "" };
const struct Style boxes_utf8 = { "", "\u25A3", "\u25A1", "" };
const struct Style filled_utf8 = { "", "\u25AB", "\u25FC", "" };

class Bar {
  public:
    Bar() = default;
    ~Bar() { draw(); }

    Bar(const Bar &) = delete;
    Bar& operator=(const Bar &) = delete;

    Bar& setDescription(const std::string& description) { _description = description;                    return *this; }
    Bar& setProgress   (size_t progress)                { _progress = progress;                          return *this; }
    Bar& setTotal      (size_t total)                   { _total = total;                                return *this; }
    Bar& setTimeout    (size_t timeout)                 { _timeout = std::chrono::milliseconds(timeout); return *this; }
    Bar& setStream     (std::ostream& stream)           { _stream = &stream;                             return *this; }
    Bar& setStyle      (const Style& style)             { _style = style;                                return *this; }

    Bar& operator++()              { _progress++;        show(); return *this; }
    Bar& operator+=(size_t delta)  { _progress += delta; show(); return *this; }

    inline void show() const;

  private:
    inline void   draw() const;
    inline size_t consoleWidth() const;
    inline size_t barLength() const;
    inline void percent(std::ostream& os, double ratio) const;

    std::string               _description;
    size_t                    _progress  = 0;
    size_t                    _total     = 0;
    std::chrono::milliseconds _timeout   = std::chrono::milliseconds(100);
    std::ostream             *_stream    = &std::cerr;
    bool                      _silent    = false;
    Style                     _style;

    mutable std::optional<std::chrono::time_point<std::chrono::steady_clock>> _timestamp;
};

void Bar::show() const
{
    if (_timestamp)
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = now - *_timestamp;
        if (duration < _timeout)
        {
          return;
        }
        _timestamp = now;
    }
    else
    {
        _timestamp = std::chrono::steady_clock::now();
    }
    draw();
}

void Bar::percent(std::ostream& os, double ratio) const
{
    size_t i = size_t(ratio*1000.0 + 0.5);
    os << (i<1000 ? ' ' : char('0' + i/1000));
    os << (i<100  ? ' ' : char('0' + (i/100)%10));
    os <<                 char('0' + (i/10)%10);
    os << '.' <<          char('0' + i%10);
}

void Bar::draw() const
{
    if (_stream)
    {
        const double ratio = !_total || !_progress ? 0.0 :
            (_progress >= _total ? 1.0 : double(_progress)/double(_total));
        const size_t size = barLength();
        const size_t full = std::floor(size*ratio);
        const size_t empty = size - full;

        *_stream << std::string("\r") + _description + " " + _style._begin;
        for (size_t i=0; i<full;  ++i) *_stream << _style._full;
        for (size_t i=0; i<empty; ++i) *_stream << _style._empty;
        *_stream << _style._end;

        *_stream << " ";
        percent(*_stream, ratio);
        *_stream << "% ";
        _stream->flush();
    }
}

size_t Bar::consoleWidth() const
{
#ifdef _WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left;
#else
        struct winsize win;
        if (ioctl(0, TIOCGWINSZ, &win) != -1)
        {
           return win.ws_col;
        }
#endif

    return 80;
}

size_t Bar::barLength() const
{
    int64_t length = consoleWidth();
    length -= _description.length() + 1;
    length -= _style._begin.length();
    length -= _style._end.length();
    length -= 6 + 1;  // 100.0%
    length -= 3; // three spaces on the right
    return size_t(std::max<int64_t>(length, 0));
}

}
