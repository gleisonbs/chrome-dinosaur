//g++ -std=c++11 screen_reader.cpp -o screen_reader
#include <iostream>

#include "screen_reader.hpp"

ScreenReader::ScreenReader() {
    this->display = XOpenDisplay(nullptr);
    this->ground_extension = 600;
    this->find_ground();
}

ScreenReader::~ScreenReader() {
    XCloseDisplay(this->display);
}

bool ScreenReader::is_game_over() {
    int x_start = this->ground_start_x + 205;
    int x_end = this->ground_start_x + 385;
    int x_size = x_end - x_start;
    int y_height = this->ground_start_y - 83;

    XImage *image;
    image = XGetImage(display, RootWindow(display, DefaultScreen(display)),
        x_start, y_height, x_size, 1, AllPlanes, XYPixmap);
    XColor color;

    int count = 0;
    for (int x = 0; x < x_size; x++) {
        color.pixel = XGetPixel(image, x, 0);
        XQueryColor(display, DefaultColormap(display, DefaultScreen (display)),
            &color);

        if (color.red/256 == 83 or
            color.red/256 == 172) count++;
    }

    XDestroyImage(image);
    return (count >= 40);
}

Obstacle ScreenReader::next_obstacle() {
    int x_start = this->ground_start_x + 95;
    int y_height = this->ground_start_y - 10;
    int x_size = this->ground_extension;

    XImage *image;
    image = XGetImage(this->display, RootWindow(this->display, DefaultScreen(this->display)),
        x_start, y_height, x_size, 1, AllPlanes, XYPixmap);
    XColor color;

    double obstacle_start = 0;
    double obstacle_size = 0;
    for (int x = 0; x < x_size; x++) {

        color.pixel = XGetPixel(image, x, 0);
        XQueryColor(this->display, DefaultColormap(this->display, DefaultScreen (this->display)),
            &color);

        if (color.red/256 == 83 or // found a cactus (day)
            color.red/256 == 172) { // // found a cactus (night)
            if (obstacle_start == 0) obstacle_start = x; // get cactus position
            obstacle_size++;
            x_size = x + 20; // look for more 20 pixels
        }
    }

    XDestroyImage(image);
    return Obstacle(obstacle_start/1000, obstacle_size/100);
}

void ScreenReader::find_ground() {
    std::cout << "-> Finding the ground. This may take a while..." << std::endl;

    constexpr int x_start = 0;
    constexpr int y_start = 0;
    constexpr int x_end = 3840;
    constexpr int y_end = 1080;

    XImage *image;
    image = XGetImage(this->display, RootWindow(display, DefaultScreen(this->display)),
        x_start, y_start, x_end, y_end, AllPlanes, XYPixmap);
    XColor color;

    for (int x = 0; x < x_end-x_start; x++) {
        for (int y = 0; y < y_end-y_start; y++) {
            bool found = true;

            color.pixel = XGetPixel(image, x, y);
            XQueryColor(this->display, DefaultColormap(this->display, DefaultScreen (this->display)), &color);
            if (color.red/256 == 83) {
                color.pixel = XGetPixel(image, x-1, y);
                XQueryColor(this->display, DefaultColormap(this->display, DefaultScreen (this->display)), &color);
                if (color.red/256 < 240) { continue; }

                for (int i = 0; i <= 5; i++) {
                    color.pixel = XGetPixel(image, x+i, y);
                    XQueryColor(this->display, DefaultColormap(this->display, DefaultScreen (this->display)), &color);
                    if (color.red/256 != 83) { found = false; break; }

                    color.pixel = XGetPixel(image, x+i, y+1);
                    XQueryColor(this->display, DefaultColormap(this->display, DefaultScreen (this->display)), &color);
                    if (color.red/256 < 240) { found = false; break; }

                    color.pixel = XGetPixel(image, x+i, y-1);
                    XQueryColor(this->display, DefaultColormap(this->display, DefaultScreen (this->display)), &color);
                    if (color.red/256 < 240) { found = false; break; }
                }

                if (found) {
                    this->ground_start_x = x + x_start;
                    this->ground_start_y = y + y_start;
                    std::cout << "-> Found the ground at pixel coordinates (x: " << x+x_start << ", y: " << y+y_start << ")" << std::endl;

                    XDestroyImage(image);
                    return;
                }
            }
        }
    }

    XDestroyImage(image);
}
