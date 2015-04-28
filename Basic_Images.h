#ifndef BASIC_IMAGES_H
#define BASIC_IMAGES_H

#include <string>
#include <array>
#include <memory>

#if defined(__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_image.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_image.h>
#endif //SDL include

#include "Basic_Game_Info.h"
#include "PositionGraphic.h"

class DeleteSurface
{
public :
    DeleteSurface(){}
    void operator()(SDL_Surface* surface)
    {
        SDL_FreeSurface(surface);
    }
};
typedef std::unique_ptr<SDL_Surface,DeleteSurface> SMARTSurface;

class Basic_Images
{
    protected:
        const Basic_Game_Info _info;
        int _height ; //height
        int _width ; //width
        //Uint16 _pos_x ; //position on x
        //Uint16 _pos_y ; //position on y
        //POSITION_IMAGE _pos_option;
        PositionGraphic _pos;
//        SDL_Surface* _surface ; //the surface
         SMARTSurface _surface ; //the surface
    public:
        Basic_Images();
        Basic_Images(std::string path);
        virtual ~Basic_Images();
        //virtual void SetPosition(Uint16 x,Uint16 y,POSITION_IMAGE option); //to set the position of the image
        virtual void SetPosition(PositionGraphic& pos); //to set the position of the image
        void Display(SDL_Surface* destination); //to display the image on the screen
        const SDL_Surface* Get_Surface() const;
        const PositionGraphic& GetPosition() const;
        std::array<Uint,2> GetSize();
    protected:

        SDL_Surface* load_image(const std::string& filename ); //to load a surface from an image
        Basic_Images(const Basic_Images& other); //to prevent copy
        Basic_Images& operator=(const Basic_Images& other); //to prevent assignment
        void apply_surface(const Uint& x,const Uint& y,const SMARTSurface& source, SDL_Surface* destination ); //to apply a surface on the screen
        virtual void act(); //the action : will be overide in the derivate classes (for example changing the position if the image is moving)
        //std::array<Uint16,2> position_image_aux();
    private:
};

#endif // BASIC_IMAGES_H
