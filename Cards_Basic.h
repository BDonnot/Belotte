#ifndef CARDS_BASIC_H
#define CARDS_BASIC_H

#include <array>
#include<string>

#include "Definitions.h"
#include "Basic_Game_Info.h"
/**This class implement the basic concept of a card, and the related methods. It does not depend on the graphic interface**/
//TO DO : find a more elegant way for 'getString()'
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
    protected:
    private:
};

#endif // CARDS_BASIC_H
