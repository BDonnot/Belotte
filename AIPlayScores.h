#ifndef AIPLAYSCORES_H
#define AIPLAYSCORES_H

#include <list>
#include <array>
#include <type_traits>

#include "Definitions.h"
//#include "Cards.h"
//#include "AIGameMemoryImproved.h"
#include "Random.h"
#include "TrickStatus.h"
#include "Datas.h"
#include "TrickBasic_Memory.h"
#include "CardsGetMethod.h"
/** The AI player will play according to a score. These one are defined in some csv files**/

//TO DO: hash table instead of indices in the current file "datas.h"


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
												,typename std::list<TypeOfCard >& playbleCards
												,const std::list<TypeOfCard>& hand
												,Random& rand
												,const TrickStatus& trickStatus
												,const typename Memory<TypeOfCard>& playerMemory)
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

    //if (pcard == nullptr) return -10000; //TO DO : exception here

    //if I or my teammate took, I try to play trump
    if (team_taken == (_player.ToInt()%2) && playerMemory.NbColorPlayed(color_trump) == 0)
    {
        if(color == color_trump )
        {
            res += _play_1st.value(28,0); //res = 30
            if(playerMemory.Greatest(color_trump) == JACK) //if I have the jack
            {
                if(height_ == JACK) res += _play_1st.value(29,0); //res += 10
                else res += _play_1st.value(30,0); //res += -10
            }
        }
        else res += _play_1st.value(31,0); //res -= 30

    }
    if(playerMemory.OpponentsCut(color_trump)) //if the opponents have no more trump, I don't play trump
    {
        if(color == color_trump) res += _play_1st.value(32,0); //res -= 50
    }
    //trumps case
    if (play_trump && (color==color_trump))
    {
        res += _play_1st.value(0,0); //res = 30
        if (height==playerMemory.Master(color))
        {
            if (number_trick == 6) res += _play_1st.value(1,0); //res = -17
            else res += _play_1st.value(2,0); //res=100
        }
        if (height_==TEN)  res += _play_1st.value(3,0); //ten;res=7
        else if (height_==NINE)  res += _play_1st.value(4,0); //nine;res=-20
        else if (height_==ACE)  res += _play_1st.value(5,0); //Ace;res=-15
        else if (height_==JACK)  res += _play_1st.value(6,0); //Jack;res=100
    }
    if (!play_trump&&(color==color_trump)) res += _play_1st.value(7,0); //res=-15
    //Case of friend call : TO DO

    if (playerMemory.Call(teammate,color)) //add an additional condition : if the teammate does a call and we are not the strongest in this colour
    {
        res += _play_1st.value(8,0); //res=5
        if (height_==TEN) //10
        {
            res += _play_1st.value(9,0); //res=-10
            if (oponentCut) res += _play_1st.value(10,0); //res=-10
        }
        else if (height_==KING) res += _play_1st.value(11,0); //res=8
        else if (height_==QUEEN) //queen
        {
            if (oponentCut) res += _play_1st.value(12,0); //res=-10
            else res += _play_1st.value(13,0); //res=7
        }
        //if (height_==TEN) res += _play_1st.value(14,0); //res=4
        if (playerMemory.NbColorPlayed(color) == 0) res += _play_1st.value(15,0); //res=7
        if (oponentCut)
        {
            if (playerMemory.DoMyTeamTook()) res += _play_1st.value(16,0); //res=-5
            else res += _play_1st.value(17,0); //res=7
        }
    }

    //Case of the major card
    if (height == playerMemory.Master(color))
    {
        res += _play_1st.value(18,0); //res=12
        if (height_==TEN) //10
        {
            if (!oponentCut) res += _play_1st.value(19,0); //17
            else res += _play_1st.value(20,0); //res=-25
        }
        else if (height_==ACE) //ace
        {
            if (!oponentCut) res += _play_1st.value(21,0); //res=35
            else res += _play_1st.value(22,0); //res=-30
        }
        else if (height_==KING) //king
        {
            if (!oponentCut) res += _play_1st.value(23,0); //res=5
            else res += _play_1st.value(24,0); //res=-12
        }
        if (playerMemory.NbColorPlayed(color) == 0) //if the color has not been played
        {
            if (height == heightsOrder[1]) res += _play_1st.value(25,0); // res= 15
            else res += _play_1st.value(26,0); // res=-9
            //if (height == heightsOrder[2]) res += _play_1st.value(26,0); // res=-9
        }
        if ((number_trick == 6)&&(color == color_trump)) res += _play_1st.value(27,0); //res=-10
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
            if (playerMemory.AmIMaster(color)) res += _play_2nd.value(0,0); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (height_ == heightsOrder[1]) res += _play_2nd.value(1,0); //res=-55
                if (height_ == heightsOrder[2]) res += _play_2nd.value(2,0); //res=-30
                if (height_ == heightsOrder[3]) res += _play_2nd.value(3,0); //res=-20
                if (height == playerMemory.Smallest(color)) res += _play_2nd.value(4,0); //res=30
            }
        }
        else //we play a trump but the color asked is not the trump
        {
            if (height==playerMemory.Smallest(color)) res += _play_2nd.value(5,0); //res=10
            if (height==playerMemory.Greatest(color)) res += _play_2nd.value(6,0); //res=-30
            if (height_ == JACK) res += _play_2nd.value(32,0); //res=-20
            if (height_ == NINE) res += _play_2nd.value(7,0); //res=-20
            if (height_ == ACE) res += _play_2nd.value(8,0); //res=-10
        }
    }
