#ifndef BASIC_GAME_INFO_H
#define BASIC_GAME_INFO_H

#include <array>
//#include<string>
//#include<ostream>
//#include<sstream>

#include "Definitions.h"

class Basic_Game_Info
{
    protected:
        static unsigned long long _clock;
        static POSITION_PLAYER _giver;

        static POSITION_PLAYER _taker;
        //static unsigned int _firstToBid; //number of the first player who must bid.
        static CARDS_COLOR _trump_color;
        static unsigned int _max_bid;
        static CARDS_COLOR _color_proposed;
        //static unsigned int _biddingCount;

        static unsigned int _trick_number;
        static POSITION_TRICK _index_strongest_card;
        static POSITION_PLAYER _position_first_player; //the first player who will played the trick
        static unsigned int _number_card_played;

        static std::array<unsigned int,2> _final_scores;
        static std::array<unsigned int,2> _running_scores;
        static std::array<unsigned int,2> _tricks_win; //number of trick won
        static std::array<CARDS_COLOR,4> _int_to_color;
        static std::array<CARDS_HEIGHT,8> _int_to_height;

        static Uint _windowsWidth;
        static Uint _windowsHeight;
    public:
        Basic_Game_Info();
        virtual ~Basic_Game_Info();

        const unsigned long long& Time() const;
        void AddClock(unsigned long long diffTime);
        //void SetClock(unsigned long long clock);

        inline const POSITION_PLAYER& Taker() const;
        inline const POSITION_PLAYER& Giver() const;
        inline const POSITION_TRICK& StrongestCard() const;
        inline const unsigned int& NumberCardsPlayed() const;
        inline const POSITION_PLAYER& FirstPlayer() const;
        inline const unsigned int& TrickNumber() const;

        inline const CARDS_COLOR& TrumpColor() const;
        inline const CARDS_COLOR& ColorProposed() const;
        inline const unsigned int& BiddingRound() const;
        //void ResetBiddingCount();
        inline const std::array<unsigned int,2>& RunningScores() const;
        inline const std::array<unsigned int,2>& FinalScores() const;

        inline const unsigned int& MaxBid() const;

        static const CARDS_COLOR ConvertIntToColor(int i);
        static const CARDS_HEIGHT ConvertIntToHeight(int i);

        inline static const Uint& WindowsWidth();
        inline static const Uint& WindowsHeight();

        //void NextBiddingTurn();
        void NextPlayerBid(){_position_first_player = Next(_position_first_player);}


        void SetTaker(const POSITION_PLAYER taker);
        void SetTrumpColor(const CARDS_COLOR trump_color);

        void SetScores(POSITION_TRICK trick_winner
                       ,unsigned int value_card_played_by_player_0 //update the score during the game
                       ,unsigned int value_card_played_by_player_1
                       ,unsigned int value_card_played_by_player_2
                       ,unsigned int value_card_played_by_player_3);
        void SetScores(POSITION_TRICK trick_winner
                       ,const std::array<unsigned int,4>& value_cards_played);
        void SetNumberCardsPlayed();
        void SetHigherCard(POSITION_TRICK winner); //update _index_strongest_card in case the last player has played a stronger card
        //void SetCurrentPlayer();

        unsigned int CalculateFinalScores(); //compute who wins (returns the equip of the winner), determined each score, reset the other parameters

        void SetMaxBid(unsigned int max_bid);
        void SetColorProposed(CARDS_COLOR color);

        POSITION_PLAYER Next(POSITION_PLAYER posPlayer) const;
        inline Uint PosPlayerToInt(POSITION_PLAYER posPlayer) const;
        inline POSITION_PLAYER IntToPosPlayer(Uint num) const;
        inline Uint PosTrickToInt(POSITION_TRICK posTrick) const;
        inline POSITION_TRICK IntToPosTrick(Uint pos) const;
        inline POSITION_PLAYER FirstToPlay(POSITION_TRICK posTrick, POSITION_PLAYER playerConcerned) const;

        inline CARDS_HEIGHT IntToHeight(Uint num) const;
        inline Uint HeightToInt(CARDS_HEIGHT height) const;
        inline CARDS_COLOR IntToColor(Uint color) const;
        inline Uint ColorToInt(CARDS_COLOR color) const;

        void SetGiver(); //the giver is always the person at the left of the current giver
//        POSITION_TRICK Next(POSITION_TRICK posTrick) const;
    protected:
        void setTrickNumber(); //the SetTrickNumber can increase of only 1 unit each time
        Uint posTrickToInt(POSITION_TRICK posTrick);
        POSITION_PLAYER posTrickToPlayer(POSITION_PLAYER firstToPlay,POSITION_TRICK posTrick);
    private:
        //Basic_Game_Info(const Basic_Game_Info& other);
        //Basic_Game_Info& operator=(const Basic_Game_Info& other);
};

