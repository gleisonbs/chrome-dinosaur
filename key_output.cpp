#include <thread>

#include "key_output.hpp"

using std::string;

Display *Keyboard::display = XOpenDisplay(0);
int Keyboard::press_time = 0;
string Keyboard::previous_key = "Up";
std::chrono::steady_clock::time_point Keyboard::press_clock = std::chrono::steady_clock::now();

bool Keyboard::press(std::string key_name, int for_milliseconds) {
    if (not Keyboard::release(previous_key)) return false;

    KeyCode key_code = XKeysymToKeycode(Keyboard::display, XStringToKeysym(key_name.c_str()));
    XTestFakeKeyEvent(Keyboard::display, key_code, True, 0);
    XFlush(Keyboard::display);

    Keyboard::press_time = for_milliseconds;
    Keyboard::press_clock = std::chrono::steady_clock::now();
    Keyboard::previous_key = key_name;

    return true;
}

bool Keyboard::release(std::string key_name) {
    int press_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - Keyboard::press_clock).count();
    if (press_time < Keyboard::press_time) return false;

    KeyCode key_code = XKeysymToKeycode(Keyboard::display, XStringToKeysym(key_name.c_str()));
    XTestFakeKeyEvent(Keyboard::display, key_code, False, 0);
    XFlush(Keyboard::display);

    return true;
}

void Keyboard::start_new_game() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Keyboard::press("Up");
    Keyboard::release("Up");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Keyboard::reload_page() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Keyboard::press("F5");
    Keyboard::release("F5");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
