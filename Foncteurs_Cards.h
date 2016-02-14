#ifndef FONCTEURS_CARDS_H_INCLUDED
#define FONCTEURS_CARDS_H_INCLUDED

/**
** These classes define some functors dealing with Cards, or Cards*
**/

#include "Cards.h"
namespace SDL
{
#if COMPLETE_GAME > 0
	class DisplayCardPlayer //handle the positioning of the card in the hand
	{
	public:
		DisplayCardPlayer() {};
		DisplayCardPlayer(SDL_Surface* screen, Uint windows_width, Uint windows_height, const Player_ID& player_number)
		{
			_screen = screen;
			bool vertical_move = (player_number.ToInt() % 2 == 1);
			if (vertical_move)
			{
				_shift.at(0) = 0;
				_shift.at(1) = 21;
			}
			else
			{
				_shift.at(0) = 27;
				_shift.at(1) = 0;
			}
			Uint& pos_x = _pos_player.Getx();
			Uint& pos_y = _pos_player.Gety();
			switch (player_number.ID())
			{
			case PLAYER0:
				pos_x = windows_width / 2;
				pos_y = windows_height - 150;
				break;
			case PLAYER1:
				pos_x = windows_width - 135;
				pos_y = windows_height / 2;
				break;
			case PLAYER2:
				pos_x = windows_width / 2;
				pos_y = 150;
				break;
			case PLAYER3:
				pos_x = 135;
				pos_y = windows_height / 2;
				break;
			default:
				pos_x = windows_width / 2;
				pos_y = windows_height / 2;
				break;
			}

		}
		void Reset(unsigned int n)
		{
			_total_card = n;
			_nb_cards = 0;
		}
		void operator()(Cards* pcard)
		{
			_nb_cards++;
			UpdatePosHand(pcard->GetUp());
			pcard->UpdatePositionHand(_pos_hand);
			pcard->Display();
		}
	private:
		unsigned int _nb_cards;
		unsigned int _total_card;
		PositionGraphic _pos_player;
		PositionGraphic _pos_hand;
		std::array<Uint, 2> _shift; //the shift of the card (all the card must be visible)
		SDL_Surface* _screen;
		void UpdatePosHand(bool up)
		{
			Uint shift = 0;
			if (_total_card % 2 == 0) shift = _nb_cards - _total_card / 2;
			else shift = _nb_cards - (_total_card + 1) / 2;

			_pos_hand.Set(_pos_player.Getx() + shift*_shift[0]
				, _pos_player.Gety() + shift*_shift[1]
				, CENTER);
			if (up)
				_pos_hand.Gety() -= 35;
		}
	};

	class DisplayCardPile
	{
	public:
		DisplayCardPile() {}
		DisplayCardPile(SDL_Surface* screen) :_screen(screen) {}
		void operator()(Cards* pcard)
		{
			pcard->Display();
		}
	private:
		SDL_Surface* _screen;

	};

	class UpdateCardMouse
	{
	public:
		UpdateCardMouse() :_on_it(true), _number(0), _number_click(255) {};
		void operator()(Cards* pcard)
		{
			pcard->Update_on_it();
			if ((pcard->Mouse_Over()) && (_on_it)) //we click on the card, and we didn't click on an other card
			{
				_on_it = false;
				pcard->Up(true);
				if (pcard->Click(true)) _number_click = _number;
			}
			else pcard->Up(false);
			_number++;
		}
		void Reset()
		{
			_on_it = true;
			_number = 0;
			_number_click = 255;
		}
		Uint8 Click()
		{
			return _number_click;
		}
	private:
		bool _on_it;
		Uint8 _number; //the number of card currently updated
		Uint8 _number_click; //the number of the card we have click on

	};
#else
class DisplayCardPlayer //handle the positioning of the card in the hand
{
public:
	DisplayCardPlayer() {};
	DisplayCardPlayer(Uint windows_width, Uint windows_height, const Player_ID& player_number) {}
	void Reset(unsigned int n) {}
	void operator()(Cards* pcard) {}
};

class DisplayCardPile
{
public:
	DisplayCardPile() {}
	void operator()(Cards* pcard)
	{
		pcard->Display();
	}
};

class UpdateCardMouse
{
public:
	UpdateCardMouse() :_on_it(true), _number(0), _number_click(255) {};
	void operator()(Cards* pcard) {}
	void Reset() {}
	Uint8 Click() {}
};

#endif //#if COMPLETE_GAME > 0
}
#endif // FONCTEURS_CARDS_H_INCLUDED