//If I am the strongest in the color
    if ((height == playerMemory.Master(color))&&(color==color_asked))
    {
        res += _play_2nd.value(9,0); //res=15
        if (height_ == NINE) //10
        {
            res += _play_2nd.value(10,0); //res=10
            if (oponentCut) res += _play_2nd.value(11,0); //res=-17
        }
        if (height_ == QUEEN) res += _play_2nd.value(12,0); //res=15
        if (height_ == KING) res += _play_2nd.value(13,0); //res=5
        if (playerMemory.NbColorPlayed(color) ==0)
        {
            if (height_ == TEN) res += _play_2nd.value(14,0); //res=-20
            if (height_ == TEN) res += _play_2nd.value(14,0); //res=-7
        }
        if (oponentCut) res += _play_2nd.value(16,0); //res=-20
        if ((number_trick == 6)&&(color == color_trump)) res += _play_2nd.value(17,0); //res=-10
    }
    //I have the colour and I am not the strongest
    if ((color==color_asked)&&(height != playerMemory.Master(color)))
    {
        if (oponentCut) res += _play_2nd.value(18,0); //res=-10
        if (height_ == heightsOrder[1]) res += _play_2nd.value(19,0); //res=-30
        if (height_ == heightsOrder[2]) res += _play_2nd.value(20,0); //res=-10
        if (height_ == heightsOrder[3]) res += _play_2nd.value(21,0); //res=-5
    }
//the case of the 10 is taken into account in the case of "pisse"
//If we "pisse"(calls)
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (playerMemory.AmIMaster(color)) res += _play_2nd.value(22,0); //res=-15
        if (height == playerMemory.Greatest(color)) res += _play_2nd.value(23,0); //res=-15
        if (height == playerMemory.Smallest(color)) res += _play_2nd.value(24,0); //res=15
        //we are risk-adverse (more than in the position 3) and the aversion is inversely proportional to the pseudo_height of the card
        if (height_ == heightsOrder[0]) res += _play_2nd.value(25,0); //res=-100
        else if (height_ == heightsOrder[1]) res += _play_2nd.value(26,0); //res=-30
        else if (height_ == heightsOrder[2]) res += _play_2nd.value(27,0); //res=-10
        else if (height_ == heightsOrder[3]) res += _play_2nd.value(28,0); //res=-5
        else if (height_ == heightsOrder[4]) res += _play_2nd.value(29,0); //res=-2
        if (playerMemory.ScoreLonge(color) > 5) res += _play_2nd.value(30,0); //res=7
        if (playerMemory.ScoreLonge(color) > 8) res += _play_2nd.value(31,0); //res=10
    }
