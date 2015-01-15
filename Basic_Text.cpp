#include "Basic_Text.h"
using namespace::std;
Basic_Text::Basic_Text()
{
    //ctor
}
Basic_Text::Basic_Text(std::string path_font,int size,SDL_Color color,std::string text):_size(size),_color(color)
{
    _keystates = SDL_GetKeyState( NULL );
    _font = TTF_OpenFont( path_font.c_str(), _size );
    _current_text = text;
    _change = false;
}
Basic_Text::~Basic_Text()
{
}
const string& Basic_Text::GetText() const
{
    return _current_text;
}
SDLKey Basic_Text::Convert(SDLKey key)
{
    switch(key)
    {
        case SDLK_q : return SDLK_a ;
        case SDLK_w : return SDLK_z ;
        case SDLK_a : return SDLK_q ;
        case SDLK_SEMICOLON : return SDLK_m;
        case SDLK_z : return SDLK_w ;
        case SDLK_m : return SDLK_COMMA;
        default : return key;
    }
}
string Basic_Text::Character(SDLKey key)
{
    if ((_keystates[SDLK_LSHIFT])||(_keystates[SDLK_RSHIFT]))
    {
        switch(key)
        {
            case SDLK_a : return "A" ;
            case SDLK_b : return "B" ;
            case SDLK_c : return "C" ;
            case SDLK_d : return "D" ;
            case SDLK_e : return "E" ;
            case SDLK_f : return "F" ;
            case SDLK_g : return "G" ;
            case SDLK_h : return "H" ;
            case SDLK_i : return "I" ;
            case SDLK_j : return "J" ;
            case SDLK_k : return "K" ;
            case SDLK_l : return "L" ;
            case SDLK_m : return "M" ;
            case SDLK_n : return "N" ;
            case SDLK_o : return "O" ;
            case SDLK_p : return "P" ;
            case SDLK_q : return "Q" ;
            case SDLK_r : return "R" ;
            case SDLK_s : return "S" ;
            case SDLK_t : return "T" ;
            case SDLK_u : return "U" ;
            case SDLK_v : return "V" ;
            case SDLK_w : return "W" ;
            case SDLK_x : return "X" ;
            case SDLK_y : return "Y" ;
            case SDLK_z : return "Z" ;
            case SDLK_0 : return "0" ;
            case SDLK_1 : return "1" ;
            case SDLK_2 : return "2" ;
            case SDLK_3 : return "3" ;
            case SDLK_4 : return "4" ;
            case SDLK_5 : return "5" ;
            case SDLK_6 : return "6" ;
            case SDLK_7 : return "7" ;
            case SDLK_8 : return "8" ;
            case SDLK_9 : return "9" ;
            case SDLK_SPACE : return " ";
            case SDLK_BACKSPACE : return "@";
            default : return "";
        }
    }
    else
    {
        switch(key)
        {
            case SDLK_a : return "a" ;
            case SDLK_b : return "b" ;
            case SDLK_c : return "c" ;
            case SDLK_d : return "d" ;
            case SDLK_e : return "e" ;
            case SDLK_f : return "f" ;
            case SDLK_g : return "g" ;
            case SDLK_h : return "h" ;
            case SDLK_i : return "i" ;
            case SDLK_j : return "j" ;
            case SDLK_k : return "k" ;
            case SDLK_l : return "l" ;
            case SDLK_m : return "m" ;
            case SDLK_n : return "n" ;
            case SDLK_o : return "o" ;
            case SDLK_p : return "p" ;
            case SDLK_q : return "q" ;
            case SDLK_r : return "r" ;
            case SDLK_s : return "s" ;
            case SDLK_t : return "t" ;
            case SDLK_u : return "u" ;
            case SDLK_v : return "v" ;
            case SDLK_w : return "w" ;
            case SDLK_x : return "x" ;
            case SDLK_y : return "y" ;
            case SDLK_z : return "z" ;
            case SDLK_0 : return "0" ;
            case SDLK_1 : return "1" ;
            case SDLK_2 : return "2" ;
            case SDLK_3 : return "3" ;
            case SDLK_4 : return "4" ;
            case SDLK_5 : return "5" ;
            case SDLK_6 : return "6" ;
            case SDLK_7 : return "7" ;
            case SDLK_8 : return "8" ;
            case SDLK_9 : return "9" ;
            case SDLK_SPACE : return " ";
            case SDLK_BACKSPACE : return "@";
            default : return "";
        }
    }
}
string Basic_Text::DeleteChar(string str)
{
    if (str == "") return "";
    string res = "";
    int n = str.size() ;
    for (int i = 0; i < n-1; i++)
    {
        res+=str[i];
    }
    return res;
}
void Basic_Text::ChangeText(std::string text)
{
    if (text == _current_text) return ;
    _current_text = text;
    _change = true;
}
