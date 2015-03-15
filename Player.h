#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <array>
#include <algorithm>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "TrickStatus.h"
#include "Cards.h"
#include "Player_Name.h"
#include "Player_Bid.h"
#include "Player_Bid_Graphic.h"
#include "Foncteurs_Cards.h"
#include "BetsMemory.h" //for the bets to be remembered
//#include "GameMemory.h" //for the past tricks to be remembered

//TO DO handle Belote and Coinche during game phases.
//template<class TakeAI,class PlayAI>
class Player
{
    protected:
        Basic_Game_Info _basic_info;
        PositionGraphic _pos;

        Player_Name _name; //his name
        std::list<Cards*> _hand; //the hand of the player
        std::list<std::list<Cards*>::iterator> _playable_cards; //the card a player can play in a trick
        //std::array<Uint16,3> _pos; //the position and the option associated
        const PLAYER_ID _number; //the number
        //UpdateCardMouvement _UpdateCardMouvement; //to update the cards
        DisplayCardPlayer _DisplayCardPlayer;//to display the cards the proper way
        SDL_Surface* _screen;
        SDL_Event* _event;
        Player_Bid _currentBid;
        Player_Bid_Graphic _oldBid;
        unsigned int _lastBidTurn;
        std::list<Cards*>::iterator _cardPlayed;
        TrickStatus _currentTrickStatus;
        //GameMemory _memory;

        //for the playing and the taking
        //Random _rand;
        //TakeAI _take;
        //PlayAI _play;

