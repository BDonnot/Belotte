#ifndef AIPLAYSCORES_H
#define AIPLAYSCORES_H

/**
** This is another wrapper for the method playing for AI players.
** It allow them to play according to scores.
** Those scores are stored in csv files, handle by
** the class Datas.
** This way of playing consist in assign a score for each
** playable cards, and play the cards with the higher score.
** The score are increased / decreased regarding some conditions.
** The value of the increasing/decreasing is stored in _play_*
**/


#include <list>
#include <array>
#include <type_traits>

#include "Definitions.h"
#include "Random.h"
#include "TrickStatus.h"
#include "Datas.h"
#include "TrickBasic_Memory.h"
#include "CardsGetMethod.h"

///Informatically :
///TO DO: hash table instead of indices in the current file "datas.h"

///Game Improvement :
///TO DO : ouvrir est souvent une bonne idée
///TO DO : jouer atout quand son partenaire a pris !!!
///TO DO : mettre ses AS quand on a la main et qu'on est en defense
///TO DO : ne pas mettre des points quand le plis n'est pas remporte
///eviter de jouer une couleur dans laquelle on a un 10
///TO DO : ne pas lancer son dernier atout
///TO DO : ne pas défausser ses AS : essayer de garder ses cartes maitresses !

template<class TypeOfCard, template<class> class Memory >
class AIPlayScores
{
    protected:
        Basic_Game_Info _infos;
        const Uint _type;
        Player_ID _player;

        Datas _play_1st;
        Datas _play_2nd;
        Datas _play_3rd;
        Datas _play_4th;

        WrapperMethod<TypeOfCard,std::is_pointer<TypeOfCard>::value> _wrapperCallMethod;
    public:
        AIPlayScores(const Player_ID& player):
            _type(1),
            _player(player),
            _play_1st("datas/_a_eval_score_play_1st.csv",_type),
            _play_2nd("datas/_a_eval_score_play_2nd.csv",_type),
            _play_3rd("datas/_a_eval_score_play_3rd.csv",_type),
            _play_4th("datas/_a_eval_score_play_4th.csv",_type)
            {
            }

        virtual ~AIPlayScores(){}
		TypeOfCard Play(const TrickBasic_Memory& trick
												,typename std::list<TypeOfCard > & plyableCards
												,const std::list< TypeOfCard >& hand
												,Random& rand
												,const TrickStatus& trickStatus
												,const Memory<TypeOfCard> & playerMemory);

        void SetNumber(const Player_ID& number)
        {
            _player = number;
        }
    protected:
		int playFirst(const TypeOfCard& pcard, const TrickBasic_Memory& trick, const TrickStatus& trickStatus, Random& rand, bool pisse, bool play_trump, const Memory<TypeOfCard>& playerMemory);
		int playSecond(const TypeOfCard& pcard, const TrickBasic_Memory& trick, const TrickStatus& trickStatus, Random& rand, bool pisse, bool play_trump, const Memory<TypeOfCard>& playerMemory);
		int playThird(const TypeOfCard& pcard, const TrickBasic_Memory& trick, const TrickStatus& trickStatus, Random& rand, bool pisse, bool play_trump, const Memory<TypeOfCard>& playerMemory);
		int playFourth(const TypeOfCard& pcard, const TrickBasic_Memory& trick, const TrickStatus& trickStatus, Random& rand, bool pisse, bool play_trump, const Memory<TypeOfCard>& playerMemory);
    private:
};


