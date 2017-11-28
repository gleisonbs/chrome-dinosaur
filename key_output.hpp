#ifndef KEY_OUTPUT_HPP
#define KEY_OUTPUT_HPP

#include <string>
#include <chrono>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

class Keyboard {
public:
    static bool press(std::string, int=0);
    static bool release(std::string);
    static void start_new_game();
    static void reload_page();
private:
    static std::string previous_key;
    static std::chrono::steady_clock::time_point press_clock;
    static int press_time;
    static Display *display;
};

#endif // KEY_OUTPUT_HPP
