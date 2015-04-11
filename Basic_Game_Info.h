#ifndef BASIC_GAME_INFO_H
#define BASIC_GAME_INFO_H

#include <array>

#include "Definitions.h"

class Basic_Game_Info
{
    protected:
        static unsigned long long _clock;
        static Player_ID _giver;

        static Player_ID _taker;
        static Card_Color _trump_color;
        static unsigned int _max_bid;
        static Card_Color _color_proposed;

        static Player_ID _position_first_player; //the first player who will played the trick
        static unsigned int _number_card_played;

        static std::array<unsigned int,2> _final_scores;
        static std::array<unsigned int,2> _running_scores;
        static std::array<unsigned int,2> _tricks_win; //number of trick won

        static Uint _windowsWidth;
        static Uint _windowsHeight;
    public:
        Basic_Game_Info();
        virtual ~Basic_Game_Info();

        const unsigned long long& Time() const;
        void AddClock(unsigned long long diffTime);

        inline const Player_ID& Taker() const;
        inline const Player_ID& Giver() const;

        inline const Card_Color& TrumpColor() const;
        inline const Card_Color& ColorProposed() const;
        inline const unsigned int& BiddingRound() const;

        inline const std::array<unsigned int,2>& RunningScores() const;
        inline const std::array<unsigned int,2>& FinalScores() const;

        inline const unsigned int& MaxBid() const;

        inline static const Uint& WindowsWidth();
        inline static const Uint& WindowsHeight();

        void NextPlayerBid(){_position_first_player.Next();}


        void SetTaker(const Player_ID& taker);
        void SetTrumpColor(const Card_Color& trump_color);
        void SetScores(const Player_ID& trick_winner,const IntIntPair& scores); //to IMPROVE !

        IntIntPair CalculateFinalScores(); //compute who wins (returns the equip of the winner), determined each score, reset the other parameters

        void SetMaxBid(unsigned int max_bid); //TO DO : smarter way here...
        void SetColorProposed(const Card_Color& color);

        //PLAYER_ID Next(PLAYER_ID posPlayer) const;
        //inline Uint PosPlayerToInt(PLAYER_ID posPlayer) const;
        //inline PLAYER_ID IntToPosPlayer(Uint num) const;
        inline Player_ID FirstToPlay(const Position_Trick& posTrick, const Player_ID& playerConcerned) const;

        void SetGiver(); //the giver is always the person at the left of the current giver
    protected:
        void setTrickNumber(); //the SetTrickNumber can increase of only 1 unit each time
        Player_ID posTrickToPlayer(const Player_ID& firstToPlay,const Position_Trick& posTrick);
};

inline const Player_ID& Basic_Game_Info::Taker() const
{
    return _taker;
}
inline const Player_ID& Basic_Game_Info::Giver() const
{
    return _giver;
}
inline const Card_Color& Basic_Game_Info::TrumpColor() const
{
    return _trump_color;
}
inline const std::array<unsigned int,2>& Basic_Game_Info::RunningScores() const
{
    return _running_scores;
}
inline const std::array<unsigned int,2>& Basic_Game_Info::FinalScores() const
{
    return _final_scores;
}
inline const unsigned int& Basic_Game_Info::MaxBid() const
{
    return _max_bid;
}
inline const Card_Color& Basic_Game_Info::ColorProposed() const
{
    return _color_proposed;
}
inline const unsigned long long& Basic_Game_Info::Time() const
{
    return _clock;
}
inline const Uint& Basic_Game_Info::WindowsWidth()
{
    return _windowsWidth;
}
inline const Uint& Basic_Game_Info::WindowsHeight()
{
    return _windowsHeight;
}
/*
inline PLAYER_ID Basic_Game_Info::IntToPosPlayer(Uint num) const
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

inline Uint Basic_Game_Info::PosPlayerToInt(PLAYER_ID posPlayer) const
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
*/
inline Player_ID Basic_Game_Info::FirstToPlay(const Position_Trick& posTrick, const Player_ID& playerConcerned) const
{
    Uint posT = posTrick.ToInt();
    Uint posP = playerConcerned.ToInt();
    return Player_ID( (posP+static_cast<Uint>(4-posT))% 4 );
}
#endif // BASIC_GAME_INFO_H