template<class TypeOfCard, template<class> class Memory >
TypeOfCard AIPlayScores<TypeOfCard,Memory>::Play(const TrickBasic_Memory& trick
												,std::list<TypeOfCard >& playbleCards
												,const std::list<TypeOfCard>& hand
												,Random& rand
												,const TrickStatus& trickStatus
												,const Memory<TypeOfCard>& playerMemory)
{
    auto res = playbleCards.front();
    int MaxScore = -10000,currentScore;
    const Card_Color& color_asked = trick.ColorAsked();
    const Card_Color& color_trump = trickStatus.TrumpColor();
    bool pisse = false; //I can play another colour than the one asked, or trump
    bool play_trump = false; //I can play at least one trump
	for (auto pcard : playbleCards)
    {
        if (( _wrapperCallMethod.callGetColour(pcard) != color_asked)&&( _wrapperCallMethod.callGetColour(pcard) != color_trump)) pisse = true;
        if ( _wrapperCallMethod.callGetColour(pcard) == color_trump) play_trump = true;
    }
	for (auto pcard : playbleCards)
    {
        switch(trickStatus.Position().Position()) //TO DO : find a more elegant way
        {
        case FIRST :
			currentScore = playFirst(pcard, trick, trickStatus, rand, pisse, play_trump, playerMemory);
            break;
        case SECOND :
			currentScore = playSecond(pcard, trick, trickStatus, rand, pisse, play_trump, playerMemory);
            break;
        case THIRD :
			currentScore = playThird(pcard, trick, trickStatus, rand, pisse, play_trump, playerMemory);
            break;
        case FOURTH :
			currentScore = playFourth(pcard, trick, trickStatus, rand, pisse, play_trump, playerMemory);
            break;
        default : //TO DO : exception here
            currentScore = -10000;
            break;
        }

        if(currentScore > MaxScore)
        {
			res = pcard;
            MaxScore = currentScore;
        }
    }
    return res;
}

