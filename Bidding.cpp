#include "Bidding.h"

using namespace std;

Bidding::Bidding(const array<Player*,4>& players,SDL_Event* pevent, SDL_Surface* pscreen,Cards_Deck* pDeck):
_infos()
,_players(players)
,_pEvent(pevent)
,_pScreen(pscreen)
,_previousPlayerPassed(0)
,_textAfterBet0("fonts/MLSGY.ttf",35,{255,255,255},"???")
,_textAfterBet1("fonts/MLSGY.ttf",35,{255,255,255},"???")
,_ok(_pEvent,"OK",PositionGraphic(_infos.WindowsWidth()/2,5*_infos.WindowsHeight()/9+50,CENTER))
,_pDeck(pDeck)
{
    PositionGraphic pos(_infos.WindowsWidth()/2,4*_infos.WindowsHeight()/9-50,CENTER);
    _textAfterBet0.SetPosition(pos);
    pos.Set(_infos.WindowsWidth()/2,4*_infos.WindowsHeight()/9,CENTER);
    _textAfterBet1.SetPosition(pos);
    _bidder = _infos.Next(_infos.Giver());
}

bool Bidding::Bid(GAME_PHASES currentPhase) //TO DO test, test test !
{
    if (currentPhase != BIDDING) return false;
    bool choose = true;
    bool biddingOver = false;
    PLAYER_ID giver = _infos.Giver();

    //char forPrinting[100];
    //sprintf(forPrinting,"the giver is %d",giver);
    //printf(forPrinting);

    Uint i_playerBid = _infos.PosPlayerToInt(_bidder);
    const Player_Bid& bid = _players[i_playerBid]->Take(_bets);
    choose = bid.GetColorBid() != NOT_CHOSEN;
    if (!choose) return false;
    //_infos.NextPlayerBid();
    _bidder = _infos.Next(_bidder);
    handleBet(bid,_bidder); //TO DO move to BetsMemory

    _bets.TellBet(bid);

    if(_bidder == giver)
    {
        Uint firstBiddingPlayer = (giver+1)%4;
        for (Uint i = firstBiddingPlayer; i < firstBiddingPlayer+4; i++)
        {
            _players[i%4]->ResetBid(false);
        }
    }

    biddingOver = _bets.IsBetsOver(_bidder);
    if(biddingOver) return true; //ajust this.
    return false;
}
void Bidding::handleBet(const Player_Bid& bid, PLAYER_ID ibet)
{
    if(bid.GetColorBid() != NO)
    {
        _infos.SetMaxBid(bid.Bid());
        _infos.SetTrumpColor(bid.Color());
        _infos.SetTaker(ibet);
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
    //the first player to play is the player after the giver
    _bidder = _infos.Next(_infos.Giver());

    switch (_infos.TrumpColor()) //setting the displayed objects
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
    for(Uint i = 0; i < 4; ++i)
    {
        _players[i]->InitMemory(); //and sort the hand
    }

    Uint i_start = ((_infos.Giver()+1)%4);
    switch (_infos.TrumpColor())
    {
    case NO : case NOT_CHOSEN:
        for (Uint i = i_start; i < i_start+4; i++)
        {
            _players[i%4]->GiveCardsBack(_pDeck->GetPile());
        }
        return BEGINNING;
    case HEART : case DIAMOND : case SPADE : case CLUB :
        return PLAYING;
    default :
        return BEGINNING;
    }
}
void Bidding::handleText()
{
    string res = _players[_infos.Taker()]->Getname();
    switch (_infos.Taker()%2)
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
    switch(_infos.TrumpColor())
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
    res += Player_Bid_Graphic::IntToString(_infos.TrumpColor(),_infos.MaxBid());
    res += " points.";
    _textAfterBet1.ChangeText(res);
}

string Bidding::GetString()
{
    string res = "<bet>";
    //Uint maxTurn = _infos.BiddingRound();
    Uint firstPlayer = (_infos.Giver()+1)%4;

    Uint betsNum = _bets.GetSize();
    //res += IntToString(betsNum);
    //res += "\t";
    Uint counter = 1;
    //for (Uint turn = 1; turn <= maxTurn+1; ++turn)
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
