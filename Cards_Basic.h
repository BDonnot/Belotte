#ifndef CARDS_BASIC_H
#define CARDS_BASIC_H

#include <array>
#include<string>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "PositionGraphic.h" //non UI Related ...
/**This class implement the basic concept of a card, and the related methods. It does not depend on the graphic interface**/
//TO DO : find a more elegant way for 'getString()'

class Cards_Basic
{
    protected :
        Basic_Game_Info _infos;
        const Card_Height _height_card;
        const Card_Color _color_card;

        std::array<std::array<bool,8>,8 > _matrix_win_trump;
        std::array<std::array<bool,8>,8 > _matrix_win_no_trump; //No_trump[height_1][height_2] : return true if height_2 wins over height_1
        static std::array<Uint,8> _value_trump;
        static std::array<Uint,8> _value_no_trump;
    public:
        Cards_Basic():_height_card(UNINTIALIZED),_color_card(NOT_CHOSEN){}
        Cards_Basic(const Card_Height& height,const Card_Color& color);
        virtual ~Cards_Basic(){}

        const Card_Color& GetColour() const;
        const Card_Height& GetHeight() const;
        const Uint& Value() const;

        bool Win(const Card_Height& height_other) const; //true if the card win | card1.Win(heightCard2) == true if card1 beats card 2.
        std::string GetString() const;

    protected:
    private:
};

#endif // CARDS_BASIC_H