//int AIPlayScores::playFirst(Carte* pcard, int color_trump,int player_take, int team_taken,bool play_trump,int pseudo_height_trump_max, int number_trick)
template<class TypeOfCard, template<class> class Memory >
int AIPlayScores<TypeOfCard, Memory>::playFirst(const TypeOfCard& pcard,
                                    const TrickBasic_Memory& trick,
                                    const TrickStatus& trickStatus,
                                    Random& rand,
                                    bool pisse,
                                    bool play_trump,
									const Memory<TypeOfCard>& playerMemory)
{
    int res=0;
    const Card_Color& color = _wrapperCallMethod.callGetColour(pcard);
    const Card_Height& height = _wrapperCallMethod.callGetHeight(pcard);
    const Card_Color& color_trump = trickStatus.TrumpColor();

    CARDS_HEIGHT height_ = _wrapperCallMethod.callGetHeight(pcard).Height();

    Player_ID player_take = _infos.Taker();
    int team_taken = (player_take.ToInt()%2);
    bool oponentCut = playerMemory.OpponentsCut(color);
    Uint number_trick = trick.TrickNumber();
    CARDS_HEIGHT heightsOrder[8] =  {ACE,TEN,KING,QUEEN,JACK,NINE,EIGHT,SEVEN};
    if(color == color_trump)
    {
        heightsOrder[0] = JACK;
        heightsOrder[1] = NINE;
        heightsOrder[2] = ACE;
        heightsOrder[3] = TEN;
        heightsOrder[4] = KING;
        heightsOrder[5] = QUEEN;
    }
    Player_ID teammate = _player.Teammate();

    //if I or my teammate took, I try to play trump
    if (team_taken == (_player.ToInt()%2) && playerMemory.NbColorPlayed(color_trump) == 0)
    {
        if(color == color_trump )
        {
            res += _play_1st.Value(28); //res = 30
            if(playerMemory.Greatest(color_trump) == JACK) //if I have the jack
            {
                if(height_ == JACK) res += _play_1st.Value(29); //res += 10
                else res += _play_1st.Value(30); //res += -10
            }
        }
        else res += _play_1st.Value(31); //res -= 30

    }
    if(playerMemory.OpponentsCut(color_trump)) //if the opponents have no more trump, I don't play trump
    {
        if(color == color_trump) res += _play_1st.Value(32); //res -= 50
    }
    //trumps case
    if (play_trump && (color==color_trump))
    {
        res += _play_1st.Value(0); //res = 30
        if (height==playerMemory.Master(color))
        {
            if (number_trick == 6) res += _play_1st.Value(1); //res = -17
            else res += _play_1st.Value(2); //res=100
        }
        if (height_==TEN)  res += _play_1st.Value(3); //ten;res=7
        else if (height_==NINE)  res += _play_1st.Value(4); //nine;res=-20
        else if (height_==ACE)  res += _play_1st.Value(5); //Ace;res=-15
        else if (height_==JACK)  res += _play_1st.Value(6); //Jack;res=100
    }
    if (!play_trump&&(color==color_trump)) res += _play_1st.Value(7); //res=-15
    //Case of friend call : TO DO

    if (playerMemory.Call(teammate,color)) //add an additional condition : if the teammate does a call and we are not the strongest in this colour
    {
        res += _play_1st.Value(8); //res=5
        if (height_==TEN) //10
        {
            res += _play_1st.Value(9); //res=-10
            if (oponentCut) res += _play_1st.Value(10); //res=-10
        }
        else if (height_==KING) res += _play_1st.Value(11); //res=8
        else if (height_==QUEEN) //queen
        {
            if (oponentCut) res += _play_1st.Value(12); //res=-10
            else res += _play_1st.Value(13); //res=7
        }
        //if (height_==TEN) res += _play_1st.value(14,0); //res=4
        if (playerMemory.NbColorPlayed(color) == 0) res += _play_1st.Value(15); //res=7
        if (oponentCut)
        {
            if (playerMemory.DoMyTeamTook()) res += _play_1st.Value(16); //res=-5
            else res += _play_1st.Value(17); //res=7
        }
    }

    //Case of the major card
    if (height == playerMemory.Master(color))
    {
        res += _play_1st.Value(18); //res=12
        if (height_==TEN) //10
        {
            if (!oponentCut) res += _play_1st.Value(19); //17
            else res += _play_1st.Value(20); //res=-25
        }
        else if (height_==ACE) //ace
        {
            if (!oponentCut) res += _play_1st.Value(21); //res=35
            else res += _play_1st.Value(22); //res=-30
        }
        else if (height_==KING) //king
        {
            if (!oponentCut) res += _play_1st.Value(23); //res=5
            else res += _play_1st.Value(24); //res=-12
        }
        if (playerMemory.NbColorPlayed(color) == 0) //if the color has not been played
        {
            if (height == heightsOrder[1]) res += _play_1st.Value(25); // res= 15
            else res += _play_1st.Value(26); // res=-9
            //if (height == heightsOrder[2]) res += _play_1st.value(26,0); // res=-9
        }
        if ((number_trick == 6)&&(color == color_trump)) res += _play_1st.Value(27); //res=-10
    }
    //We try to not play in the "longe" of our opponents : TO DO
    //if (_calls[((_number+1)%4)*4+color]||_calls[modulo(_number+1,4)*4+color]) res += _play_1st.value(28,0); //res=-9
    return res;
}

