#ifndef AIPLAYSCORES_H
#define AIPLAYSCORES_H

#include<list>
#include<array>

#include "Definitions.h"
#include "Cards.h"
#include "Random.h"
#include "TrickStatus.h"
#include "Datas.h"
//#include "AIGameMemory.h"
//TO DO : implement the possibility to have another memory
//TO DO : resoudre les histoires avec les pseudo height, la moins bonne idée du monde..
/** The AI player will play according to a score. This one is define in some csv files**/
//TO : hash table instead of indices in the current file "datas.h"
/*template<POSITION_TRICK>
class WrapperScore
{
    public :
        WrapperScore(POSITION_TRICK pos){};
        Uint operator()(Cards* pcard,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus);
};
*/
class AIPlayScores
{
    protected:
        Basic_Game_Info _infos;
        Uint _type;
        //AIGameMemory *_pPlayerMemory;
        Datas _play_1st;
        Datas _play_2nd;
        Datas _play_3rd;
        Datas _play_4th;
    public:
        AIPlayScores():
            _type(1),
            //_pPlayerMemory(pPlayerMemory),
            _play_1st("datas/_a_eval_score_play_1st.csv",_type),
            _play_2nd("datas/_a_eval_score_play_2nd.csv",_type),
            _play_3rd("datas/_a_eval_score_play_3rd.csv",_type),
            _play_4th("datas/_a_eval_score_play_4th.csv",_type)
            {
            }

        virtual ~AIPlayScores(){}
        std::list<Cards*>::iterator Play(const std::array<Cards*,4>& trick
                                         ,std::list<std::list<Cards*>::iterator>& plyableCards
                                         ,const std::list<Cards*>& hand
                                         ,Random& rand
                                         ,const TrickStatus& trickStatus);
    protected:
        int playFirst(Cards*,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump);
        int playSecond(Cards*,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump);
        int playThird(Cards*,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump);
        int playFourth(Cards*,const std::array<Cards*,4>& trick,const TrickStatus& trickStatus,Random& rand,bool pisse, bool play_trump);
    private:
};

#endif // AIPLAYSCORES_H
