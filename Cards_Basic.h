#ifndef CARDS_BASIC_H
#define CARDS_BASIC_H

#include <array>
#include<string>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "PositionGraphic.h" //non UI Related ...
/**This class implement the basic concept of a card, and the related methods. It does not depend on the graphic interface**/
//TO DO : find a more elegant way for 'getString()'

//Mouse_Over()
//Click(bool)
//Reset_Click()
//ChangeSprite(Uint)
//Display()
//SetPosition(PositionGraphic&)
class Cards_Basic
{
    protected :
        Basic_Game_Info _infos;
        const CARDS_HEIGHT _height_card;
        const CARDS_COLOR _color_card;
        std::array<std::array<bool,8>,8 > _matrix_win_trump;
        std::array<std::array<bool,8>,8 > _matrix_win_no_trump; //No_trump[height_1][height_2] : return true if height_2 wins over height_1
        static std::array<Uint,8> _value_trump;
        static std::array<Uint,8> _value_no_trump;
    public:
        Cards_Basic():_height_card(UNINTIALIZED),_color_card(NOT_CHOSEN){}
        Cards_Basic(CARDS_HEIGHT height,CARDS_COLOR color);
        virtual ~Cards_Basic(){}

        const CARDS_COLOR& GetColour() const;
        const CARDS_HEIGHT& GetHeight() const;
        const Uint& Value() const;

        bool Win(CARDS_HEIGHT height_other) const; //true if the card win | card1.Win(heightCard2) == true if card1 beats card 2.
        std::string GetString() const;

        //UI RELATED METHOD
        //virtual void InitMouvement(bool transparency,PositionGraphic pos,Uint duration,Uint time_lag){}//to init a mouvement
        //virtual void Display(){}

        //virtual void SetPosition(PositionGraphic& pos_final){}
        //virtual void ChangeSprite(Uint sprite_number){}

        /*
        virtual bool Click(bool Short){ return false;}
        virtual void Reset_Click(){}
        virtual bool Mouse_Over(){return false;}
        virtual void Update_on_it(){}


        virtual void UpdatePositionHand(PositionGraphic& pos_end){}

        virtual void Up(bool go_up){} //up the card if go_up, otherwise down the card
        virtual void Reveal(Uint duration,Uint time_lag,Uint current_time){} //reveal or hide the card, depending on which part of the card is visible
        virtual void Reset(){}

        virtual const bool GetUp() const {return false;}*/
    protected:
    private:
};

#endif // CARDS_BASIC_H
