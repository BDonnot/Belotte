#ifndef TEXT_TYPING_H
#define TEXT_TYPING_H
#include "Basic_Text.h"
#include "Images_Click.h"
#include <list>


class Text_Typing : public Images_Click,public Basic_Text
{
    public:
        Text_Typing();
        Text_Typing(std::string path_font,int size,SDL_Color color,SDL_Event* event);
        Text_Typing(std::string path_font,int size,SDL_Color color,SDL_Event* event,std::string text);
        virtual ~Text_Typing();
        void Update_Text_OnIt();
        void Change_Text(std::string text);
    protected:
        std::string _text_old;

        void act();
    private:
};

#endif // TEXT_TYPING_H
