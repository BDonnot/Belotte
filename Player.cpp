#include "Player.h"
using namespace::std;


void Player::ReceivedCard(Cards* pcard,unsigned int card_number)
{
    //pcard->InitMouvement(true,_pos,1000,50*card_number); //only the cards of "North are revealed"
    //pcard->InitMouvement(_number == PLAYER0,_pos,1000,50*card_number); //only the cards of "North are revealed"
    if(_number == PLAYER0)
        pcard->RevealCard(_pos,1000,50*card_number);
    else
        pcard->HideCard(_pos,1000,50*card_number);

    _hand.push_back(pcard);
}

PositionGraphic Player::setPosition(const Player_ID& number,Uint16 windows_width, Uint16 windows_height)
{
    PositionGraphic res;
    switch (number.ID())
    {
        case PLAYER0 : //you : the bottom player (sud)
            res.Set(windows_width/2,windows_height,BOTTOM_CENTER);
            return res;
        case PLAYER1 : //the left player (ouest)
            res.Set(windows_width,windows_height/2,CENTER_RIGHT);
            return res;
        case PLAYER2 : //the top player (nord)
            res.Set(windows_width/2,0,TOP_CENTER);
            return res;
        case PLAYER3: //the left player (ouest)
			res.Set(0, windows_height / 2, CENTER_LEFT);
			return res;
		default: //TO DO exception here
			res.Set(windows_width / 2, windows_height / 2, CENTER);
			return res;
	}
}

string Player::setName(const Player_ID&  number)
{
	string res;
	switch (number.ID())
	{
	case PLAYER0: //you : the bottom player (sud)
		res = "Sud";
		return res;
	case PLAYER1: //the left player (ouest)
		res = "Ouest";
		return res;
	case PLAYER2: //the top player (nord)
		res = "Nord";
		return res;
	case PLAYER3: //the left player (ouest)
		res = "Est";
		return res;
	default: //TO DO exception here
		return "";
	}
	return "";
}

string Player::setPathImage(const Player_ID&  number)
{
	string res;
	switch (number.ID())
	{
	case PLAYER0: //you : the bottom player (sud)
		res = "images/you.jpg";
		return res;
	case PLAYER1: //the left player (ouest)
		res = "images/maul.jpg";
		return res;
	case PLAYER2: //the top player (nord)
		res = "images/yoda.jpg";
		return res;
	case PLAYER3: //the left player (ouest)
		res = "images/vador.jpg";
		return res;
	default: // TO DO exception here
		return "";
	}
	return "";
}

int Player::how_many_colour(const Card_Color& colour) //how many cards I have in the color
{
	int res = 0;
	for (list<Cards*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
	{
		if ((*it)->GetColour() == colour) res++;
	}
	return res;
}

Cards* Player::PlayCard(const TrickBasic_Memory& trick)
{
    _printf("I enter PlayCard\n");
	if (_hand.size() + trick.TrickNumber() != 8) return nullptr;
	if (_playable_cards.size() == 0)
	{
		updatePlayebleCards(trick); //TO DO this is called multiple times
	}
	_cardPlayed = what_card_do_i_play(trick);
	if (!(_cardPlayed != nullptr)) return nullptr;
	Card_Color colPlayed = _cardPlayed->GetColour();
	Card_Height heightPlayed = _cardPlayed->GetHeight();
	for (auto it = _hand.begin(); it != _hand.end(); ++it)
	{
		if (((*it)->GetColour() == colPlayed) && ((*it)->GetHeight() == heightPlayed))
		{
			it = _hand.erase(it);
			break;
		}
	}
    _playable_cards.clear();
	return _cardPlayed;
}

Card_Color Player::Take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed)
{
    return do_i_take(first_round,color_proposed,height_proposed);
}

void Player::UpdateEndTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos) //do whatever you have to do at the end of each trick
{
    updateMemoryTrick(trick,myPos);
}