template<class TypeOfCard, template<class> class Memory >
int AIPlayScores<TypeOfCard, Memory>::playSecond(const TypeOfCard& pcard,
                                     const TrickBasic_Memory& trick,
                                     const TrickStatus& trickStatus,
                                     Random& rand,
                                     bool pisse,
                                     bool play_trump,
									 const Memory<TypeOfCard>& playerMemory)
{
    int res = 0;
    const Card_Color& color = _wrapperCallMethod.callGetColour(pcard);
    const Card_Height& height = _wrapperCallMethod.callGetHeight(pcard);
    const Card_Color& color_trump = trickStatus.TrumpColor();
    const Card_Color& color_asked = trick.ColorAsked();

    CARDS_HEIGHT height_ = _wrapperCallMethod.callGetHeight(pcard).Height();

    bool oponentCut = playerMemory.OpponentsCut(color);
    Uint number_trick = trick.TrickNumber();
    CARDS_HEIGHT heightsOrder[8] =  {ACE,TEN,KING,QUEEN,JACK,NINE,EIGHT,SEVEN};
    if(color == color_trump)
    {
        heightsOrder[0] = JACK;
        heightsOrder[1] = NINE;
        heightsOrder[2] = ACE;
        heightsOrder[3] = TEN;
        heightsOrder[4] = KING;
        heightsOrder[5] = QUEEN;
    }
//If I have to play a trump
    if ((play_trump)&&(color==color_trump))
    {
        if (color==color_asked) //if the colour asked is the trump color
        {
            if (playerMemory.AmIMaster(color)) res += _play_2nd.Value(0); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (height_ == heightsOrder[1]) res += _play_2nd.Value(1); //res=-55
                if (height_ == heightsOrder[2]) res += _play_2nd.Value(2); //res=-30
                if (height_ == heightsOrder[3]) res += _play_2nd.Value(3); //res=-20
                if (height == playerMemory.Smallest(color)) res += _play_2nd.Value(4); //res=30
            }
        }
        else //we play a trump but the color asked is not the trump
        {
            if (height==playerMemory.Smallest(color)) res += _play_2nd.Value(5); //res=10
            if (height==playerMemory.Greatest(color)) res += _play_2nd.Value(6); //res=-30
            if (height_ == JACK) res += _play_2nd.Value(32); //res=-20
            if (height_ == NINE) res += _play_2nd.Value(7); //res=-20
            if (height_ == ACE) res += _play_2nd.Value(8); //res=-10
        }
    }
//If I am the strongest in the color
    if ((height == playerMemory.Master(color))&&(color==color_asked))
    {
        res += _play_2nd.Value(9); //res=15
        if (height_ == NINE) //10
        {
            res += _play_2nd.Value(10); //res=10
            if (oponentCut) res += _play_2nd.Value(11); //res=-17
        }
        if (height_ == QUEEN) res += _play_2nd.Value(12); //res=15
        if (height_ == KING) res += _play_2nd.Value(13); //res=5
        if (playerMemory.NbColorPlayed(color) ==0)
        {
            if (height_ == TEN) res += _play_2nd.Value(14); //res=-20
            if (height_ == TEN) res += _play_2nd.Value(14); //res=-7
        }
        if (oponentCut) res += _play_2nd.Value(16); //res=-20
        if ((number_trick == 6)&&(color == color_trump)) res += _play_2nd.Value(17); //res=-10
    }
    //I have the colour and I am not the strongest
    if ((color==color_asked)&&(height != playerMemory.Master(color)))
    {
        if (oponentCut) res += _play_2nd.Value(18); //res=-10
        if (height_ == heightsOrder[1]) res += _play_2nd.Value(19); //res=-30
        if (height_ == heightsOrder[2]) res += _play_2nd.Value(20); //res=-10
        if (height_ == heightsOrder[3]) res += _play_2nd.Value(21); //res=-5
    }
//the case of the 10 is taken into account in the case of "pisse"
//If we "pisse"(calls)
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (playerMemory.AmIMaster(color)) res += _play_2nd.Value(22); //res=-15
        if (height == playerMemory.Greatest(color)) res += _play_2nd.Value(23); //res=-15
        if (height == playerMemory.Smallest(color)) res += _play_2nd.Value(24); //res=15
        //we are risk-adverse (more than in the position 3) and the aversion is inversely proportional to the pseudo_height of the card
        if (height_ == heightsOrder[0]) res += _play_2nd.Value(25); //res=-100
        else if (height_ == heightsOrder[1]) res += _play_2nd.Value(26); //res=-30
        else if (height_ == heightsOrder[2]) res += _play_2nd.Value(27); //res=-10
        else if (height_ == heightsOrder[3]) res += _play_2nd.Value(28); //res=-5
        else if (height_ == heightsOrder[4]) res += _play_2nd.Value(29); //res=-2
        if (playerMemory.ScoreLonge(color) > 5) res += _play_2nd.Value(30); //res=7
        if (playerMemory.ScoreLonge(color) > 8) res += _play_2nd.Value(31); //res=10
    }
