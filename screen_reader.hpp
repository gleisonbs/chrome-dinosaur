#ifndef SCREEN_READER_HPP
#define SCREEN_READER_HPP

#include <gtkmm.h>
#include <gdkmm.h>

struct Obstacle {
    Obstacle() { this->distance=0; this->size=0; }
    Obstacle(double distance, double size) {
        this->distance=distance;
         this->size=size;
     }

    double distance;
    double size;
    double speed;
};

class ScreenReader {
public:
    ScreenReader(int, char**);
    Obstacle next_obstacle();
    bool is_game_over();

private:
    Glib::RefPtr<Gdk::Screen> screen;
    Glib::RefPtr<Gdk::Drawable> win;
    Glib::RefPtr<Gdk::Pixbuf> pb;
};

#endif // SCREEN_READER_HPP
