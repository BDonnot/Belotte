#ifndef TRICK_H
#define TRICK_H

#include<array>
#include<string>
#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Images_SpriteEvent.h"
#include "Cards.h"
#include "Player.h"
#include "Player_Bid_Graphic.h"
#include "Cards_Deck.h"
#include "Score_Team.h"
class Trick
{
    protected :
        Basic_Game_Info _info;
        Cards_Deck* _pDeck;
        std::array<Player*,4> _players;
        std::array<Cards*,4> _currentTrick;
        std::array<Cards*,4> _previousTrick;
        std::array<POSITION_TRICK,4> _posTrick;
        SDL_Surface* _pScreen;
        Images_SpriteEvent<4> _logo;
        Images_Text _teamName;
        Images_Text _contract;
        Images_Text _infoPreviousTrick;
        Images_Button _ok;
        Uint _cardsPlayedThisTurn;
        PositionGraphic _posCard;
        Score_Team _scoreTeam;
        POSITION_PLAYER _oldFirstPlayer;

    public:
        Trick();
        Trick(Cards_Deck* pDeck,const std::array<Player*,4>& players,Uint windowsWidth,Uint WindowsHeight,SDL_Event* pevent,SDL_Surface* pScreen);
        virtual ~Trick();
        void Update(); //update the taker, the trump color and the contract
        void Display(GAME_PHASES currentPhase);
        void UpdateEvent(GAME_PHASES currentPhase);
        bool Play();
        void SetPlayers(const std::array<Player*,4>& players);
        //bool Click(bool Short);
        void GatherCards();
        void WinnerTakeCards();
        std::string GetString() const;
    protected:
        void updatePosCard(Uint playerNumber);
        POSITION_TRICK whoWinTrick();
        void posReturn(POSITION_PLAYER posPlayer);
    private:
};

#endif // TRICK_H
