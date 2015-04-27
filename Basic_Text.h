#ifndef BASIC_TEXT_H
#define BASIC_TEXT_H

#include <string>

#if defined(__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_ttf.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_ttf.h>
#endif //SDL include

class Basic_Text
{
    public:
        Basic_Text();
		Basic_Text(std::string path_font, int size, char colR, char colG, char colB, std::string text);
        virtual ~Basic_Text();
        const std::string& GetText() const;
        void ChangeText(std::string text);
    protected:
        int _size; //the size of the text
        Uint8* _keystates; //state of the key : pressed / not pressed
        TTF_Font* _font; //the font of te writing
        SDL_Color _color; //the color of the writing
        std::string _current_text; //the text
        bool _change;

        SDLKey Convert(SDLKey); //convert keyboard qwerty in keyboard azerty
        std::string Character(SDLKey); //deal with maj/min
        std::string DeleteChar(std::string strs); //to delete the last character of a string
    private:
};

#endif // BASIC_TEXT_H
