#ifndef IMAGES_TEXT_H
#define IMAGES_TEXT_H

#include "Basic_Images.h"
#include "Basic_Text.h"


class Images_Text : public Basic_Images,public Basic_Text
{
    public:
        Images_Text();
        Images_Text(std::string path_font,int size,char colR,char colG,char colB,std::string text);
        virtual ~Images_Text();
        //void ChangeText(std::string text);
        virtual void Display(SDL_Surface* destination);
    protected:



    private:
};

#endif // IMAGES_TEXT_H
