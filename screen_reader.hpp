#ifndef SCREEN_READER_HPP
#define SCREEN_READER_HPP

#include <X11/Xutil.h>

struct Obstacle {
    Obstacle() { this->distance=0; this->size=0; }
    Obstacle(double distance, double size) {
        this->distance=distance;
         this->size=size;
     }

    double distance;
    double size;
};

class ScreenReader {
public:
    ScreenReader();
    ~ScreenReader();
    Obstacle next_obstacle();
    bool is_game_over();

private:
    void find_ground();

    Display *display;
    int ground_extension = 600;
    int ground_start_x = 0;
    int ground_start_y = 0;
};

#endif // SCREEN_READER_HPP
