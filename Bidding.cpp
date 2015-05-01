#include "Bidding.h"

using namespace std;

Bidding::Bidding(const array<Player*,4>& players,SDL_Event* pevent, SDL_Surface* pscreen,Cards_Deck* pDeck):
_infos()
,_players(players)
,_pEvent(pevent)
,_pScreen(pscreen)
,_previousPlayerPassed(0)
,_textAfterBet0("fonts/MLSGY.ttf",35,255,255,255,"???")
,_textAfterBet1("fonts/MLSGY.ttf",35,255,255,255,"???")
,_ok(_pEvent,"OK",PositionGraphic(_infos.WindowsWidth()/2,5*_infos.WindowsHeight()/9+50,CENTER))
,_pDeck(pDeck)
{
    PositionGraphic pos(_infos.WindowsWidth()/2,4*_infos.WindowsHeight()/9-50,CENTER);
    _textAfterBet0.SetPosition(pos);
    pos.Set(_infos.WindowsWidth()/2,4*_infos.WindowsHeight()/9,CENTER);
    _textAfterBet1.SetPosition(pos);
    _bidder = _infos.Giver().NextPlayer();
}

bool Bidding::Bid(GAME_PHASES currentPhase) //TO DO test, test test !
{
    if (currentPhase != BIDDING) return false;
    bool choose = true;
    bool biddingOver = false;
    Player_ID giver = _infos.Giver();

    Uint i_playerBid = _bidder.ToInt();

    const Player_Bid& bid = _players[i_playerBid]->Take(_bets);
    choose = bid.Color() != Card_Color(NOT_CHOSEN);
    if (!choose) return false;
    _printf("%d has chosen {v: %d c : %d}\n",_bidder.ID(),bid.Bid(),bid.Color().ToInt());
    _printf("max bet : %d, current bet : %d\n",_infos.MaxBid(),bid.Bid());
    handleBet(bid,_bidder); //TO DO move to BetsMemory

    _bets.TellBet(bid);

    if(_bidder == giver)
    {
        Uint firstBiddingPlayer = giver.NextPlayer().ToInt();
        for (Uint i = firstBiddingPlayer; i < firstBiddingPlayer+4; i++)
        {
            _players[i%4]->ResetBid(false);
        }
    }
    biddingOver = _bets.IsBetsOver();
    _bidder.Next();
    if(biddingOver) return true; //ajust this.
    return false;
}
void Bidding::handleBet(const Player_Bid& bid, const Player_ID& ibet)
{
    if(bid.Color() != Card_Color(NO))
    {
        _infos.SetMaxBid(bid.Bid());
        _infos.SetTrumpColor(bid.Color());
        _infos.SetTaker(ibet);
        _printf("%d took at %d at color %d \n",ibet.ID(),bid.Bid(),bid.Color().Color());
    }
}

void Bidding::SetPlayers(const array<Player*,4>& players)
{
    _players = players;
}
void Bidding::UpdateEvent(GAME_PHASES currentPhase)
{
    if (currentPhase != AFTER_BET) return;
    _ok.Update_on_it();
}
void Bidding::Display(GAME_PHASES currentPhase)
{
    if (currentPhase != AFTER_BET) return;
    _ok.Display(_pScreen);
    _textAfterBet0.Display(_pScreen);
    _textAfterBet1.Display(_pScreen);
}
bool Bidding::Click(bool Short)
{
    return _ok.Click(Short);
}
void Bidding::SummarizeBet()
{
    _printf("I enter SummarizeBet\n");
    //the first player to play is the player after the giver
    _bidder = _infos.Giver().NextPlayer();

    switch (_infos.TrumpColor().Color()) //setting the displayed objects
    {
    case NOT_CHOSEN : case NO :
        _textAfterBet0.ChangeText("Personne n'a pris, on redonne !");
        _textAfterBet1.ChangeText(" ");
        return;
    case HEART : case DIAMOND : case SPADE : case CLUB :
        handleText();
        return;
    default :
        _textAfterBet0.ChangeText(":-( Une erreur est survenue, on redonne...");
        _textAfterBet1.ChangeText(" ");
        _bets.ResetBets();
        return;
    }
}
GAME_PHASES Bidding::NextPhase()
{
     _printf("I enter NextPhase\n");
    for(Uint i = 0; i < 4; ++i)
    {
        _players[i]->InitMemory(); //and sort the hand
    }

    Uint i_start = ((_infos.Giver().ToInt()+1)%4);
    switch (_infos.TrumpColor().Color())
    {
    case NO : case NOT_CHOSEN :
        _printf("I give the cards back\n");
        for (Uint i = i_start; i < i_start+4; i++)
        {
            _players[i%4]->GiveCardsBack(_pDeck->GetPile());
        }
        _pDeck->ReceiveCardsBack();
        return BEGINNING;
    case HEART : case DIAMOND : case SPADE : case CLUB :
        return PLAYING;
    default :
        return BEGINNING;
    }
}
void Bidding::handleText()
{
    string res = _players[_infos.Taker().ToInt()]->Getname();
    switch (_infos.Taker().ToInt()%2)
    {
    case 0 :
        res += " (équipe 1)";
        break;
    case 1 :
        res += " (équipe 2)";
        break;
    default :
        break;
    }
    res +=" a pris à";
    switch(_infos.TrumpColor().Color())
    {
    case HEART:
        res += " coeur.";
        break;
    case DIAMOND :
        res += " carreur.";
        break ;
    case SPADE:
        res += " pique.";
        break;
    case CLUB :
        res += " trèfle.";
        break;
    default :
        break;
    }
    _textAfterBet0.ChangeText(res);
    res = "Le contrat est de ";
    res += DisplayOldBets::IntToString(_infos.TrumpColor().Color(),_infos.MaxBid());
    res += " points.";
    _textAfterBet1.ChangeText(res);
}

string Bidding::GetString()
{
    string res = "<bet>";
    Uint firstPlayer = (_infos.Giver().ToInt()+1)%4;

    Uint betsNum = _bets.GetSize();
    Uint counter = 1;

    while(true)
    {
        for(Uint player = firstPlayer; player < firstPlayer+4; ++player)
        {
            res += _players[player%4]->GetString(_bets.GetString());
            counter++;
            if(counter > betsNum) break;
        }
        if(counter > betsNum) break;
    }
    res += "<\\bet>";
    return res;
}

void Bidding::Reset()
{
    _infos.SetGiver();
    _bets.ResetBets();
    for(auto it = _players.begin(); it != _players.end(); ++it)
    {
        (*it)->ResetBid(true); //the bets are over for this round.
    }
}
