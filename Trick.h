#ifndef TRICK_H
#define TRICK_H

/** This class is the last of the classes
** used to represent the trick.
** It handles mainly the displaying / updating of the
** cards.
** For example it tells the cards where to go (for pretty animations).
**/

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
#if COMPLETE_GAME > 0
        SDL_Surface* _pScreen;
#endif //#if COMPLETE_GAME > 0
		Images_SpriteEvent<4> _logo;
		Images_Text _teamName;
		Images_Text _contract;
		Images_Text _infoPreviousTrick;
		Images_Button _ok;
        PositionGraphic _posCard;
        Score_Team _scoreTeam;

    public:
        Trick(){}
#if COMPLETE_GAME > 0
        Trick(Cards_Deck* pDeck
			,const std::array<Player*,4>& players
			,Uint windowsWidth,Uint WindowsHeight
			,SDL_Event* pevent
			,SDL_Surface* pScreen);
#endif //#if COMPLETE_GAME > 0
		Trick(Cards_Deck* pDeck
			, const std::array<Player*, 4>& players
			, Uint windowsWidth, Uint WindowsHeight);
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
#if PLAY_HUMAN > 0
            _trickFinished = _trickFinished || _ok.Click(true);
#else
            _trickFinished = true; //HERE
#endif
        }
        virtual void animateIfGraphic(Cards* pcard, Uint i)
        {
            updatePosCard(i%4);
            //pcard->InitMouvement(pcard->GetSpriteNumber() == 0,_posCard,1000,(i%4)*50);
            pcard->RevealCard(_posCard,1000,(i%4)*50); //reveal the hidden cards
        }
    private:
};

#endif // TRICK_H
