#ifndef BUTTON_H
#define BUTTON_H

#include "Images.h"
#include "SDL/SDL_ttf.h"

class Button : public Images
{
    public:
        Button();
        Button(std::string,SDL_Event&);
        virtual ~Button();
        Button(const Button& other);
        Button& operator=(const Button& other);
        void Apply_surface(SDL_Surface*);
        void type(std::string);
        void update();
    protected:
        SDL_Surface* _surface2;
        SDL_Surface* _text_surface;
        SDL_Surface* _text_surface2;
        double _pos_text_x;
        double _pos_text_y;
        TTF_Font* _font;
        SDL_Color _color;
        SDL_Color _color2;
        std::string _path2;
        std::string _text;
        int _size;
        std::string _path_font;
        double _width_text;
        double _height_text;

        void position_image();
    private:
};

#endif // BUTTON_H
