#include "AIPlayScores.h"

AIPlayScores::AIPlayScores()
{
    //ctor
}

AIPlayScores::~AIPlayScores()
{
    //dtor
}
std::list<Cards*>::iterator AIPlayScores::Play(const std::array<Cards*,4>& trick
                                       ,std::list<std::list<Cards*>::iterator>& playbleCards
                                       ,const std::list<Cards*>& hand
                                       ,Random& rand
                                       ,const TrickStatus& trickStatus)
{
    std::list<Cards*>::iterator res = playbleCards.begin();
    std::list<Cards*>::iterator pcard= playbleCards.begin();
    int MaxScore = -10000,currentScore;
    CARDS_COLOR color_asked = trickStatus.ColourAsked();
    CARDS_COLOR color_trump = _infos.TrumpColor();
    bool pisse; //I can play another colour than the one asked, or trump
    bool play_trump; //I can play at least one trump
    for (auto it : _playable_cards)
    {
        pcard = **it;
        if (( pcard->GetColour() != color_asked)&&( pcard->GetColour() != color_trump)) pisse = true;
        if ( pcard->GetColour() == color_trump) play_trump = true;
    }
    for(auto itCard : playbleCards)
    {
        switch(trickStatus.Position()) //TO DO : find a more elegant way
        {
        case FIRST :
            currentScore =  playFirst(**itCard,trick,trickStatus,rand,pisse,play_trump);
            break;
        case SECOND :
            currentScore =  playSecond(**itCard,trick,trickStatus,rand,pisse,play_trump);
            break;
        case THIRD :
            currentScore =  playThird(**itCard,trick,trickStatus,rand,pisse,play_trump);
            break;
        case FOURTH :
            currentScore =  playFourth(**itCard,trick,trickStatus,rand,pisse,play_trump);
            break;
        default : //TO DO : exception here
            currentScore = -10000;
            break;
        }

        if(currentScore > MaxScore)
        {
            res = *itCard;
            MaxScore = score;
        }
    }
    return res;
}

//int AIPlayScores::playFirst(Carte* pcard, int color_trump,int player_take, int team_taken,bool play_trump,int pseudo_height_trump_max, int number_trick)
int AIPlayScores::playFirst(Cards*pcard,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump)
{
    int res=0;
    CARDS_COLOR color = pcard->GetColour();
    CARDS_HEIGHT height = pcard->GetHeight();
    CARDS_COLOR color_trump = _infos.TrumpColor();
    POSITION_PLAYER player_take = _infos.Taker();
    int team_taken = _infos.PosPlayerToInt(player_take)%2;
    //int pseudo_height = pcard ->convertion(color_trump);


    if (pcard == nullptr) return -10000; //TO DO : exception here
    //trumps case
    if (play_trump&&(color==color_trump))
    {
        res += _play_1st.value(0,0); //res = 20
        if (pseudo_height==pseudo_height_trump_max)
        {
            if (number_trick == 6) res += _play_1st.value(1,0); //res = -17
            else res += _play_1st.value(2,0); //res=100
        }
        if (height==3)  res += _play_1st.value(3,0); //ten;res=7
        if (height==2)  res += _play_1st.value(4,0); //nine;res=-20
        if (height==7)  res += _play_1st.value(5,0);; //Ace;res=-15
        if (height==4)  res += _play_1st.value(6,0); //Jack;res=100
    }
    if (!play_trump&&(color==color_trump)) res += _play_1st.value(7,0); //res=-15
    //Case of friend call : TO DO
    /*
    if (_calls[modulo(_number+2,4)*4+color]) //add an additional condition : if the teammate does a call and we are not the strongest in this colour
    {
        res += _play_1st.value(8,0); //res=5
        if (height==3) //10
        {
            res += _play_1st.value(9,0); //res=-10
            if (opponents_cut(color)) res += _play_1st.value(10,0); //res=-10
        }
        if (height==6) res += _play_1st.value(11,0); //res=8
        if (height==5) //queen
        {
            if (opponents_cut(color)) res += _play_1st.value(12,0); //res=-10
            else res += _play_1st.value(13,0); //res=7
        }
        if (height==4) res += _play_1st.value(14,0); //res=4
        if (_nb_color_played[color] == 0) res += _play_1st.value(15,0); //res=7
        if (opponents_cut(color))
        {
            if (_number%2 == team_taken) res += _play_1st.value(16,0); //res=-5
            else res += _play_1st.value(17,0); //res=7
        }
    }
    */
    //Case of the major card
    if (pcard->convertion(color_trump) == _color_master[color])
    {
        res += _play_1st.value(18,0); //res=12
        if (height==3) //10
        {
            res += _play_1st.value(19,0); //17
            if (opponents_cut(color)) res += _play_1st.value(20,0); //res=-25
        }
        if (height==7) //ace
        {
            res += _play_1st.value(21,0); //res=35
            if (opponents_cut(color)) res += _play_1st.value(22,0); //res=-30
        }
        if (height==6) //king
        {
            res += _play_1st.value(23,0); //res=5
            if (opponents_cut(color)) res += _play_1st.value(24,0); //res=-12
        }
        if (_nb_color_played[color] ==0)
        {
            if (pseudo_height == 2) res += _play_1st.value(25,0); // res=-15
            if (pseudo_height == 3) res += _play_1st.value(26,0); // res=-9
        }
        if ((number_trick == 6)&&(color == color_trump)) res += _play_1st.value(27,0); //res=-10
    }
    //We try to not play in the "longe" of our opponents : TO DO
    //if (_calls[((_number+1)%4)*4+color]||_calls[modulo(_number+1,4)*4+color]) res += _play_1st.value(28,0); //res=-9
    return res;
}
//int AIPlayScores::playSecond(Carte* pcard, array<Carte*,4> trick,int color_trump,int player_take, int team_take,int i_master,bool pisse,bool play_trump,int number_trick,int color_asked,int height_master,int pseudo_height_master)
int AIPlayScores::playSecond(Cards*,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump)
{
    int res = 0;
    if (pcard==NULL) return -1000;
    int color = pcard->GetColour();
    int height= pcard->GetHeight();
    int pseudo_height = pcard->convertion(color_trump);
//If I have to play a trump
    if ((play_trump)&&(color==color_trump))
    {
        if (color==color_asked) //if the colour asked is the trump color
        {
            if (is_master(color_trump,color)) res += _play_2nd.value(0,0); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (pseudo_height == 2) res += _play_2nd.value(1,0); //res=-55
                if (pseudo_height == 3) res += _play_2nd.value(2,0); //res=-30
                if (pseudo_height == 4) res += _play_2nd.value(3,0); //res=-20
                if (height==smallest(color_trump,color_trump)) res += _play_2nd.value(4,0); //res=30
            }
        }
        else //we play a trump but the color asked is not the trump
        {
            if (height==smallest(color_trump,color)) res += _play_2nd.value(5,0); //res=10
            if (height==greatest(color_trump,color)) res += _play_2nd.value(6,0); //res=-30
            if (pseudo_height == 1) res += _play_2nd.value(32,0); //res=-20
            if (pseudo_height == 2) res += _play_2nd.value(7,0); //res=-20
            if (pseudo_height == 3) res += _play_2nd.value(8,0); //res=-10
        }
    }