//if the cards Value something and the that it is not the color played, we don't play the card
    if (color!=color_asked)
    {
        if (height_ == heightsOrder[0]) res += _play_2nd.Value(33); //res = -25
    }
    return res;
}

template<class TypeOfCard, template<class> class Memory >
int AIPlayScores<TypeOfCard, Memory>::playThird(const TypeOfCard& pcard,
                                    const TrickBasic_Memory& trick,
                                    const TrickStatus& trickStatus,
                                    Random& rand,
                                    bool pisse,
                                    bool play_trump,
									const Memory<TypeOfCard>& playerMemory)
{
    int res = 0;
    const Card_Color& color = _wrapperCallMethod.callGetColour(pcard);
    const Card_Height& height = _wrapperCallMethod.callGetHeight(pcard);
    const Card_Color& color_trump = trickStatus.TrumpColor();
    const Card_Color& color_asked = trick.ColorAsked();

    CARDS_HEIGHT height_ = _wrapperCallMethod.callGetHeight(pcard).Height();

    bool oponentCut = playerMemory.OpponentsCut(color);

    CARDS_HEIGHT heightsOrder[8] =  {ACE,TEN,KING,QUEEN,JACK,NINE,EIGHT,SEVEN};
    if(color == color_trump)
    {
        heightsOrder[0] = JACK;
        heightsOrder[1] = NINE;
        heightsOrder[2] = ACE;
        heightsOrder[3] = TEN;
        heightsOrder[4] = KING;
        heightsOrder[5] = QUEEN;
    }
    Position_Trick i_master = trick.CurrentWinner();
    Position_Trick First(FIRST);
    //Player_ID teammate = _player.Teammate();
//If I can play trump
    if (play_trump)
    {
        if ((color_asked!=color_trump)&&(i_master==First)&&(color==color_trump)) res += _play_3rd.Value(0);//res=-10, we don't play  trump if the partner is mayor
        if ((color==color_trump)&&(color==color_asked))
        {
            if (playerMemory.AmIMaster(color)) res += _play_3rd.Value(1); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (height_ == heightsOrder[1]) res += _play_3rd.Value(2); //res=-55
                if (height_ == heightsOrder[2]) res += _play_3rd.Value(3); //res=-20
                if (height_ == heightsOrder[3]) res += _play_3rd.Value(4); //res=-10
                if (height ==playerMemory.Smallest(color)) res += _play_3rd.Value(5); //res=30
            }
        }
        if ((color_asked!=color_trump)&&(i_master==SECOND)&&(color==color_trump))
        {
            res += _play_3rd.Value(6);//res=15
            if (height==playerMemory.Smallest(color)) res += _play_3rd.Value(7); //res=10
            if (height==playerMemory.Greatest(color)) res += _play_3rd.Value(8); //res=-30
            if (height_ == heightsOrder[0]) res += _play_3rd.Value(9); //res=-20, we keep the jack for a trick where the color of the trump is played
            if (height_ == heightsOrder[1]) res += _play_3rd.Value(10); //res=-20
            if (height_ == heightsOrder[2]) res += _play_3rd.Value(11); //res=-10
        }
    }
