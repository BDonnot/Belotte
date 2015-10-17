#include "Cards_Deck.h"
using namespace::std;
using namespace SDL;

Cards_Deck::Cards_Deck(SDL_Event* event,SDL_Surface* screen,Uint16 screenWidth, Uint16 screenHeight):
_DisplayCardPile(screen)
,_screenCenter(PositionGraphic(screenWidth/2,screenHeight/2,CENTER))
,_rand(3,28)
,_randDecreasing(31)
{
    _first = true;
    Cards* pcard = NULL;
    array<CARDS_COLOR,4> Color = {DIAMOND,HEART,SPADE,CLUB};
    array<CARDS_HEIGHT,8> Height = {SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING,ACE};
    for (int height = 0; height < 8; height++)
    {
        for(int color = 0; color < 4; color++)
        {
            pcard = new Cards(Card_Height(Height[height]),Color[color],event,screen);
            pcard->SetPosition(_screenCenter);
            _pile.push_back(pcard);
        }
    }
}

Cards_Deck::~Cards_Deck()
{
    for(Cards * pcards : _pile)
    {
        delete (pcards);
    }
}

void Cards_Deck::BeginGame()
{
    if (_first) Shuffle();
    else Cut();
    _first = false;
}

void Cards_Deck::Cut() //cut the pack
{
    for(Cards* pCard : _pile)
    {
        pCard->SetPosition(_screenCenter);
        pCard->ChangeSprite(0);
    }
    int nb_rand = _rand.generate_number();
    list<Cards*>::iterator it = _pile.begin();
    advance(it,nb_rand);
    list<Cards*> aux;
    aux.splice(aux.begin(),_pile,_pile.begin(),it);
    _pile.splice(_pile.end(),aux);
}

void Cards_Deck::Shuffle()
{
    list<Cards*> aux;
    int nb_rand = 0;
    list<Cards*>::iterator it=_pile.begin();
    for (int i = 0; i < 31; i++)
    {
        it = _pile.begin();
        nb_rand = _randDecreasing();
        advance(it,nb_rand); //linear
		aux.splice(aux.begin(), _pile, it); //constant
    }
	aux.splice(aux.begin(), _pile, _pile.begin()); //constant
    _pile.swap(aux); //constant
}

void Cards_Deck::GiveCards(const list<Player*>& li_players,bool first_round,unsigned int i_taker)
{
    //DEPRECATED (used for classical Belote)
    Cards* pcard = NULL;
    unsigned int nb_cards_given = 3;
    unsigned int counter = 0;
    if (first_round)
    {
        for(int k = 0; k < 2; k++)
        {
            for (auto pplayer = li_players.begin();pplayer != li_players.end(); ++pplayer)
            {
                for (unsigned int i = 0; i < nb_cards_given;i++)
                {
                    pcard = _pile.front();
                    _pile.pop_front();
                    (*pplayer) -> ReceivedCard(pcard,counter);
                }
            }
            nb_cards_given--;
        }
        //(_pile.front())->Reveal(1000,50*counter,_info.Time());
    }
    else
    {
        auto itP = li_players.begin();
        advance(itP,i_taker);
        (*itP)->ReceivedCard(_pile.front(),counter);
        _pile.pop_front();
        unsigned int p = 0;
        for (auto pplayer = li_players.begin();pplayer != li_players.end(); ++pplayer)
        {
            if (p == i_taker) nb_cards_given = 2;
            else nb_cards_given = 3;
            p++;
            for (unsigned int i = 0; i < nb_cards_given;i++)
            {
                pcard = _pile.front();
                _pile.pop_front();
                (*pplayer) -> ReceivedCard(pcard,counter);
            }
        }
    }
}

void Cards_Deck::GiveCards(const std::list<Player*>& li_players)
{
    Cards* pcard = NULL;
    unsigned int nb_cards_given = 0;
    unsigned int counter = 0;
    unsigned int number_cards[3] = {3,2,3};
    for(unsigned int k = 0; k < 3; k++)
    {
        nb_cards_given = number_cards[k];
        for (auto pplayer = li_players.begin();pplayer != li_players.end(); ++pplayer)
        {
            for (unsigned int i = 0; i < nb_cards_given;i++)
            {
                pcard = _pile.front();
                _pile.pop_front();
                (*pplayer) -> ReceivedCard(pcard,counter);
                counter++;
            }
        }
    }
}

void Cards_Deck::GiveCards(const std::array<Player*,4>& li_players)
{
    Cards* pcard = NULL;
    unsigned int nb_cards_given = 0;
    unsigned int counter = 0;
    unsigned int number_cards[3] = {3,2,3};
    for(unsigned int k = 0; k < 3; k++)
    {
        nb_cards_given = number_cards[k];
        for (auto pplayer = li_players.begin(); pplayer != li_players.end(); ++pplayer)
        {
            for (unsigned int i = 0; i < nb_cards_given;i++)
            {
                pcard = _pile.front();
                _pile.pop_front();
                (*pplayer) -> ReceivedCard(pcard,counter);
                counter++;
            }
        }
    }
}

void Cards_Deck::Display(GAME_PHASES currentPhase)
{
    if((currentPhase!=SELECT_NAMES)&&(currentPhase!=GIVING)) return;
    for_each(_pile.rbegin(),_pile.rend(),_DisplayCardPile);
}

const bool Cards_Deck::FirstGame()
{
    return _first;
}

const bool Cards_Deck::Click(GAME_PHASES currentPhase)
{
#if PLAY_HUMAN > 0
    for(auto it = _pile.begin(); it != _pile.end(); ++it)
    {
        if ((*it)->Click(true))
        {
            for(auto it_bis = it; it_bis != _pile.end();++it_bis)
            {
                (*it)->Reset_Click();
            }
            return true;
        }
    }
    return false;
#else
    return true;
#endif
}
void Cards_Deck::UpdateEvent(GAME_PHASES currentPhase)
{
    if ((currentPhase != GIVING)&&(currentPhase != SELECT_NAMES)) return;
    for(Cards* pcard : _pile)
    {
        pcard->Update_on_it();
    }
}

void Cards_Deck::Reset()
{
    for(Cards* pcard : _pile)
    {
        pcard->Reset();
    }
}

void Cards_Deck::GetCardBack(Cards*& pCard)
{
    pCard->SetPosition(_screenCenter);
    pCard->ChangeSprite(0);
    pCard->Reset();
    _pile.push_back(pCard);
    pCard = nullptr;
}

void Cards_Deck::ReceiveCardsBack()
{
    /*
    for(Cards* pcard : _pile)
    {
        pcard->ChangeSprite(0);
        pcard->Reset();
    }
    */
}
