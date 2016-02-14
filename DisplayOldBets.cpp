#include "DisplayOldBets.h"
using namespace std;
using namespace SDL;

#if COMPLETE_GAME > 0
DisplayOldBets::DisplayOldBets(SDL_Surface* screen
	, Uint windows_width
	, Uint windows_height
	, const Player_ID& player_number
	, Images_Text* text
	, Images_Sprite<4>* logo) :
	_screen(screen)
	, _pText(text)
	, _pLogo(logo)
{
	bool vertical_move = (player_number.ToInt() % 2 == 1);
	if (vertical_move)
	{
		_shift.at(0) = 0;
		_shift.at(1) = 60;
	}
	else
	{
		_shift.at(0) = 60;
		_shift.at(1) = 0;
	}
	Uint position[4][2] = { { windows_width / 2 - 100, windows_height - 230 }
		, { windows_width - 200, windows_height / 2 - 135 }
		, { windows_width / 2 - 100, 215 }
	, { 200, windows_height / 2 - 135 } };
	_pos_th.Set(position[player_number.ToInt()][0], position[player_number.ToInt()][1], CENTER);

}

void DisplayOldBets::Reset()
{
	_nb_bets = 0;
}

void DisplayOldBets::operator()(const Player_Bid& bid)
{
	string text;
	switch (bid.Color().Color())
	{
	case NOT_CHOSEN:
		return;
	case NO:
		text = "passe";
		_pText->ChangeText(text);
		_pos_real = _pos_th.Shift(_nb_bets, _shift);
		_pText->SetPosition(_pos_real);
		_pText->Display(_screen);
		break;
	default:
		text = IntToString(bid.Color(), bid.Bid());
		_pLogo->ChangeSprite(bid.Color().ToInt());
		_pos_real = _pos_th.Shift(_nb_bets, _shift);
		_pLogo->SetPosition(_pos_real);
		_pLogo->Display(_screen);
		_pText->ChangeText(text);
		_pos_real = _pos_th.Shift(_nb_bets, _shift);
		_pText->SetPosition(_pos_real);
		_pText->Display(_screen);
		break;
	}
	_nb_bets++;
}

Uint DisplayOldBets::colorToInt(CARDS_COLOR color)
{
	switch (color)
	{
	case DIAMOND:
		return 0;
	case HEART:
		return 1;
	case SPADE:
		return 2;
	case CLUB:
		return 3;
	default:
		return 0;
	}
}

string DisplayOldBets::IntToString(const Card_Color & color, const Uint& number)
{
	string Result;
	if (color.Color() == NO)
	{
		Result = "Passe";
	}
	else
	{
		ostringstream convert;
		convert << number;
		Result = convert.str();
	}
	return Result;
}
#endif // COMPLETE_GAME > 0