//If my teammate is major
    if (i_master == First)
    {
        if (color==color_trump)
        {
            //printf("je suis a 1\n");
            if ((color_asked!=color_trump)&& playerMemory.ProtectPoints(color) && (height == playerMemory.Greatest(color))) res += _play_3rd.Value(12); //res=9
        }
        else
        {
            if (playerMemory.NbColorPlayed(color_asked) == 0)
            {
                if (height_ == heightsOrder[1]) res += _play_3rd.Value(13);//res=-8
                if (height_ == heightsOrder[2]) res += _play_3rd.Value(14); //res=5
                else res += _play_3rd.Value(15); //res=-3
            }
            else
            {
                if (!(_wrapperCallMethod.callWin(pcard,Card_Height(heightsOrder[2])))) res += _play_3rd.Value(16); //res=-5
                else res += _play_3rd.Value(17); //res=3
            }
        }
        //add the case where the opponent don't cut (or has no more trumps) and my partner has the master card in the colour
    }
//I have the 10 in the colour
    if ((color==color_asked)&&(playerMemory.HaveTen(color)))
    {
        if (height_ == heightsOrder[1]) //if it's the 10 (or the 9 of trump)
        {
            if ((height == playerMemory.Master(color))&&(!oponentCut)) res += _play_3rd.Value(18); //res=15
            else res += _play_3rd.Value(19); //res=-25
            if (playerMemory.NbColorPlayed(color) == 1) res += _play_3rd.Value(20); //res=3
            else if (playerMemory.NbColorPlayed(color) ==0) res += _play_3rd.Value(21);//res=-7
            if (playerMemory.NextCall(color) || playerMemory.NextCut(color)) res += _play_3rd.Value(22); //res=-8
            else res += _play_3rd.Value(23); //res=6
        }
        else
        {
            if (playerMemory.NextCall(color)||playerMemory.NextCut(color) ) res += _play_3rd.Value(24); //res=-5
            else res += _play_3rd.Value(25); //res=6
        }
    }
//If I am major in the color
    if (playerMemory.AmIMaster(color))
    {
        if (_wrapperCallMethod.callValue(pcard) < 4) res += _play_3rd.Value(26); //res=13
        else
        {
            if (oponentCut)
            {
                switch (height.Height())
                {
                    case TEN : res += _play_3rd.Value(27); //res=-15
                        break;
                    case ACE : res += _play_3rd.Value(28); //res=-25
                        break;
                    case NINE : res += _play_3rd.Value(29); //res=-20
                        break;
                    default : break;
                }
            }
            else
            {
                if (playerMemory.NbColorPlayed(color) == 0) res += _play_3rd.Value(30); //res=7
                else res += _play_3rd.Value(31); //res=-6
            }
        }
    }
//If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (playerMemory.AmIMaster(color))
        {
            res += _play_3rd.Value(32); //res=10
            if (height==playerMemory.Greatest(color)) res += _play_3rd.Value(33); //res=-25
            if (height==playerMemory.Smallest(color)) res += _play_3rd.Value(34); //res=25
        }
        if (playerMemory.ScoreLonge(color) > 5) res += _play_3rd.Value(35); //res=7
        if (playerMemory.ScoreLonge(color) > 8) res += _play_3rd.Value(36); //res=10
    }
    return res;
}

