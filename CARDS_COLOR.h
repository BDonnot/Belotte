#ifndef CARDS_COLOR_H_INCLUDED
#define CARDS_COLOR_H_INCLUDED
enum CARDS_COLOR {DIAMOND,HEART,SPADE,CLUB,ALL_TRUMP,NO_TRUMP,NO,NOT_CHOSEN}; //carreau,coeur,pique,trefle, non je ne prends pas,je n'ai pas choisi
class Card_Color
{
    private :
        CARDS_COLOR my_color;
    public :
        Card_Color():my_color(){}
        Card_Color(CARDS_COLOR color):my_color(color){}
        Card_Color(Uint iCol):my_color(fromInt(iCol)){}
        CARDS_COLOR Color() const {return my_color;}
        bool operator==(const Card_Color& color) const{ return my_color == color.Color();}
        bool operator!=(const Card_Color& color) const{ return my_color != color.Color();}
        Uint ToInt() const
        {
            switch(my_color)
            {
            case DIAMOND :
                return 0;
            case HEART :
                return 1;
            case SPADE :
                return 2;
            case CLUB :
                return 3;
            case ALL_TRUMP :
                return 4;
            case NO_TRUMP :
                return 5;
            case NO :
                return 6;
            case NOT_CHOSEN :
                return 7;
            default :
                return 255;
            }
        }
    private :
        CARDS_COLOR fromInt(Uint i)
        {
            switch(i)
            {
            case 0 :
                return DIAMOND;
            case 1 :
                return HEART;
            case 2 :
                return SPADE;
            case 3 :
                return CLUB;
            case 4 :
                return ALL_TRUMP;
            case 5 :
                return NO_TRUMP;
            case 6 :
                return NO;
            case 7 :
                return NOT_CHOSEN;
            default :
                return NOT_CHOSEN;
            }
        }
};
#endif // CARDS_COLOR_H_INCLUDED