//If I am the strongest in the color
    if ((pseudo_height == _color_master[color])&&(color==color_asked))
    {
        res += _play_2nd.value(9,0); //res=15
        if (height==3) //10
        {
            res += _play_2nd.value(10,0); //res=10
            if (opponents_cut(color)) res += _play_2nd.value(11,0); //res=-17
        }
        if (height==7) res += _play_2nd.value(12,0); //res=15
        if (height==6) res += _play_2nd.value(13,0); //res=5
        if (_nb_color_played[color] ==0)
        {
            if (height == 3) res += _play_2nd.value(14,0); //res=-20
            if (height == 6) res += _play_2nd.value(14,0); //res=-7
        }
        if (opponents_cut(color)) res += _play_2nd.value(16,0); //res=-20
        if ((number_trick == 6)&&(color == color_trump)) res += _play_2nd.value(17,0); //res=-10
    }
    //I have the colour and I am not the strongest
    if ((color==color_asked)&&(pseudo_height != _color_master[color]))
    {
        if (opponents_cut(color)) res += _play_2nd.value(18,0); //res=-10
        if (pseudo_height == 2) res += _play_2nd.value(19,0); //res=-30
        if (pseudo_height == 3) res += _play_2nd.value(20,0); //res=-10
        if (pseudo_height == 4) res += _play_2nd.value(21,0); //res=-5
    }
//the case of the 10 is taken into account in the case of "pisse"
//If we "pisse"(calls)
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (is_master(color_trump,color)) res += _play_2nd.value(22,0); //res=-15
        if (height==greatest(color_trump,color)) res += _play_2nd.value(23,0); //res=-15
        if (height==smallest(color_trump,color)) res += _play_2nd.value(24,0); //res=15
        //we are risk-adverse (more than in the position 3) and the aversion is inversely proportional to the pseudo_height of the card
        if (pseudo_height == 1) res += _play_2nd.value(25,0); //res=-100
        if (pseudo_height == 2) res += _play_2nd.value(26,0); //res=-30
        if (pseudo_height == 3) res += _play_2nd.value(27,0); //res=-10
        if (pseudo_height == 4) res += _play_2nd.value(28,0); //res=-5
        if (pseudo_height == 5) res += _play_2nd.value(29,0); //res=-2
        if (score_longe(color_trump,color) > 5) res += _play_2nd.value(30,0); //res=7
        if (score_longe(color_trump,color) > 8) res += _play_2nd.value(31,0); //res=10
    }
