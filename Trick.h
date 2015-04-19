#ifndef TRICK_H
#define TRICK_H

#include<array>
#include<string>
#include "Definitions.h"
#include "Basic_Game_Info.h"

#include "Images_SpriteEvent.h"
#include "Trick_Basic.h"
#include "Cards.h"
#include "Player.h"
#include "Player_Bid_Graphic.h"
#include "Cards_Deck.h"
#include "Score_Team.h"
class Trick :public Trick_Basic
{
    protected :
        SDL_Surface* _pScreen;
        Images_SpriteEvent<4> _logo;
        Images_Text _teamName;
        Images_Text _contract;
        Images_Text _infoPreviousTrick;
        Images_Button _ok;
        PositionGraphic _posCard;
        Score_Team _scoreTeam;

    public:
        Trick(){}
        Trick(Cards_Deck* pDeck,const std::array<Player*,4>& players,Uint windowsWidth,Uint WindowsHeight,SDL_Event* pevent,SDL_Surface* pScreen);
        virtual ~Trick(){}

        void Update(); //update the the logos of taker, trump color and the contract
        void Display(GAME_PHASES currentPhase);
        void UpdateEvent(GAME_PHASES currentPhase);

        void GatherCards();

    protected:
        void updatePosCard(const Player_ID& playerNumber);
        void posReturn(const Player_ID& posPlayer);
        virtual void trickFinished()
        {
            //printf("ok ; %d\n",_trickFinished);
            //_trickFinished = _trickFinished || _ok.Click(true);
            _trickFinished = true;
        }
        virtual void animateIfGraphic(Cards* pcard, Uint i)
        {
            updatePosCard(i%4);
            pcard->InitMouvement(true,_posCard,1000,(i%4)*50);
        }
    private:
};

#endif // TRICK_H
