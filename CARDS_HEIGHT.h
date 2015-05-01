#ifndef CARDS_HEIGHT_H_INCLUDED
#define CARDS_HEIGHT_H_INCLUDED

/**
** This class is a helper, dealing with
** the height of the cards.
** The height is one particular aspect of cards, of course.
**/

enum CARDS_HEIGHT {SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING,ACE,UNINTIALIZED};
class Card_Height
{
    private :
        CARDS_HEIGHT my_height;
    public :
        Card_Height():my_height(UNINTIALIZED){}
        Card_Height(CARDS_HEIGHT height):my_height(height){}
        Card_Height(Uint iHeight):my_height(fromInt(iHeight)){}
        CARDS_HEIGHT Height() const {return my_height;}
        bool operator==(const Card_Height& height) const{ return my_height == height.Height();}
        bool operator!=(const Card_Height& height) const{ return my_height != height.Height();}
        Card_Height HeightUnder(bool isTrump)
        {
            switch(my_height)
            {
                case SEVEN :
                    return Card_Height(SEVEN);
                case EIGHT :
                    return Card_Height(SEVEN);
                case NINE :
                    return isTrump ? Card_Height(ACE) : Card_Height(EIGHT) ;
                case TEN :
                    return Card_Height(KING);
                 case JACK :
                    return Card_Height(NINE);
                case QUEEN :
                    return isTrump ? Card_Height(EIGHT) : Card_Height(JACK);
                case KING :
                    return Card_Height(QUEEN);
                case ACE :
                    return Card_Height(TEN);
                default :
                    return Card_Height();
            }
        }
        Uint ToInt() const
        {
            switch(my_height)
            {
            case SEVEN :
                return 0;
            case EIGHT :
                return 1;
            case NINE :
                return 2;
            case TEN :
                return 3;
            case JACK :
                return 4;
            case QUEEN :
                return 5;
            case KING :
                return 6;
            case ACE :
                return 7;
            default :
                return 255;
            }
        }
    private :
        CARDS_HEIGHT fromInt(Uint i)
        {
            switch(i)
            {
            case 0 :
                return SEVEN;
            case 1 :
                return EIGHT;
            case 2 :
                return NINE;
            case 3 :
                return TEN;
            case 4 :
                return JACK;
            case 5 :
                return QUEEN;
            case 6 :
                return KING;
            case 7 :
                return ACE;
            default :
                return UNINTIALIZED;
            }
        }
};



#endif // CARDS_HEIGHT_H_INCLUDED
