//g++ -std=c++11 screen_reader.cpp -o screen_reader `pkg-config gtkmm-2.4 --cflags --libs`
#include <iostream>
#include <memory>

#include "screen_reader.hpp"

using namespace std;

ScreenReader::ScreenReader(int argc, char **argv) {
    Gtk::Main kit(argc, argv);
    this->screen = Gdk::Screen::get_default();
    this->win = this->screen->get_root_window();
}

bool ScreenReader::is_game_over() {
    int x_start = 385;
    int x_end = 565;
    this->pb = Gdk::Pixbuf::create(this->win, x_start, 190, x_end-x_start, 1);
    unsigned char* pixel_line = this->pb->get_pixels();

    int count = 0;
    for (int i = 0; i < x_end - x_start; i++) {
        if ((int)(pixel_line++)[0] == 83 or
            (int)(pixel_line++)[0] == 172) count++;
    }

    return (count >= 50);
}

Obstacle ScreenReader::next_obstacle() {
    constexpr int x_start = 275;
    constexpr x_end = 1700;
    int x_size = x_end - x_start;
    this->pb = Gdk::Pixbuf::create(this->win, x_start, 265, x_size, 1);
    unsigned char* pixel_line = this->pb->get_pixels();

    double obstacle_start = -1;
    double obstacle_size = 0;
    for (int x = 0; x < x_size; x++) {
        if ((int)(pixel_line+x)[0] == 83) { // found a cactus
            if (obstacle_start == -1) obstacle_start = x; // get cactus position
            obstacle_size++;
            x_size = x + 20; // look for more 20 pixels
        }
    }

    obstacle_start = (obstacle_start == -1) ? 0 : obstacle_start;
    return Obstacle(obstacle_start/1000, obstacle_size);
}
