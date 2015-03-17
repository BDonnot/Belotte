#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#define LAG_MIN 100 //lag min between two bets (for human)
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define MINBET 79 //we force the bid to be strictly above 79

#include<string>
#include<ostream>
#include<sstream>

typedef unsigned int Uint;


enum POSITION_OPTION{CENTER=0
                    ,TOP_LEFT //y_x
                    ,TOP_CENTER
                    ,TOP_RIGHT
                    ,CENTER_RIGHT
                    ,BOTTOM_RIGHT
                    ,BOTTOM_CENTER
                    ,BOTTOM_LEFT
                    ,CENTER_LEFT};

enum CARDS_HEIGHT {SEVEN=0,EIGHT,NINE,TEN,JACK,QUEEN,KING,ACE,UNINTIALIZED};
enum CARDS_COLOR {DIAMOND=0,HEART,SPADE,CLUB,ALL_TRUNK,NO_TRUNK,NO,NOT_CHOSEN}; //carreau,coeur,pique,trefle, non je ne prends pas,je n'ai pas choisi
enum GAME_PHASES {BEGINNING,SELECT_NAMES,GIVING,PREBET,BIDDING,AFTER_BET,PLAYING,AFTER_TRICK0,AFTER_TRICK1,SCORES,EXIT};
enum OUTPUT_BEGIN {PLAY,OPTION,QUIT,CONTINUE};
enum OPTION_OUTPUT{OPTION_QUIT,OPTION_RETURN,OPTION_CONTINUE};
enum POSITION_TRICK {FIRST = 0,SECOND,THIRD,FOURTH,UNKNOWN};
enum PLAYER_ID {PLAYER0,PLAYER1,PLAYER2,PLAYER3,GHOST};


inline std::string IntToString(Uint number)
{
    std::ostringstream convert;
    convert << number;
    return convert.str();
}

#endif // DEFINITIONS_H_INCLUDED
