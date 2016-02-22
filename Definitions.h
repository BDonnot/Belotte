#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

/**
** This header define the project properties
** as well as some usefull classes
** and types
** It is include in every header of the project
**/

//Definition of some SDL constant :
#define SCREEN_BPP 32
#define FRAMES_PER_SECOND_FOR_ME 32
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

//Definition of some constants of the game
#define LAG_MIN 100 //lag min between two bets (for human)
#define MINBET 79 //we force the bid to be strictly above 79

//Definition of some constant, used for simulation and testing
#define NBGAMEMAX  3000
#define TEST_PLAY_GAME 0
#define PLAY_HUMAN 1
#define MULTITHREAD_GRAPHIC 1
#define USE_BOOST 0 //EXPERIMENTAL : if 0; remove boost dependency and use stl instead

#ifndef COMPLETE_GAME
	#define COMPLETE_GAME 1 // COMPLETE_GAME 0 : Does not require SDL to compile and / or play . Useful to launch benchmarks
#endif //ifndef COMPLETE_GAME

#if COMPLETE_GAME == 0
	typedef unsigned short Uint8;
	typedef unsigned int Uint16;
	typedef unsigned long Uint32;
	#define ALPHA_TRANSPARENT 0 
	#define ALPHA_OPAQUE 255 
#else
	#define ALPHA_TRANSPARENT SDL_ALPHA_TRANSPARENT
	#define ALPHA_OPAQUE SDL_ALPHA_OPAQUE
#endif

#include<string>
#include<ostream>
#include<sstream>
#include<utility> //used for pairs

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

//Definition of some basic classes
#include "CARDS_HEIGHT.h"
#include "CARDS_COLOR.h"
#include "POSITION_TRICK.h"
#include "PLAYER_ID.h"

//For converting int to string, for the displaying for example.
inline std::string IntToString(Uint number)
{
    std::ostringstream convert;
    convert << number;
    return convert.str();
}

#endif // DEFINITIONS_H_INCLUDED
