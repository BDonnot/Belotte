#ifndef TEXT_H
#define TEXT_H

#include "Images.h"
#include "SDL/SDL_ttf.h"

class Text : public Images
{
    public:
        Text();
        Text(std::string, int,SDL_Color,SDL_Event&);
        ~Text();
        Text(const Text& other);
        Text& operator=(const Text& other);
        void typing();
        void type(std::string);
        void update();
        std::string GetText();
        TTF_Font* GetFont();
        void remove_text();
    protected:
        TTF_Font* _font;
        SDL_Color _color;
        std::string _text;
        std::string _text_old;
        int _size;
        bool _typing;

        void add_letter(char);
        void remove_letter();
        void write();
    private:
};

#endif // TEXT_H
