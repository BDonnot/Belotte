#ifndef BIDDING_H
#define BIDDING_H

/**
** This class implement the bidding phase.
** Each player bid (one after the other), at the end it
** stores the "winning bet" which define the taker and
** the contract : minimum score to be done and color of
** trump.
**/

#include<array>
#include<list>
#include "Definitions.h"

#include "Basic_Game_Info.h"
#include "Player.h"
#include "Cards_Deck.h"
#include "BetsMemory.h"

//Define the variable for debugging purpose
#include "DebugwithPrint.h"
#define BIDDING_DEBUG 0

class Bidding
{
	protected :
		Basic_Game_Info _infos;
		std::array<Player*,4> _players;
		BetsMemory _bets;
#if COMPLETE_GAME > 0
		SDL_Event* _pEvent;
		SDL_Surface* _pScreen;
#endif //#if COMPLETE_GAME > 0
		Uint _previousPlayerPassed;
		Images_Text _textAfterBet0;
		Images_Text _textAfterBet1;
		Images_Button _ok;
		Cards_Deck* _pDeck;
		Player_ID _bidder;
		WrapperPrint<BIDDING_DEBUG> _printf;

	public:
		Bidding(){}
		Bidding(const std::array<Player*, 4>& players, Cards_Deck* pDeck);
#if COMPLETE_GAME > 0
		Bidding(const std::array<Player*,4>& players,SDL_Event* pevent,SDL_Surface* screen,Cards_Deck* pDeck);
#endif //#if COMPLETE_GAME > 0
		virtual ~Bidding(){}
		bool Bid(GAME_PHASES currentPhase);
		void SetPlayers(const std::array<Player*,4>& players);
		void UpdateEvent(GAME_PHASES currentPhase);
		void Display(GAME_PHASES currentPhase);
		bool Click(bool Short);
		void SummarizeBet();
		GAME_PHASES NextPhase();

		void Reset(); //reset the previous bets

		std::string GetString(); //not const because it erase the bets made in _bets.
	protected:
		void handleBet(const Player_Bid& bid, const Player_ID& ibet);
		void handleText();
	private:
};
#endif // BIDDING_H