void Player::updatePlayebleCards(const TrickBasic_Memory& trick_in_progress)
{
    if (trick_in_progress.NumberCardsPlayed() == 0) //if we are the first to play, we can play everything
    {
        for(Cards* pcard : _hand)
        {
            _playable_cards.push_back(pcard);
        }
        return;
    }

    _currentTrickStatus.Update(trick_in_progress
                               ,has_colour(trick_in_progress.ColorAsked())
                               ,has_colour(_basic_info.TrumpColor()));
    _fCanPlayCard.Init(trick_in_progress,_currentTrickStatus,_hand);
	for (Cards* pcard : _hand)
    {
        if( _fCanPlayCard(pcard) ) _playable_cards.push_back(pcard);
    }
}


bool Player::has_colour(const Card_Color& colour) //do I have the color
{
    for (auto pcard : _hand)
    {
        if (pcard->GetColour() == colour)
            return true;
    }
    return false;
}

Card_Color Player::do_i_take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed)
{
    return Card_Color(NO);
}

const Player_Bid& Player::Take(bool previousPlayerChoose,const BetsMemory& bets) //choose if the player take or not
{
    if ((previousPlayerChoose)&&(_currentBid.Color()==NOT_CHOSEN))
    {
        updateBid(bets);
        if(_currentBid.Color()!=NOT_CHOSEN)
        {
            _oldBid.AddBid(_currentBid.Color(),_currentBid.Bid());
        }
    }
    return _currentBid;
}

const Player_Bid& Player::Take(const BetsMemory& bets) //choose if the player take or not
{
    updateBid(bets);
    if(_currentBid.Color()!=NOT_CHOSEN)
    {
        _oldBid.AddBid(_currentBid.Color(),_currentBid.Bid());
    }
    return _currentBid;
}

const bool Player::Coinche()
{
    //currently unused
    return do_I_coinche();
}

bool Player::do_I_coinche()
{
    //currently unused
    return false;
}

void Player::ResetBid(bool betFinished)
{
    _currentBid.Reset();
    if (!betFinished) return;
    _oldBid.Reset();
    resetTake();
}

void Player::GiveCardsBack(list<Cards*>& deck)
{
    _printf("%d give the cards back\n",_number.ToInt());
    PositionGraphic pos(_basic_info.WindowsWidth()/2,_basic_info.WindowsHeight()/2,CENTER);
    for(Cards* pcard : _hand)
    {
        pcard->Reset_Click();
        pcard->HideCard(pos,1000,0); //HERE FOR HIDING CARDS
    }
    deck.splice(deck.end(),_hand);
}

const Card_Color& Player::CurrentColorBid()
{
    if (_currentBid.Color() == Card_Color(NOT_CHOSEN))
        return _oldBid.Color();
    return _currentBid.Color();
}
void Player::InitMemory()
{
    initMemoryTrick();
    if(_number != PLAYER0) return;
    Sort_Cards sorting;
    _hand.sort(sorting);
}

string Player::GetString(const std::string& embraced) const
{
    string res = "<player : ";
    res += IntToString(_number.ToInt());
    res += ">";
    res += embraced;
    res += "<\\player>";
    return res;
}

string Player::GetString() const
{
    string embraced = "";
    for(auto it = _hand.begin(); it != _hand.end(); ++it)
    {
        embraced += (*it)->GetString();
    }
    return GetString(embraced);
}

string Player::GetString(Uint biddingTurn) const
{
    const string embraced = _oldBid.GetString(biddingTurn);
    if(embraced == "") return "";
    return GetString(embraced);
}

string Player::GetString(const Player_Bid& bid) const
{
    const string& embraced = bid.GetString();
    if(embraced == "") return "";
    return GetString(embraced);
}

#if COMPLETE_GAME > 0
void Player::Display(GAME_PHASES currentPhase)
{
	_name.Display(_screen, currentPhase == SELECT_NAMES);
	_oldBid.Display(_screen, currentPhase);
	_DisplayCardPlayer.Reset(_hand.size());
	for_each(_hand.begin(), _hand.end(), _DisplayCardPlayer); //display the cards in the right order
}

void Player::Update_Mouse(GAME_PHASES currentPhase)
{
	_name.Update(currentPhase);
	if ((currentPhase == BIDDING) || (currentPhase == AFTER_BET))
		_oldBid.UpdateEvent();
}
#endif //#if COMPLETE_GAME > 0