//if the cards value something and the that it is not the color played, we don't play the card
    if (color!=color_asked)
    {
        if (height_ == heightsOrder[0]) res += _play_2nd.value(33,0); //res = -25
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
        if ((color_asked!=color_trump)&&(i_master==First)&&(color==color_trump)) res += _play_3rd.value(0,0);//res=-10, we don't play  trump if the partner is mayor
        if ((color==color_trump)&&(color==color_asked))
        {
            if (playerMemory.AmIMaster(color)) res += _play_3rd.value(1,0); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (height_ == heightsOrder[1]) res += _play_3rd.value(2,0); //res=-55
                if (height_ == heightsOrder[2]) res += _play_3rd.value(3,0); //res=-20
                if (height_ == heightsOrder[3]) res += _play_3rd.value(4,0); //res=-10
                if (height ==playerMemory.Smallest(color)) res += _play_3rd.value(5,0); //res=30
            }
        }
        if ((color_asked!=color_trump)&&(i_master==SECOND)&&(color==color_trump))
        {
            res += _play_3rd.value(6,0);//res=15
            if (height==playerMemory.Smallest(color)) res += _play_3rd.value(7,0); //res=10
            if (height==playerMemory.Greatest(color)) res += _play_3rd.value(8,0); //res=-30
            if (height_ == heightsOrder[0]) res += _play_3rd.value(9,0); //res=-20, we keep the jack for a trick where the color of the trump is played
            if (height_ == heightsOrder[1]) res += _play_3rd.value(10,0); //res=-20
            if (height_ == heightsOrder[2]) res += _play_3rd.value(11,0); //res=-10
        }
    }
//If my teammate is major
    if (i_master == First)
    {
        if (color==color_trump)
        {
            //printf("je suis a 1\n");
            if ((color_asked!=color_trump)&& playerMemory.ProtectPoints(color) && (height == playerMemory.Greatest(color))) res += _play_3rd.value(12,0); //res=9
        }
        else
        {
            if (playerMemory.NbColorPlayed(color_asked) == 0)
            {
                if (height_ == heightsOrder[1]) res += _play_3rd.value(13,0);//res=-8
                if (height_ == heightsOrder[2]) res += _play_3rd.value(14,0); //res=5
                else res += _play_3rd.value(15,0); //res=-3
            }
            else
            {
                if (!(_wrapperCallMethod.callWin(pcard,Card_Height(heightsOrder[2])))) res += _play_3rd.value(16,0); //res=-5
                else res += _play_3rd.value(17,0); //res=3
            }
        }
        //add the case where the opponent don't cut (or has no more trumps) and my partner has the master card in the colour
    }
//I have the 10 in the colour
    if ((color==color_asked)&&(playerMemory.HaveTen(color)))
    {
        if (height_ == heightsOrder[1]) //if it's the 10 (or the 9 of trump)
        {
            if ((height == playerMemory.Master(color))&&(!oponentCut)) res += _play_3rd.value(18,0); //res=15
            else res += _play_3rd.value(19,0); //res=-25
            if (playerMemory.NbColorPlayed(color) == 1) res += _play_3rd.value(20,0); //res=3
            else if (playerMemory.NbColorPlayed(color) ==0) res += _play_3rd.value(21,0);//res=-7
            if (playerMemory.NextCall(color) || playerMemory.NextCut(color)) res += _play_3rd.value(22,0); //res=-8
            else res += _play_3rd.value(23,0); //res=6
        }
        else
        {
            if (playerMemory.NextCall(color)||playerMemory.NextCut(color) ) res += _play_3rd.value(24,0); //res=-5
            else res += _play_3rd.value(25,0); //res=6
        }
    }
