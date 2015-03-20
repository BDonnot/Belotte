#ifndef CARDS_H
#define CARDS_H

#include "Basic_Click.h"
#include "Basic_Transparency.h"
#include "Images_Sprite_Move.h"
#include "Cards_Basic.h"

class Cards :public Cards_Basic, public Images_Sprite_Move<2>,public Basic_Click,public Basic_Transparency
{
    protected:
        //const Basic_Game_Info _basic_info;
        bool _Up; //true if the card if up
        bool _first_sprite;
        Uint32 _half_duration;
//        static std::array<std::array<bool,8>,8 > _matrix_win_trump;
//        static std::array<std::array<bool,8>,8 > _matrix_win_no_trump; //No_trump[height_1][height_2] : return true if height_2 wins over height_1

    public:
        Cards(){}
        Cards(CARDS_HEIGHT height,CARDS_COLOR color,SDL_Event* event);
        virtual ~Cards(){}
        void Update_on_it(); //true if the mouse is on it

        void InitMouvement(bool transparency,PositionGraphic pos,Uint32 duration,Uint32 time_lag);//to init a mouvement
        void UpdatePositionHand(PositionGraphic& pos_end);

        void Up(bool go_up); //up the card if go_up, otherwise down the card
        void Reveal(Uint32 duration,Uint32 time_lag,Uint32 current_time); //reveal or hide the card, depending on which part of the card is visible
        void Reset();

        const bool& GetUp() const;

    protected:
        void act();

    private:
        Cards(const Cards& other);
        Cards& operator=(const Cards& other);
        void Revealing();
        void updateMouvement(); //to update the timers for the movement
        //Uint HeightToInt(CARDS_HEIGHT height_card) const; //TO DO : remove

};

#endif // CARDS_H
