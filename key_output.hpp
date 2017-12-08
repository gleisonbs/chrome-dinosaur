#ifndef KEY_OUTPUT_HPP
#define KEY_OUTPUT_HPP

#include <string>
#include <chrono>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

class Keyboard {
public:
    Keyboard();
    ~Keyboard();
    bool press(std::string, int=0);
    bool release(std::string);
    void start_new_game();
    void reload_page();
private:
    std::string previous_key;
    std::chrono::steady_clock::time_point press_clock;
    int press_time;
    Display *display;
};

#endif // KEY_OUTPUT_HPP