template<class TypeOfCard, template<class> class Memory >
int AIPlayScores<TypeOfCard, Memory>::playFourth(const TypeOfCard& pcard,
                                     const TrickBasic_Memory& trick,
                                     const TrickStatus& trickStatus,
                                     Random& rand,
                                     bool pisse,
                                     bool play_trump,
									 const Memory<TypeOfCard>& playerMemory)
{
    int res = 0;
    const Card_Color& color = _wrapperCallMethod.callGetColour(pcard);
    const Card_Height& height = _wrapperCallMethod.callGetHeight(pcard);
    const Card_Color& color_trump = trickStatus.TrumpColor();
    const Card_Color& color_asked = trick.ColorAsked();

    CARDS_HEIGHT height_ = _wrapperCallMethod.callGetHeight(pcard).Height();
    //TO DO vraimet bizarre que ca de ne depende pas de qui gagne le pli ...

    Uint number_trick = trick.TrickNumber();

    //If we play trump
    if ((play_trump)&&(color == color_trump))
    {
        if (playerMemory.AmIMaster(color)) //will we play a big or a small trump ?
        {
            //printf("je suis a 2\n");
            if (playerMemory.ProtectPoints(color))
            {
                if (height == playerMemory.Greatest(color)) res += _play_4th.Value(0);//res=-7
            }
            else
            {
                if (height == playerMemory.Smallest(color)) res += _play_4th.Value(1);//res=7
            }
        }
    }
// If my teammate is major
    if (trick.CurrentWinner() == SECOND)
    {
        if (color == color_trump) res += _play_4th.Value(2); //res=-30
        else
        {
            if (height_ == TEN) res += _play_4th.Value(3); //res=8
            if (height_ == ACE)
            {
                if (color == color_asked) res += _play_4th.Value(4); //res=15
                else res += _play_4th.Value(5);//res=-10
            }
            if ((playerMemory.AllRemainingCards(color))&&(height == playerMemory.Greatest(color))) res += _play_4th.Value(6); //res=35
        }
        //printf("je suis a 3\n");
        if (playerMemory.ProtectPoints(color))
        {
            if (height == playerMemory.Greatest(color)) res += _play_4th.Value(7); //res=9
        }
        else
        {
            if (height == playerMemory.Smallest(color)) res += _play_4th.Value(8); //res=7
        }
    }
    else
    {
        switch (_wrapperCallMethod.callValue(pcard))
        {
            case 0 : res += _play_4th.Value(9); //res=13
                break;
            case 2 : res += _play_4th.Value(10); //res=9
                break;
            case 3 : res += _play_4th.Value(11); //res=7
                break;
            case 4 : res += _play_4th.Value(12); //res=5
                break;
            case 10 : res += _play_4th.Value(13); //res=-15
                break;
            case 11 : res += _play_4th.Value(14); //res=-20
                break;
            case 14 : res += _play_4th.Value(15); //res=-25
                break;
            case 20 : res += _play_4th.Value(16); //res=100
                break;
            default : break;
        }
        if (height == playerMemory.Greatest(color)) res += _play_4th.Value(17); //res=-7
    }
 //If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (playerMemory.AmIMaster(color))
        {
            res += _play_4th.Value(18); //res=-10
            if (height==playerMemory.Greatest(color)) res += _play_4th.Value(19); //res=-25
            if (height==playerMemory.Smallest(color)) res += _play_4th.Value(20); //res=25
        }
        else
        {
            if (trick.CurrentWinner() == SECOND)
            {
               res += _play_4th.Value(21); //res=5
                if (height==playerMemory.Greatest(color)) res += _play_4th.Value(22); //res=15
                if (height==playerMemory.Smallest(color)) res += _play_4th.Value(23); //res=-15
            }
            else
            {
                res += _play_4th.Value(24); //res=-5
                if (height==playerMemory.Greatest(color)) res += _play_4th.Value(25); //res=-15
                if (height==playerMemory.Smallest(color)) res += _play_4th.Value(26); //res=15
            }
        }
        if (playerMemory.ScoreLonge(color) > 5) res += _play_4th.Value(27); //res=7
        if (playerMemory.ScoreLonge(color) > 8) res += _play_4th.Value(28); //res=10
    }
//If it is the penultimate trick, we try to keep our trump for the last trick
    if ((number_trick == 6)&&(color == color_trump)) res += _play_4th.Value(29); //res=-8
//"Passe" of the ace, not always (risky)
    if ((color != color_trump)&&(height_ == ACE)&&(playerMemory.NbColorPlayed(color_asked)==0)&&(trick.HeightMaster().Height() != TEN)&&(!playerMemory.CardsFallen(color,Card_Height(TEN))))
    {
        if (rand.generate_number() >= 768) res += _play_4th.Value(30); //res=-9 /3 chance out of 4 not to "passe"
    }
    return res;
}
#endif // AIPLAYSCORES_H