    public:
        Player():_number(PLAYER0){}
        Player(PLAYER_ID number,Uint windows_width, Uint windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
            _basic_info()
            ,_pos(SetPosition(number,windows_width,windows_height))
            ,_name(fond
                   ,SetName(number)
                   ,SetPathImage(number)
                   ,pevent
                   ,windows_width
                   ,windows_height
                   ,_pos)
            ,_playable_cards()
            ,_number(number)
            ,_DisplayCardPlayer(screen,windows_width,windows_height,number)
            ,_currentBid()
            ,_oldBid(number,pevent,screen,windows_width,windows_height)
            ,_lastBidTurn(0)
            ,_cardPlayed(_hand.end())
        {
            _pos = SetPosition(number,windows_width,windows_height);
            _screen = screen;
        }
        virtual ~Player()
        {
            for (auto it = _hand.begin(); it != _hand.end(); ++it)
            {
                delete(*it);
            }
        }


        void ReceivedCard(Cards* pcard,unsigned int card_number); //received a card, during the phase of giving
        virtual void Display(GAME_PHASES currentPhase); //to display the right thing : the name, the image, and the cards in the hand.
        //void Update_Mouvement(Uint32 current_time); //to update the movement of the card

        virtual void Update_Mouse(GAME_PHASES currentPhase); //to update the position of the mouse, the click etc.
        //virtual void Update_Trick(std::array<Cards*,4> trick,unsigned int subscript_first_player);

        Cards* PlayCard(const std::array<Cards*,4>& trick); //choose the right card to play
        void UpdateEndTrick(const std::array<Cards*,4>& trick,POSITION_TRICK myPos); //do whatever you have to do at the end of each trick
        CARDS_COLOR Take(bool first_round,CARDS_COLOR color_proposed,CARDS_HEIGHT height_proposed); //choose if the player take or not (classic game)
        const Player_Bid& Take(bool previousPlayerChoose,const BetsMemory& bets); //choose if the player take or not (coinche)
        const Player_Bid& Take(const BetsMemory& bets); //choose if the player take or not (coinche)
        const bool Coinche();

        const unsigned int GetHandSize() const { return _hand.size(); }
        const std::string& Getname() const { return _name.Name(); }
        virtual void ResetBid(bool betFinished);
        const Uint GetLastBidTurn() const {return _lastBidTurn;}
//        void ResetBid(bool betFinished);

        void GiveCardsBack(std::list<Cards*>& deck);
        CARDS_COLOR CurrentColorBid();

        void InitMemory(); //and sort the hand
        //void

        std::string GetString(const std::string& embraced) const; //return <Player : (#)> (embraced) <\player>
        std::string GetString() const; //return <Player : (#)> (hands) <\player>
        std::string GetString(Uint biddingCount) const; //return <Player : (#)> (bet) <\player>
        std::string GetString(const Player_Bid& bid) const; //return <Player : (#)> (bet) <\player>

    protected:
        virtual void updateMemoryTrick(const std::array<Cards*,4>& trick,POSITION_TRICK myPos);
        virtual void initMemoryTrick();
        virtual void updateBid(const BetsMemory& bets){}
        virtual bool do_I_coinche();

        virtual CARDS_COLOR do_i_take(bool first_round,CARDS_COLOR color_proposed,CARDS_HEIGHT height_proposed);
        virtual std::list<Cards*>::iterator what_card_do_i_play(const std::array<Cards*,4>& trick) {return _hand.end();}

        int how_many_colour(CARDS_COLOR colour);

        virtual void resetTake(){} //for the AI to be able to forget the previous bets
    private:
        PositionGraphic SetPosition(int number,Uint16 windows_width, Uint16 widows_height); //to properly set the right position
        //std::array<Uint16,3> SetPosition(int number,Uint16 windows_width, Uint16 widows_height); //to properly set the right position
        std::string SetName(int number); //to set the default name
        std::string SetPathImage(int number); //to set the path of the image of the player
        //std::array<Uint16,2> SetPositionCards(int number,Uint16 windows_width, Uint16 widows_height);

        bool has_higher(CARDS_COLOR color_asked,CARDS_HEIGHT max_height);
        void updatePlayebleCards(const std::array<Cards*,4>& trick_in_progress);
        bool can_play_card(Cards* PmyCard
                           ,const std::array<Cards*,4>& trick_in_progress);
//        bool can_play_card(Cards* PmyCard
//                           ,const std::array<Cards*,4>& trick_in_progress
//                           ,CARDS_COLOR trumpColor
//                           ,CARDS_COLOR colour_asked
//                           ,bool has_col
//                           ,bool has_trump
//                           ,Uint myPosition
//                           ,POSITION_TRICK winner
//                           ,CARDS_HEIGHT maxHeight
//                           ,CARDS_COLOR maxColor);
        bool has_colour(CARDS_COLOR color);
        bool trump_played(const std::array<Cards*,4>& trick_in_progress);

//        bool can_play_trump(Cards* PmyCard,const std::array<Cards*,4>& trick_in_progress);
//        bool can_discarding(const std::array<Cards*,4>& trick_in_progress,CARDS_COLOR trump_colour);
        //unsigned int getPosition(const std::array<Cards*,4>& trick_in_progress);
        //unsigned int strongest_card(unsigned int position,const std::array<Cards*,4>& trick_in_progress,CARDS_COLOR trump_colour);
};
/*
template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::ReceivedCard(Cards* pcard,unsigned int card_number)
{
    pcard->InitMouvement(true,_pos,1000,50*card_number);
    _hand.push_back(pcard);
}

template<class TakeAI,class PlayAI>
PositionGraphic Player<TakeAI,PlayAI>::SetPosition(int number,Uint16 windows_width, Uint16 windows_height)
{
    PositionGraphic res;
    switch (number)
    {
        case 0 : //you : the bottom player (sud)
            res.Set(windows_width/2,windows_height,BOTTOM_CENTER);
            return res;
        case 1 : //the left player (ouest)
            res.Set(windows_width,windows_height/2,CENTER_RIGHT);
            return res;
        case 2 : //the top player (nord)
            res.Set(windows_width/2,0,TOP_CENTER);
            return res;
        case 3 : //the left player (ouest)
            res.Set(0,windows_height/2,CENTER_LEFT);
            return res;
        default :
            res.Set(windows_width/2,windows_height/2,CENTER);
            break;
    }
    return res;
}

template<class TakeAI,class PlayAI>
string Player<TakeAI,PlayAI>::SetName(int number)
{
    string res;
    switch(number)
    {
        case 0 : //you : the bottom player (sud)
            res = "Sud";
            return res;
        case 1 : //the left player (ouest)
            res = "Ouest";
            return res;
        case 2 : //the top player (nord)
            res = "Nord";
            return res;
        case 3 : //the left player (ouest)
            res = "Est";
            return res;
        default: break;
    }
    return "";
}

template<class TakeAI,class PlayAI>
string Player<TakeAI,PlayAI>::SetPathImage(int number)
{
    string res;
    switch(number)
    {
        case 0 : //you : the bottom player (sud)
            res = "images/you.jpg";
            return res;
        case 1 : //the left player (ouest)
            res = "images/maul.jpg";
            return res;
        case 2 : //the top player (nord)
            res = "images/yoda.jpg";
            return res;
        case 3 : //the left player (ouest)
            res = "images/vador.jpg";
            return res;
        default: break;
    }
    return "";
}

template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::Display(GAME_PHASES currentPhase)
{
    _name.Display(_screen,currentPhase==SELECT_NAMES);
    _oldBid.Display(_screen,currentPhase);
    _DisplayCardPlayer.Reset(_hand.size());
    for_each(_hand.begin(),_hand.end(),_DisplayCardPlayer);
}

template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::Update_Mouse(GAME_PHASES currentPhase)
{
    _name.Update(currentPhase);
    if ((currentPhase == BIDDING)||(currentPhase == AFTER_BET))
        _oldBid.UpdateEvent();
}

template<class TakeAI,class PlayAI>
int Player<TakeAI,PlayAI>::how_many_colour(CARDS_COLOR colour) //how many cards I have in the color
{
    int res = 0;
    for (list<Cards*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if ((*it)->GetColour() == colour) res++;
    }
    return res;
}

template<class TakeAI,class PlayAI>
bool Player<TakeAI,PlayAI>::has_higher(CARDS_COLOR color_asked,CARDS_HEIGHT max_height) //true if the player has a higher card in his game
{
    for (list<Cards*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if (((*it)->GetColour()==color_asked)&&((*it)->Win(max_height)))
            return true;
    }
    return false;
}

template<class TakeAI,class PlayAI>
Cards* Player<TakeAI,PlayAI>::PlayCard(const array<Cards*,4>& trick)
{
    if(_hand.size() != static_cast<Uint>(8-_basic_info.TrickNumber())) return nullptr;
    if(_playable_cards.size()==0)
    {
        updatePlayerbleCards(trick);
    }
    _cardPlayed = what_card_do_i_play(trick);
    if (!(_cardPlayed != _hand.end())) return nullptr;
    Cards* res = *_cardPlayed;
    _hand.erase(_cardPlayed);
    _playable_cards.clear();
    return res;
}

template<class TakeAI,class PlayAI>
CARDS_COLOR Player<TakeAI,PlayAI>::Take(bool first_round,CARDS_COLOR color_proposed,CARDS_HEIGHT height_proposed)
{
    return do_i_take(first_round,color_proposed,height_proposed);
}

template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::updatePlayerbleCards(const array<Cards*,4>& trick_in_progress)
{
    if (trick_in_progress[0]==nullptr) //if we are the first to play, we can play everything
    {
        for(auto it = _hand.begin(); it != _hand.end();++it)
        {
            _playable_cards.push_back(it);
        }
        return;
    }

    _currentTrickStatus.Update(trick_in_progress
                               ,has_colour(trick_in_progress[0]->GetColour())
                               ,has_colour(_basic_info.TrumpColor()));

    for(auto it = _hand.begin(); it != _hand.end();++it)
    {
       if (can_play_card(*it,trick_in_progress)) _playable_cards.push_back(it);
    }
}

template<class TakeAI,class PlayAI>
bool Player<TakeAI,PlayAI>::can_play_card(Cards* PmyCard
                           ,const array<Cards*,4>& trick_in_progress)
{
    if (PmyCard==nullptr)
        return false;
    CARDS_COLOR my_colour = PmyCard->GetColour();
    if (_currentTrickStatus.HasCol()) //If we have the color asked we have to played it
    {
        if(_currentTrickStatus.ColourAsked()!= _basic_info.TrumpColor()) //we must play in the color if we can
            return (_currentTrickStatus.ColourAsked() == my_colour);
        return (my_colour == _basic_info.TrumpColor())&&((PmyCard->Win(_currentTrickStatus.MaxHeight()))
                                                         ||(!has_higher(_basic_info.TrumpColor(),_currentTrickStatus.MaxHeight())));
    }
    //So I do not have the color
    if ((!_currentTrickStatus.HasTrump())||(_currentTrickStatus.Winner()%2 == _currentTrickStatus.Position()%2)) return true; //I can play what I want if I dont have the color asked and : I dont have trump or my partner is the master
    //So I do have trump, my partner is no master, and I do not have the color asked.
    bool trumpPlayed = _currentTrickStatus.MaxColor() == _basic_info.TrumpColor();
    if(trumpPlayed)
        return (my_colour == _basic_info.TrumpColor())&&((PmyCard->Win(_currentTrickStatus.MaxHeight()))
                                                         ||(!has_higher(_basic_info.TrumpColor(),_currentTrickStatus.MaxHeight())));
    return my_colour == _basic_info.TrumpColor();
}

template<class TakeAI,class PlayAI>
bool Player<TakeAI,PlayAI>::has_colour(CARDS_COLOR colour) //do I have the color
{
    for (list<Cards*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if ((*it)->GetColour() == colour)
            return true;
    }
    return false;
}

template<class TakeAI,class PlayAI>
bool Player<TakeAI,PlayAI>::trump_played(const std::array<Cards*,4>& trick_in_progress)
{
    return trick_in_progress[_basic_info.StrongestCard()+2]->GetColour() == _basic_info.TrumpColor();
}

template<class TakeAI,class PlayAI>
CARDS_COLOR Player<TakeAI,PlayAI>::do_i_take(bool first_round,CARDS_COLOR color_proposed,CARDS_HEIGHT height_proposed)
{
    return NO;
}

template<class TakeAI,class PlayAI>
std::list<Cards*>::iterator Player<TakeAI,PlayAI>::what_card_do_i_play(const std::array<Cards*,4>& trick)
{
    return _hand.end();
}

template<class TakeAI,class PlayAI>
const Player_Bid& Player<TakeAI,PlayAI>::Take(bool previousPlayerChoose) //choose if the player take or not
{
    if ((_lastBidTurn == _basic_info.BiddingRound())&&(previousPlayerChoose)&&(_currentBid.Color()==NOT_CHOSEN))
    {
        UpdateBid();
        if(_currentBid.Color()!=NOT_CHOSEN)
        {
            _oldBid.AddBid(_currentBid.Color(),_currentBid.Bid());
            _lastBidTurn++;
        }
    }
    return _currentBid;
}

template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::UpdateBid()
{
    //nothing to do here !
}

template<class TakeAI,class PlayAI>
const unsigned int Player<TakeAI,PlayAI>::GetHandSize() const
{
    return _hand.size();
}

template<class TakeAI,class PlayAI>
const bool Player<TakeAI,PlayAI>::Coinche()
{
    return do_I_coinche();
}

template<class TakeAI,class PlayAI>
bool Player<TakeAI,PlayAI>::do_I_coinche()
{
    return false;
}

template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::ResetBid(bool betFinished)
{
    _currentBid.Reset();
    if (!betFinished) return;
    _lastBidTurn = 0;
    _oldBid.Reset();
}

template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::GiveCardsBack(list<Cards*>& deck)
{
    PositionGraphic pos(_basic_info.WindowsWidth()/2,_basic_info.WindowsHeight()/2,CENTER);
    for(auto it = _hand.begin(); it !=_hand.end(); ++it)
    {
        (*it)->Reset_Click();
        (*it)->InitMouvement(_number==PLAYER0,pos,1000,0);
    }
    deck.splice(deck.end(),_hand);
}

template<class TakeAI,class PlayAI>
CARDS_COLOR Player<TakeAI,PlayAI>::CurrentColorBid()
{
    if (_currentBid.Color() == NOT_CHOSEN)
        return _oldBid.Color();
    return _currentBid.Color();
}

template<class TakeAI,class PlayAI>
const std::string& Player<TakeAI,PlayAI>::Getname() const
{
    return _name.Name();
}

template<class TakeAI,class PlayAI>
void Player<TakeAI,PlayAI>::SortHand()
{
    if(_number != PLAYER0) return;
    Sort_Cards sorting(_basic_info.TrumpColor());
    _hand.sort(sorting);
}

template<class TakeAI,class PlayAI>
string Player<TakeAI,PlayAI>::GetString(const std::string& embraced) const
{
    string res = "<player : ";
    res += IntToString(_number);
    res += ">";
    res += embraced;
    res += "<\\player>";
    return res;
}

template<class TakeAI,class PlayAI>
string Player<TakeAI,PlayAI>::GetString() const
{
    string embraced = "";
    for(auto it = _hand.begin(); it != _hand.end(); ++it)
    {
        embraced += (*it)->GetString();
    }
    return GetString(embraced);
}

template<class TakeAI,class PlayAI>
string Player<TakeAI,PlayAI>::GetString(Uint biddingTurn) const
{
    const string embraced = _oldBid.GetString(biddingTurn);
    if(embraced == "") return "";
    return GetString(embraced);
}
*/
#endif // PLAYER_H
