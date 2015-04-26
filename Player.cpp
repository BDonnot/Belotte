#include "Player.h"
using namespace::std;

//TO DO update the memory, release it when the game is over !

void Player::ReceivedCard(Cards* pcard,unsigned int card_number)
{
    //pcard->InitMouvement(true,_pos,1000,50*card_number);
    pcard->InitMouvement(_number==PLAYER0,_pos,1000,50*card_number);
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
        case PLAYER3 : //the left player (ouest)
            res.Set(0,windows_height/2,CENTER_LEFT);
            return res;
        default : //TO DO exception here
            res.Set(windows_width/2,windows_height/2,CENTER);
            return res;
    }
}
string Player::setName(const Player_ID&  number)
{
    string res;
    switch(number.ID())
    {
        case PLAYER0 : //you : the bottom player (sud)
            res = "Sud";
            return res;
        case PLAYER1 : //the left player (ouest)
            res = "Ouest";
            return res;
        case PLAYER2 : //the top player (nord)
            res = "Nord";
            return res;
        case PLAYER3 : //the left player (ouest)
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
    switch(number.ID())
    {
        case PLAYER0 : //you : the bottom player (sud)
            res = "images/you.jpg";
            return res;
        case PLAYER1 : //the left player (ouest)
            res = "images/maul.jpg";
            return res;
        case PLAYER2 : //the top player (nord)
            res = "images/yoda.jpg";
            return res;
        case PLAYER3 : //the left player (ouest)
            res = "images/vador.jpg";
            return res;
        default: // TO DO exception here
            return "";
    }
    return "";
}
void Player::Display(GAME_PHASES currentPhase)
{
    _name.Display(_screen,currentPhase==SELECT_NAMES);
    _oldBid.Display(_screen,currentPhase);
    _DisplayCardPlayer.Reset(_hand.size());
    for_each(_hand.begin(),_hand.end(),_DisplayCardPlayer);
}
void Player::Update_Mouse(GAME_PHASES currentPhase)
{
    _name.Update(currentPhase);
    if ((currentPhase == BIDDING)||(currentPhase == AFTER_BET))
        _oldBid.UpdateEvent();
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
/*
bool Player::has_higher(const Card_Color& color_asked,const Card_Height& max_height) //true if the player has a higher card in his game
{
    for (auto pcard : _hand)
    {
        if ((pcard->GetColour()==color_asked)&&(pcard->Win(max_height)))
            return true;
    }
    return false;
}
*/
Cards* Player::PlayCard(const TrickBasic_Memory& trick)
{
    if(_hand.size() + trick.TrickNumber()  != 8 ) return nullptr;
    //if(_number != PLAYER0) printf("%d va jouer \n",_basic_info.PosPlayerToInt(_number));
    if(_playable_cards.size()==0)
    {
        updatePlayebleCards(trick); //TO DO this is called multiple times
    }
    _cardPlayed = what_card_do_i_play(trick);
    if (!(_cardPlayed != _hand.end())) return nullptr;
    Cards* res = *_cardPlayed;
    _hand.erase(_cardPlayed);
    _playable_cards.clear();
    return res;
}
Card_Color Player::Take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed)
{
    return do_i_take(first_round,color_proposed,height_proposed);
}

void Player::UpdateEndTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos) //do whatever you have to do at the end of each trick
{
    updateMemoryTrick(trick,myPos);
}

