#ifndef IMAGES_CLICK_H
#define IMAGES_CLICK_H

#include "Basic_Images.h"
#include "Basic_Click.h"


class Images_Click : public Basic_Images, public Basic_Click
{
    public:
        Images_Click();
        Images_Click(std::string path,SDL_Event* event);
        virtual ~Images_Click();
        virtual void Update_on_it(); //true if the mouse is on it
    protected:

    private:
};

#endif // IMAGES_CLICK_H
