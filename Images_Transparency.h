#ifndef IMAGES_TRANSPARENCY_H
#define IMAGES_TRANSPARENCY_H

#include "Basic_Images.h"
#include "Basic_Transparency.h"


class Images_Transparency : public Basic_Images,public Basic_Transparency
{
    public:
        Images_Transparency();
        Images_Transparency(std::string path); //by default : the image is totally visible
        Images_Transparency(std::string path, Uint16 transparency);
        virtual ~Images_Transparency();
    protected:
        virtual void act(); //overriding of the method act : ajust transparency
    private:
};

#endif // IMAGES_TRANSPARENCY_H