/*
bool Player::can_play_card(Cards* PmyCard,const TrickBasic_Memory& trick)
{
    if (PmyCard==nullptr)
        return false;
    Card_Color my_colour = PmyCard->GetColour();
    Card_Height maxHeight = trick.HeightMaster();
    Card_Color maxColor = trick.ColorMaster();
    if (_currentTrickStatus.HasCol()) //If we have the color asked we have to played it
    {
        if(trick.ColorAsked() != _currentTrickStatus.TrumpColor()) //we must play in the color if we can
            return (my_colour == trick.ColorAsked());
        return (my_colour == _currentTrickStatus.TrumpColor())&&((PmyCard->Win(maxHeight))
                                                         ||(!has_higher(_currentTrickStatus.TrumpColor(),maxHeight)));
    }
    //So I do not have the color
    if ((!_currentTrickStatus.HasTrump())||(static_cast<Uint>(trick.CurrentWinner().ToInt()%2) == static_cast<Uint>(trick.NumberCardsPlayed()%2))) return true; //I can play what I want if I dont have the color asked and : I dont have trump or my partner is the master
    //So I do have trump, my partner is no master, and I do not have the color asked.
    bool trumpPlayed = maxColor == _currentTrickStatus.TrumpColor();
    if(trumpPlayed)
        return (my_colour == _currentTrickStatus.TrumpColor())&&((PmyCard->Win(maxHeight))
                                                         ||(!has_higher(_currentTrickStatus.TrumpColor(),maxHeight)));
    return my_colour == _currentTrickStatus.TrumpColor();
}
*/
void Player::updatePlayebleCards(const TrickBasic_Memory& trick_in_progress)
{
    //_memory.TellTrick(trick_in_progress,_basic_info.FirstPlayer());
    auto itEnd = _hand.end();
    if (trick_in_progress.NumberCardsPlayed() == 0) //if we are the first to play, we can play everything
    {
        for(auto it = _hand.begin(); it != itEnd;++it)
        {
            _playable_cards.push_back(it);
        }
        return;
    }

    _currentTrickStatus.Update(trick_in_progress
                               ,has_colour(trick_in_progress.ColorAsked())
                               ,has_colour(_basic_info.TrumpColor()));
    _fCanPlayCard.Init(trick_in_progress,_currentTrickStatus,_hand);
    for(auto it = _hand.begin(); it != itEnd;++it)
    {
        //if (can_play_card(*it,trick_in_progress)) _playable_cards.push_back(it);
        if( _fCanPlayCard(*it) ) _playable_cards.push_back(it);
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
    //if ((_lastBidTurn == _basic_info.BiddingRound())&&(previousPlayerChoose)&&(_currentBid.Color()==NOT_CHOSEN))
    if ((previousPlayerChoose)&&(_currentBid.Color()==NOT_CHOSEN))
    {
        updateBid(bets);
        if(_currentBid.Color()!=NOT_CHOSEN)
        {
            _oldBid.AddBid(_currentBid.Color(),_currentBid.Bid());
            //_lastBidTurn++;
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
        //_lastBidTurn++;
    }
    return _currentBid;
}

const bool Player::Coinche()
{
    return do_I_coinche();
}
bool Player::do_I_coinche()
{
    return false;
}
void Player::updateMemoryTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos)
{

}
void Player::ResetBid(bool betFinished)
{
    _currentBid.Reset();
    if (!betFinished) return;
    _oldBid.Reset();
    resetTake();
    //_memory.Reset();
}
void Player::GiveCardsBack(list<Cards*>& deck)
{
    PositionGraphic pos(_basic_info.WindowsWidth()/2,_basic_info.WindowsHeight()/2,CENTER);
    for(auto it = _hand.begin(); it !=_hand.end(); ++it)
    {
        (*it)->Reset_Click();
        (*it)->InitMouvement(_number==PLAYER0,pos,1000,0); //HERE FOR REVEALING CARDS
        // (*it)->InitMouvement(true,pos,1000,0); //HERE FOR REVEALING CARDS
    }
    deck.splice(deck.end(),_hand);
    //_oldBid.Reset();
    //ResetBid(true);
}
const Card_Color& Player::CurrentColorBid()
{
    if (_currentBid.Color() == Card_Color(NOT_CHOSEN))
        return _oldBid.Color();
    return _currentBid.Color();
}
void Player::InitMemory()
{
    //_memory.Reset();
    initMemoryTrick();
    if(_number != PLAYER0) return;
    Sort_Cards sorting;
    _hand.sort(sorting);
}

void Player::initMemoryTrick()
{
    //NOTHING TO DO
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
