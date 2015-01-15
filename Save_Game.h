#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include <iostream>
#include <fstream>
#include <string>

#include "Definitions.h"

#include "Player.h"
#include "Trick.h"
#include "Bidding.h"
/** The aim of this class is to make sure that the informations
relative to the bets and the trick are written properly, regardless
is the human quit during the game or if other external/internal
events occur.
The idea is to store every information in a temporary file, and if the
game end properly to write it in the main file.**/

class Save_Game
{
    protected :
//        bool _gameOpen;
//        bool _trickOpen;
//        bool _betOpen;
//        std::string _currentTrick;
//        std::string _currentBit;
        std::ofstream _mainFile;
        std::ofstream _temporaryFile;
        std::array<Uint,2> _prevScores;


    public:
        Save_Game();
        virtual ~Save_Game();
        void Quit(); //when the player quit during a game
        void EndGame(); //At the end of a game (tricks)

        void SaveHands(const std::array<Player*,4>& players); //save the hands of the player at when the cards are given
        void SaveBet(Bidding& bid); //save the bet (only call at the end of the bets)
        void SaveTake(Uint playerTake, Uint trumpColor,Uint bid); //save the 'winner' of the bet, and the color of the trump
        void SaveTrick(const Trick& current_trick); //save the trick (only call at the end of the trick)
        void SaveScores(Uint totalScoreFirstTeam,Uint totalScoreSecondTeam);

    protected:
        void copy(std::ifstream& origin,std::ofstream& destination); //copy origin in destination
    private:
};

#endif // SAVE_GAME_H
