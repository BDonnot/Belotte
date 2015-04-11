#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#define LAG_MIN 100 //lag min between two bets (for human)
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define MINBET 79 //we force the bid to be strictly above 79

#include<string>
#include<ostream>
#include<sstream>
#include<utility> //pair

typedef unsigned int Uint;
typedef std::pair<Uint,Uint> IntIntPair;

enum POSITION_OPTION{CENTER=0
                    ,TOP_LEFT //y_x
                    ,TOP_CENTER
                    ,TOP_RIGHT
                    ,CENTER_RIGHT
                    ,BOTTOM_RIGHT
                    ,BOTTOM_CENTER
                    ,BOTTOM_LEFT
                    ,CENTER_LEFT};
enum GAME_PHASES {BEGINNING,SELECT_NAMES,GIVING,PREBET,BIDDING,AFTER_BET,PLAYING,AFTER_TRICK0,AFTER_TRICK1,SCORES,EXIT};
enum OUTPUT_BEGIN {PLAY,OPTION,QUIT,CONTINUE};
enum OPTION_OUTPUT{OPTION_QUIT,OPTION_RETURN,OPTION_CONTINUE};

#include "CARDS_HEIGHT.h"
#include "CARDS_COLOR.h"
#include "POSITION_TRICK.h"
#include "PLAYER_ID.h"



inline std::string IntToString(Uint number)
{
    std::ostringstream convert;
    convert << number;
    return convert.str();
}

#endif // DEFINITIONS_H_INCLUDED