inline const POSITION_PLAYER& Basic_Game_Info::Taker() const
{
    return _taker;
}
inline const POSITION_PLAYER& Basic_Game_Info::Giver() const
{
    return _giver;
}
inline const CARDS_COLOR& Basic_Game_Info::TrumpColor() const
{
    return _trump_color;
}
inline const unsigned int& Basic_Game_Info::TrickNumber() const
{
    return _trick_number;
}
inline const std::array<unsigned int,2>& Basic_Game_Info::RunningScores() const
{
    return _running_scores;
}
inline const std::array<unsigned int,2>& Basic_Game_Info::FinalScores() const
{
    return _final_scores;
}
inline const POSITION_TRICK& Basic_Game_Info::StrongestCard() const
{
    return _index_strongest_card;
}
inline const unsigned int& Basic_Game_Info::NumberCardsPlayed() const
{
    return _number_card_played;
}
inline const POSITION_PLAYER& Basic_Game_Info::FirstPlayer() const
{
    return _position_first_player;
}
inline const unsigned int& Basic_Game_Info::MaxBid() const
{
    return _max_bid;
}
inline const CARDS_COLOR& Basic_Game_Info::ColorProposed() const
{
    return _color_proposed;
}
inline const unsigned long long& Basic_Game_Info::Time() const
{
    return _clock;
}
/*
inline const unsigned int& Basic_Game_Info::BiddingRound() const
{
    return _biddingCount;
}
*/
inline const Uint& Basic_Game_Info::WindowsWidth()
{
    return _windowsWidth;
}
inline const Uint& Basic_Game_Info::WindowsHeight()
{
    return _windowsHeight;
}

inline POSITION_PLAYER Basic_Game_Info::IntToPosPlayer(Uint num) const
{
    switch(num)
    {
        case 0: return PLAYER0;
        case 1: return PLAYER1;
        case 2: return PLAYER2;
        case 3: return PLAYER3;
        default: return GHOST; // TO DO exception here
    }
}

inline CARDS_HEIGHT Basic_Game_Info::IntToHeight(Uint num) const
{
    switch(num)
    {
        case 0 : return SEVEN;
        case 1 : return EIGHT;
        case 2 : return NINE;
        case 3 : return TEN;
        case 4 : return JACK;
        case 5 : return QUEEN;
        case 6 : return KING;
        case 7 : return ACE;
        default : return UNINTIALIZED; //TO DO exception here
    }
}

inline Uint Basic_Game_Info::HeightToInt(CARDS_HEIGHT height) const
{
    switch(height)
    {
        case SEVEN : return 0;
        case EIGHT : return 1;
        case NINE : return 2;
        case TEN : return 3;
        case JACK : return 4;
        case QUEEN : return 5;
        case KING : return 6;
        case ACE : return 7;
        default : return 42; //TO DO exception here
    }
}

inline CARDS_COLOR Basic_Game_Info::Basic_Game_Info::IntToColor(Uint color) const
{
    switch(color)
    {
        case 0: return DIAMOND;
        case 1: return HEART;
        case 2: return CLUB;
        case 3: return SPADE;
        default : return NOT_CHOSEN; //TO DO exception here
    }
}

inline Uint Basic_Game_Info::ColorToInt(CARDS_COLOR color) const
{
    switch(color)
    {
        case DIAMOND : return 0;
        case HEART : return 1;
        case CLUB : return 2;
        case SPADE : return 3;
        default : return 42;
    }
}

inline Uint Basic_Game_Info::PosPlayerToInt(POSITION_PLAYER posPlayer) const
{
    switch(posPlayer)
    {
    case PLAYER0 :
        return 0;
    case PLAYER1 :
        return 1;
    case PLAYER2:
        return 2;
    case PLAYER3 :
        return 3;
    default : // TO DO exception here
        return 4;
    }
}


inline Uint Basic_Game_Info::PosTrickToInt(POSITION_TRICK posTrick) const
{
    switch(posTrick)
    {
    case FIRST :
        return 0;
    case SECOND :
        return 1;
    case THIRD :
        return 2;
    case FOURTH :
        return 3;
    default : // TO DO exception here
        return 4;
    }
}
inline POSITION_TRICK Basic_Game_Info::IntToPosTrick(Uint pos) const
{
    switch(pos)
    {
    case 0 :
        return FIRST;
    case 1 :
        return SECOND;
    case 2 :
        return THIRD;
    case 3 :
        return FOURTH;
    default : // TO DO exception here
        return UNKNOWN;
    }
}

inline POSITION_PLAYER Basic_Game_Info::FirstToPlay(POSITION_TRICK posTrick, POSITION_PLAYER playerConcerned) const
{
    Uint posT = PosTrickToInt(posTrick);
    Uint posP = PosPlayerToInt(playerConcerned);
    return IntToPosPlayer( (posP+static_cast<Uint>(4-posT))% 4 );
}
#endif // BASIC_GAME_INFO_H