//if the cards value something and the that it is not the color playd, we don't play the card
    if (color!=color_asked)
    {
        if (pseudo_height==1) res += _play_2nd.value(33,0); //res = -25
    }
    return res;
}
//int AIPlayScores::playThird(Carte* pcard, array<Carte*,4> trick,int color_trump,int player_take, int team_take,int i_master,bool pisse,bool play_trump,int number_trick,int color_asked,int height_master,int pseudo_height_master)
int AIPlayScores::playThird(Cards*,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump)
{
    int res = 0;
    int color = pcard->GetColour();
    int height= pcard->GetHeight();
    int pseudo_height = pcard->convertion(color_trump);
//If I can play trump
    if (play_trump)
    {
        if ((color_asked!=color_trump)&&(i_master==0)&&(color==color_trump)) res += _play_3rd.value(0,0);//res=-10, we don't play  trump if the partner is mayor
        if ((color==color_trump)&&(color==color_asked))
        {
            if (is_master(color_trump,color)) res += _play_3rd.value(1,0); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (pseudo_height == 2) res += _play_3rd.value(2,0); //res=-55
                if (pseudo_height == 3) res += _play_3rd.value(3,0); //res=-20
                if (pseudo_height == 4) res += _play_3rd.value(4,0); //res=-10
                if (height==smallest(color_trump,color_trump)) res += _play_3rd.value(5,0); //res=30
            }
        }
        if ((color_asked!=color_trump)&&(i_master==1)&&(color==color_trump))
        {
            res += _play_3rd.value(6,0);//res=15
            if (height==smallest(color_trump,color)) res += _play_3rd.value(7,0); //res=10
            if (height==greatest(color_trump,color)) res += _play_3rd.value(8,0); //res=-30
            if (pseudo_height == 1) res += _play_3rd.value(9,0); //res=-20, we keep the jack for a trick where the color of the trump is played
            if (pseudo_height == 2) res += _play_3rd.value(10,0); //res=-20
            if (pseudo_height == 3) res += _play_3rd.value(11,0); //res=-10
        }
    }
//If my teammate is major
    if (i_master == 0)
    {
        if (color==color_trump)
        {
            if (protect_point(color_trump,color)&&(color_asked!=color_trump)&&(height = greatest(color_trump,color))) res += _play_3rd.value(12,0); //res=9
        }
        else
        {
            if (_nb_color_played[color_asked] == 0)
            {
                if (pseudo_height == 2) res += _play_3rd.value(13,0);//res=-8
                if (pseudo_height == 3) res += _play_3rd.value(14,0); //res=5
                else res += _play_3rd.value(15,0); //res=-3
            }
            else
            {
                if (pseudo_height <= 3) res += _play_3rd.value(16,0); //res=-5
                else res += _play_3rd.value(17,0); //res=3
            }
        }
        //add the case where the opponent don't cut (or has no more trumps) and my partner hase the master card in the colour
    }
//I have the 10 in the colour
    if ((color=color_asked)&&(got_ten(color_trump,color)))
    {
        if (pseudo_height == 2) //if it's the 10 (or the 9 of trump)
        {
            if ((pseudo_height ==_color_master[color])&&(!opponents_cut(color))) res += _play_3rd.value(18,0); //res=15
            else res += _play_3rd.value(19,0); //res=-25
            if (_nb_color_played[color]==1) res += _play_3rd.value(20,0); //res=3
            if (_nb_color_played[color]==0) res += _play_3rd.value(21,0);//res=-7
            if (_calls[modulo(_number+1,4)*4+color]) res += _play_3rd.value(22,0); //res=-8
            else res += _play_3rd.value(23,0); //res=6
        }
        else
        {
            if (!_calls[modulo(_number+1,4)*4+color]) res += _play_3rd.value(24,0); //res=-5
            else res += _play_3rd.value(25,0); //res=6
        }
    }
//If I am major in the color
    if (is_master(color_trump,color))
    {
        if (pcard->Nb_Point(color_trump) < 4) res += _play_3rd.value(26,0); //res=13
        else
        {
            if (opponents_cut(color))
            {
                switch (height)
                {
                    case 3 : res += _play_3rd.value(27,0); //res=-15
                        break;
                    case 7 : res += _play_3rd.value(28,0); //res=-25
                        break;
                    case 2 : res += _play_3rd.value(29,0); //res=-20
                        break;
                    default : break;
                }
            }
            else
            {
                if (_nb_color_played[color] == 0) res += _play_3rd.value(30,0); //res=7
                else res += _play_3rd.value(31,0); //res=-6
            }
        }
    }
