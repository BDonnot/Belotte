#ifndef POSITION_TRICK_H_INCLUDED
#define POSITION_TRICK_H_INCLUDED
enum POSITION_TRICK {FIRST,SECOND,THIRD,FOURTH,UNKNOWN};
class Position_Trick
{
    private :
        POSITION_TRICK my_pos;
    public :
        Position_Trick():my_pos(){}
        Position_Trick(POSITION_TRICK pos):my_pos(pos){}
        Position_Trick(Uint iPos):my_pos(fromInt(iPos)){}
        POSITION_TRICK Position() const {return my_pos;}
        bool operator==(const Position_Trick& pos) const{ return my_pos == pos.Position();}
        bool operator!=(const Position_Trick& pos) const{ return my_pos != pos.Position();}
        void Next()
        {
            switch(my_pos)
            {
            case FIRST :
                my_pos = SECOND;
                break;
            case SECOND :
                my_pos = THIRD;
                break;
            case THIRD :
                my_pos = FOURTH;
                break;
            case FOURTH :
                my_pos = FIRST;
                break;
            default :
                my_pos = UNKNOWN;
                break;
            }
        }
        Position_Trick Next() const
        {
            switch(my_pos)
            {
            case FIRST :
                return Position_Trick(SECOND);
            case SECOND :
                return Position_Trick(THIRD);
            case THIRD :
                return Position_Trick(FOURTH);
            case FOURTH :
                return Position_Trick(FIRST);
            default :
                return Position_Trick(UNKNOWN);
            }
        }
        Uint ToInt() const
        {
            switch(my_pos)
            {
            case FIRST :
                return 0;
            case SECOND :
                return 1;
            case THIRD :
                return 2;
            case FOURTH :
                return 3;
            default :
                return 255;
            }
        }
    private :
        POSITION_TRICK fromInt(Uint i)
        {
            switch(i)
            {
            case 0 :
                return FIRST;
            case 1 :
                return SECOND;
            case 2 :
                return THIRD;
            case 3 :
                return FOURTH;
            default :
                return UNKNOWN;
            }
        }
};
#endif // POSITION_TRICK_H_INCLUDED
