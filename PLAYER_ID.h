#ifndef PLAYER_ID_H_INCLUDED
#define PLAYER_ID_H_INCLUDED
enum PLAYER_ID {PLAYER0,PLAYER1,PLAYER2,PLAYER3,GHOST};
class Player_ID
{
    private :
        PLAYER_ID my_ID;
    public :
        Player_ID():my_ID(){}
        Player_ID(PLAYER_ID ID):my_ID(ID){}
        Player_ID(Uint iID):my_ID(fromInt(iID)){}
        PLAYER_ID ID() const {return my_ID;}
        bool operator==(const Player_ID& id) const{ return my_ID == id.ID();}
        bool operator!=(const Player_ID& id) const{ return my_ID != id.ID();}
        void Next()
        {
            switch(my_ID)
            {
            case PLAYER0 :
                my_ID = PLAYER1;
                break;
            case PLAYER1 :
                my_ID = PLAYER2;
                break;
            case PLAYER2 :
                my_ID = PLAYER3;
                break;
            case PLAYER3 :
                my_ID = PLAYER0;
                break;
            default :
                my_ID = GHOST;
                break;
            }
        }
        Player_ID Teammate() const
        {
            Player_ID res = NextPlayer();
            res.Next();
            return res;
        }
        Player_ID NextPlayer() const
        {
            switch(my_ID)
            {
            case PLAYER0 :
                return Player_ID(PLAYER1);
            case PLAYER1 :
                return Player_ID(PLAYER2);
            case PLAYER2 :
                return Player_ID(PLAYER3);
            case PLAYER3 :
                return Player_ID(PLAYER0);
            default :
                return Player_ID(GHOST);
            }
        }
        Uint ToInt() const
        {
            switch(my_ID)
            {
            case PLAYER0 :
                return 0;
            case PLAYER1 :
                return 1;
            case PLAYER2 :
                return 2;
            case PLAYER3 :
                return 3;
            default :
                return 255;
            }
        }
    private :
        PLAYER_ID fromInt(Uint i)
        {
            switch(i)
            {
            case 0 :
                return PLAYER0;
            case 1 :
                return PLAYER1;
            case 2 :
                return PLAYER2;
            case 3 :
                return PLAYER3;
            default :
                return GHOST;
            }
        }
};
#endif // PLAYER_ID_H_INCLUDED
