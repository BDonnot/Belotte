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

#include "Basic_Game_Info.h"
#include "Player.h"
#include "Cards_Deck.h"
#include "BetsMemory.h"

//Define the variable for debugging purpose
#include "DebugwithPrint.h"
#define BIDDING_DEBUG 0

#if COMPLETE_GAME > 0
	class Bidding
	{
		protected :
			Basic_Game_Info _infos;
			std::array<Player*,4> _players;
			BetsMemory _bets;
			SDL_Event* _pEvent;
			SDL_Surface* _pScreen;
			Uint _previousPlayerPassed;
			SDL::Images_Text _textAfterBet0;
			SDL::Images_Text _textAfterBet1;
			SDL::Images_Button _ok;
			Cards_Deck* _pDeck;
			Player_ID _bidder;
			WrapperPrint<BIDDING_DEBUG> _printf;

		public:
			Bidding(){}
			Bidding(const std::array<Player*,4>& players,SDL_Event* pevent,SDL_Surface* screen,Cards_Deck* pDeck);
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
#else
	class Bidding
	{
	protected:
		Basic_Game_Info _infos;
		std::array<Player*, 4> _players;
		BetsMemory _bets;
		Uint _previousPlayerPassed;
		SDL::Images_Text _textAfterBet0;
		SDL::Images_Text _textAfterBet1;
		//SDL::Images_Button _ok;
		Cards_Deck* _pDeck;
		Player_ID _bidder;
		WrapperPrint<BIDDING_DEBUG> _printf;

	public:
		Bidding() {}
		Bidding(const std::array<Player*, 4>& players, Cards_Deck* pDeck);
		virtual ~Bidding() {}
		bool Bid(GAME_PHASES currentPhase);
		void SetPlayers(const std::array<Player*, 4>& players);
		void UpdateEvent(GAME_PHASES currentPhase){}
		void Display(GAME_PHASES currentPhase){}
		bool Click(bool Short) { return true; }
		void SummarizeBet();
		GAME_PHASES NextPhase();

		void Reset(); //reset the previous bets

		std::string GetString(); //not const because it erase the bets made in _bets.
	protected:
		void handleBet(const Player_Bid& bid, const Player_ID& ibet);
		void handleText();
	private:
	};
#endif //COMPLETE_GAME
#endif // BIDDING_H