//If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (is_master(color_trump,color))
        {
            res += _play_3rd.value(32,0); //res=10
            if (height==greatest(color_trump,color)) res += _play_3rd.value(33,0); //res=-25
            if (height==smallest(color_trump,color)) res += _play_3rd.value(34,0); //res=25
        }
        if (score_longe(color_trump,color) > 5) res += _play_3rd.value(35,0); //res=7
        if (score_longe(color_trump,color) > 8) res += _play_3rd.value(36,0); //res=10
    }
    return res;
}
//int AIPlayScores::playFourth(Carte* pcard, array<Carte*,4> trick,int color_trump,int player_take, int team_take,int i_master,bool pisse,bool play_trump,int number_trick,int color_asked,int height_master,int pseudo_height_master)
int AIPlayScores::playFourth(Cards*,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump)
{
    int res = 0;
    int color = pcard->GetColour();
    int height= pcard->GetHeight();
    //If we play trump
    if ((play_trump)&&(color == color_trump))
    {
        if (is_master(color_trump,color_trump)) //will we play a big or a small trump ?
        {
            if (protect_point(color_trump,color_trump))
            {
                if (greatest(color_trump,color_trump)) res += _play_4th.value(0,0);//res=-7
            }
            else
            {
                if (smallest(color_trump,color_trump)) res += _play_4th.value(1,0);//res=7
            }
        }
    }
// If my teammate is major
    if (i_master == 1)
    {
        if (color == color_trump) res += _play_4th.value(2,0); //res=-30
        else
        {
            if (height == 3) res += _play_4th.value(3,0); //res=8
            if (height == 7)
            {
                if (color == color_asked) res += _play_4th.value(4,0); //res=15
                else res += _play_4th.value(5,0);//res=-10
            }
            if ((_nb_resting_cards[color]+_nb_resting_card_color[color] == 8)&&(height == greatest(color_trump,color))) res += _play_4th.value(6,0); //res=35
        }
        if (protect_point(color_trump,color))
        {
            if (greatest(color_trump,color)) res += _play_4th.value(7,0); //res=9
        }
        else
        {
            if (smallest(color_trump,color)) res += _play_4th.value(8,0); //res=7
        }
    }
    else
    {
        switch (pcard->Nb_Point(color_trump))
        {
            case 0 : res += _play_4th.value(9,0); //res=13
                break;
            case 2 : res += _play_4th.value(10,0); //res=9
                break;
            case 3 : res += _play_4th.value(11,0); //res=7
                break;
            case 4 : res += _play_4th.value(12,0); //res=5
                break;
            case 10 : res += _play_4th.value(13,0); //res=-15
                break;
            case 11 : res += _play_4th.value(14,0); //res=-20
                break;
            case 14 : res += _play_4th.value(15,0); //res=-25
                break;
            case 20 : res += _play_4th.value(16,0); //res=100
                break;
            default : break;
        }
        if (greatest(color_trump,color)) res += _play_4th.value(17,0); //res=-7
    }
 //If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (is_master(color_trump,color))
        {
            res += _play_4th.value(18,0); //res=-10
            if (height==greatest(color_trump,color)) res += _play_4th.value(19,0); //res=-25
            if (height==smallest(color_trump,color)) res += _play_4th.value(20,0); //res=25
        }
        if (!is_master(color_trump,color))
        {
            if (i_master==1)
            {
               res += _play_4th.value(21,0); //res=5
                if (height==greatest(color_trump,color)) res += _play_4th.value(22,0); //res=15
                if (height==smallest(color_trump,color)) res += _play_4th.value(23,0); //res=-15
            }
            else
            {
                res += _play_4th.value(24,0); //res=-5
                if (height==greatest(color_trump,color)) res += _play_4th.value(25,0); //res=-15
                if (height==smallest(color_trump,color)) res += _play_4th.value(26,0); //res=15
            }
        }
        if (score_longe(color_trump,color) > 5) res += _play_4th.value(27,0); //res=7
        if (score_longe(color_trump,color) > 8) res += _play_4th.value(28,0); //res=10
    }
//If it is the penultimate trick, we try to keep our trump for the last trick
    if ((number_trick == 6)&&(color == color_trump)) res += _play_4th.value(29,0); //res=-8
//"Passe" of the ace, not always (risky)
    if ((color!=color_trump)&&(!ten_played(color_trump,color,trick,3))&&(height == 7)&&(_nb_color_played[color_asked]==0)&&(height_master!=3))
    {
        if (random(0,9)>=6) res += _play_4th.value(30,0); //res=-9
    }
    return res;
}
