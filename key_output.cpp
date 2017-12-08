#include <thread>

#include "key_output.hpp"

using std::string;

Keyboard::Keyboard() {
    this->display = XOpenDisplay(nullptr);
    this->previous_key = "Up";
    this->press_time = 0;
    this->press_clock = std::chrono::steady_clock::now();
}

Keyboard::~Keyboard() {
    XCloseDisplay(this->display);
}

bool Keyboard::press(std::string key_name, int for_milliseconds) {
    if (not this->release(this->previous_key)) return false;

    KeyCode key_code = XKeysymToKeycode(this->display, XStringToKeysym(key_name.c_str()));
    XTestFakeKeyEvent(this->display, key_code, True, 0);
    XFlush(this->display);

    this->press_time = for_milliseconds;
    this->press_clock = std::chrono::steady_clock::now();
    this->previous_key = key_name;

    return true;
}

bool Keyboard::release(std::string key_name) {
    int press_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->press_clock).count();
    if (press_time < this->press_time) return false;

    KeyCode key_code = XKeysymToKeycode(this->display, XStringToKeysym(key_name.c_str()));
    XTestFakeKeyEvent(this->display, key_code, False, 0);
    XFlush(this->display);

    return true;
}

void Keyboard::start_new_game() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    this->press("Up");
    this->release("Up");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Keyboard::reload_page() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    this->press("F5");
    this->release("F5");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
