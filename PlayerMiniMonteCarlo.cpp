#include "PlayerMiniMonteCarlo.h"


void PlayerMiniMonteCarlo::ReceiveInitInfo(Random& initRandomState)
{
	_rand = &initRandomState;
}

void PlayerMiniMonteCarlo::AddConstraint(const Cards_Basic& card)
{
	_canReceiveCard[card] = false;
	_printf("I have added the constraint in player\n");
}


void PlayerMiniMonteCarlo::RemoveConstraint(const Cards_Basic& card)
{
	//TO DO : maybe remember the initial state : we don't want a constraint to be added illegally
	_canReceiveCard[card] = true;
}


void PlayerMiniMonteCarlo::ReceiveHand(const std::list<Cards*>& initHand)
{
	_hand.clear();
	for (auto pCard : initHand)
	{
		_hand.push_back(static_cast<Cards_Basic>(*pCard));
	}
	_nbCardToReceive = 0;

}


void PlayerMiniMonteCarlo::ReceiveCard(const Cards_Basic& card)
{
	_hand.push_back(card);
	_nbCardToReceive--;
}


bool PlayerMiniMonteCarlo::CanReceiveAnotherCard() const
{
	return _nbCardToReceive > 0;
}


bool PlayerMiniMonteCarlo::CanReceiveCard(const Cards_Basic& card) const
{
	bool ok = _canReceiveCard.at(card);
	return ok && _nbCardToReceive > 0;
}


void PlayerMiniMonteCarlo::RetrieveCard(const Cards_Basic& card)
{
	auto it = _hand.begin();
	const Card_Height height = card.GetHeight();
	const Card_Color color = card.GetColour();
	_printf("I mus retrieve [c:%d,h:%d]\n",color.ToInt(),height.ToInt());
	_printf("I have in Hand :");
	for (auto itC = _hand.begin(); itC != _hand.end(); ++itC)
	{
		_printf("[c:%d,h:%d];",itC->GetColour().ToInt(),itC->GetHeight().ToInt());
		if (itC->GetColour() == color && itC->GetHeight() == height)
		{
			it = itC;
		}
	}
	_printf("\n");
	_nbCardToReceive++;
	it = _hand.erase(it);
}


void PlayerMiniMonteCarlo::RetrieveCard(std::list<Cards_Basic>::iterator& itCard) //retrieve the card from the hand
{
	itCard = _hand.erase(itCard);
}


void PlayerMiniMonteCarlo::PrintHand() const
{
	printf("Mini player %d have :\n", _number.ToInt());
	for (auto card : _hand)
	{
		printf("[c:%d,h:%d] ,", card.GetColour().ToInt(), card.GetHeight().ToInt());
	}
	printf("\n");
}


//TO DO : this is a copy/paste from player, there may be another option...
void PlayerMiniMonteCarlo::updatePlayableCard(const TrickBasic_Memory& trick)
{
	_playableCards.clear();
	if (trick.NumberCardsPlayed() == 0) //if we are the first to play, we can play everything
	{
		for (auto pcard : _hand)
		{
			_playableCards.push_back(pcard);
		}
		return;
	}

	_status.Update(trick
		, has_colour(trick.ColorAsked())
		, has_colour(_basic_info.TrumpColor()));
	_fCanPlayCard.Init(trick, _status, _hand);
	for (auto pcard : _hand)
	{
		if (_fCanPlayCard(pcard)) _playableCards.push_back(pcard);
	}
}

//TO DO : this is a copy/paste from player, there may be another option...
bool PlayerMiniMonteCarlo::has_colour(const Card_Color& colour) //do I have the color
{
	for (auto pcard : _hand)
	{
		if (pcard.GetColour() == colour)
			return true;
	}
	return false;
}