//If I am major in the color
    if (playerMemory.AmIMaster(color))
    {
        if (_wrapperCallMethod.callValue(pcard) < 4) res += _play_3rd.value(26,0); //res=13
        else
        {
            if (oponentCut)
            {
                switch (height.Height())
                {
                    case TEN : res += _play_3rd.value(27,0); //res=-15
                        break;
                    case ACE : res += _play_3rd.value(28,0); //res=-25
                        break;
                    case NINE : res += _play_3rd.value(29,0); //res=-20
                        break;
                    default : break;
                }
            }
            else
            {
                if (playerMemory.NbColorPlayed(color) == 0) res += _play_3rd.value(30,0); //res=7
                else res += _play_3rd.value(31,0); //res=-6
            }
        }
    }
//If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (playerMemory.AmIMaster(color))
        {
            res += _play_3rd.value(32,0); //res=10
            if (height==playerMemory.Greatest(color)) res += _play_3rd.value(33,0); //res=-25
            if (height==playerMemory.Smallest(color)) res += _play_3rd.value(34,0); //res=25
        }
        if (playerMemory.ScoreLonge(color) > 5) res += _play_3rd.value(35,0); //res=7
        if (playerMemory.ScoreLonge(color) > 8) res += _play_3rd.value(36,0); //res=10
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
                if (height == playerMemory.Greatest(color)) res += _play_4th.value(0,0);//res=-7
            }
            else
            {
                if (height == playerMemory.Smallest(color)) res += _play_4th.value(1,0);//res=7
            }
        }
    }
// If my teammate is major
    if (trick.CurrentWinner() == SECOND)
    {
        if (color == color_trump) res += _play_4th.value(2,0); //res=-30
        else
        {
            if (height_ == TEN) res += _play_4th.value(3,0); //res=8
            if (height_ == ACE)
            {
                if (color == color_asked) res += _play_4th.value(4,0); //res=15
                else res += _play_4th.value(5,0);//res=-10
            }
            if ((playerMemory.AllRemainingCards(color))&&(height == playerMemory.Greatest(color))) res += _play_4th.value(6,0); //res=35
        }
        //printf("je suis a 3\n");
        if (playerMemory.ProtectPoints(color))
        {
            if (height == playerMemory.Greatest(color)) res += _play_4th.value(7,0); //res=9
        }
        else
        {
            if (height == playerMemory.Smallest(color)) res += _play_4th.value(8,0); //res=7
        }
    }
    else
    {
        switch (_wrapperCallMethod.callValue(pcard))
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
        if (height == playerMemory.Greatest(color)) res += _play_4th.value(17,0); //res=-7
    }
 //If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (playerMemory.AmIMaster(color))
        {
            res += _play_4th.value(18,0); //res=-10
            if (height==playerMemory.Greatest(color)) res += _play_4th.value(19,0); //res=-25
            if (height==playerMemory.Smallest(color)) res += _play_4th.value(20,0); //res=25
        }
        else
        {
            if (trick.CurrentWinner() == SECOND)
            {
               res += _play_4th.value(21,0); //res=5
                if (height==playerMemory.Greatest(color)) res += _play_4th.value(22,0); //res=15
                if (height==playerMemory.Smallest(color)) res += _play_4th.value(23,0); //res=-15
            }
            else
            {
                res += _play_4th.value(24,0); //res=-5
                if (height==playerMemory.Greatest(color)) res += _play_4th.value(25,0); //res=-15
                if (height==playerMemory.Smallest(color)) res += _play_4th.value(26,0); //res=15
            }
        }
        if (playerMemory.ScoreLonge(color) > 5) res += _play_4th.value(27,0); //res=7
        if (playerMemory.ScoreLonge(color) > 8) res += _play_4th.value(28,0); //res=10
    }
//If it is the penultimate trick, we try to keep our trump for the last trick
    if ((number_trick == 6)&&(color == color_trump)) res += _play_4th.value(29,0); //res=-8
//"Passe" of the ace, not always (risky)
    if ((color != color_trump)&&(height_ == ACE)&&(playerMemory.NbColorPlayed(color_asked)==0)&&(trick.HeightMaster().Height() != TEN)&&(!playerMemory.CardsFallen(color,Card_Height(TEN))))
    {
        if (rand.generate_number() >= 768) res += _play_4th.value(30,0); //res=-9 /3 chance out of 4 not to "passe"
    }
    return res;
}
#endif // AIPLAYSCORES_H
