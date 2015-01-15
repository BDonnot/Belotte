#include "Save_Game.h"
using namespace std;

Save_Game::Save_Game():
_mainFile("datas/games.blt", ios::out | ios::app)
,_temporaryFile("datas/temp.blt", ios::out | ios::trunc)
{
    _prevScores = {0,0};
    _temporaryFile << "<coinche>" <<endl;
}

Save_Game::~Save_Game()
{
    _mainFile.close();
    _temporaryFile.close();
}

void Save_Game::EndGame()
{
    //_mainFile << "toto\n";
    _temporaryFile.close();
    ifstream tempFile("datas/temp.blt", ios::in);
    copy(tempFile,_mainFile);
    _mainFile << "<\\coinche>" << endl;
    tempFile.close();
    _temporaryFile.open("datas/temp.blt", ios::out | ios::trunc);
    _temporaryFile << "<coinche>" << endl;
}

 void Save_Game::Quit()
 {
    _temporaryFile.close();
    ifstream tempFile("datas/temp.blt", ios::in);
    ofstream garbage("datas/unfinishedGames.blt", ios::out | ios::app);
    copy(tempFile,garbage);
    garbage << "<\\coinche>" << endl;
    garbage.close();
    tempFile.close();
    _temporaryFile.open("datas/temp.blt", ios::out | ios::trunc);
 }

void Save_Game::copy(ifstream& origin,ofstream& destination)
{
    string line;
    while(getline(origin, line))
    {
        destination << line << endl;
    }
}
void Save_Game::SaveHands(const std::array<Player*,4>& players)
{
    _temporaryFile << '\t' << "<hands>";
    for(Uint i = 0; i < 4; i++)
    {
         _temporaryFile << players[i]->GetString();
    }
    _temporaryFile << "<\\hands>" << endl;
}
void Save_Game::SaveBet(Bidding& bid)
{
    _temporaryFile << '\t' << bid.GetString() << endl;
}
void Save_Game::SaveTake(Uint playerTake, Uint trumpColor, Uint bid)
{
    _temporaryFile << '\t'<< "<taker>";
    _temporaryFile << IntToString(playerTake);
    _temporaryFile << ',';
    _temporaryFile << IntToString(trumpColor);
    _temporaryFile << ',';
    _temporaryFile << IntToString(bid);
    _temporaryFile << "<\\taker>" << endl;
}
void Save_Game::SaveTrick(const Trick& trick)
{
    _temporaryFile << '\t' << trick.GetString() << endl;
}
void Save_Game::SaveScores(Uint totalScoreFirstTeam,Uint totalScoreSecondTeam)
{

    _temporaryFile << '\t'<< "<scores>";
    _temporaryFile << IntToString(totalScoreFirstTeam - _prevScores[0]);
    _temporaryFile << ',';
    _temporaryFile << IntToString(totalScoreSecondTeam - _prevScores[1]);
    _temporaryFile << "<\\scores>" << endl;
    _prevScores = {totalScoreFirstTeam,totalScoreSecondTeam};
}
