#ifndef IMAGES_H
#define IMAGES_H

#include "SDL/SDL.h"
#include <string>
#include <array>


class Images
{
    public:
        Images(); //constructor by default
        Images(std::string,SDL_Event& event); //constructor with in "string" the path of the image and a pointer to a "event" for the events
        Images(int,int,std::string,SDL_Event& event);//we add also 2 numbers : the height and the width
    //Since SDL_Surface are pointers, we are forced to modify the 3 following methodes :
        virtual ~Images(); //destructor
        Images (const Images& other); //copy constructor
        Images& operator=(const Images& rhs);//operator =
    //accessors :
        SDL_Surface* GetSurface();
        double GetAlpha();
    //to apply a surface on another one :
        static void apply_surface(int,int,SDL_Surface*,SDL_Surface*); //classic method, not a method
        virtual void Apply_surface(SDL_Surface*); //to apply a surface on another one :
    //to load an image
        static SDL_Surface* load_image(std::string);
    //to begin the movements
        void SetPosition(double,double,int); //define the position (x,y,option) with option a number which :
        //if is equal to 0 : we set the image so it is located at the center of the given coordinates
        //if is equal to 1 : set the horizontal position, the vertical part is centered
        //if is equal to 2 : fixing the horizontal position, vertical position shift of the size of the image
        //if is equal to 3 : fixing the vertical position, horizontal position centered
        //if is equal to 4 : fixing the vertical position, horizontale position shift of the size of the image
        //if is equal to 5, no change for the coordinates entered
        void Set_animate(double,double,int,int,int); //the first 2 numbers are for the final position, the 3rd for the speed, the smaller the faster (0 to 4)
        void Set_animate_transparency(double,double,int,int,bool,int); //the first 3 numbers are for the final position, the 4th for the speed, the smallest it is the faster it is (0 to 4)
        void Set_transparent(bool,int,int); //to change the transparency of the image

        virtual void type(std::string); //for the class with texts, allow to write a text that will be display on the window
    //event management
        virtual void update(); //update according to the events
        bool Mouse_over(); //is the mouse over ?
        bool click_on(bool Short); //did we click on the image ?
        bool Arrived() ; //has the image arrived ?
        void restart_click(); //to force a 'false' the click on the image
    //mouvement
        bool Moving(); //does the image move ?
        void reset(); //keep the position, set all the other things to 0 : transparency, move etc.
        void Move_down();//to move down the image of a few pixels
        void Move_up(); //to move up the image of a few pixels
    //transparency
        void up_transparent(); //to increase the transparency
        void down_transparent(); //to decrease the transparency
    protected:
    //SDL
        SDL_Surface* _surface; //the pointer on a surface
        std::string _path; //path to the image
        SDL_Event* _pEvent; //pointer to an event
    //position
        int _height;
        int _width;
        double _pos_x; //position on the x axis
        double _pos_y;
        double _pos_th_x; //theorical position on the x axis
        double _pos_th_y;
        int _pos_option; //memorizing of the position of the option
    //movement
        int _nmb_movt; //the number of move the image will ahve to do
        double _mov_x; //the evolution of x during each 'mini movement'
        double _mov_y; //the evolution of y during each 'mini movement'
        int _time_lag; //to delay the beginning of a movement
        int _nb_moving; //number of movements
        int _end_x; //final position on the axis x
        int _end_y;
    //transparency
        double _alpha; //level of opacity of the image
        int _transparency; //1, 0 or -1 if we want to increase, not to change or decrease the transparency of a image
        int _nmb_movt_alpha; //number of movement in order to make the image perfectly transparent (or opaque)
    //evenement
        bool _on_it; //if the mouse is over
        bool _simple_image; //if it is a simple image
        bool _click_on_short; //if we click on
        bool _click_on_long; //if we have clicked on, and we still don't have clicked somewhere else


        virtual void position_image(); //to position the image
        std::array<double,2> position_image_aux(double,double,int); //auxiliary fonction of the previous one to determinate the positions on the x and y axis
        void animate_aux();
        int choose_nb_movt(int); //choose the number of movement
        void set_on_it(); //handle the events : is the mouse over ?
        void set_click_on_short(); //events management : do we click on the image ?
        void set_click_on_long(); //events management : did we click on the image and still not somewhere else ?
        void animate();
        void Set_animate_aux(double,double,int,int,int);
        void transparent();
        void calcule_alpha(double);
    private:
};

#endif // IMAGES_H
