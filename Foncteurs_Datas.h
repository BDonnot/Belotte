#ifndef FONCTEURS_DATAS_H_INCLUDED
#define FONCTEURS_DATAS_H_INCLUDED
#include "Datas.h"
class Update_Stats
{
    public :
        Update_Stats(int diff_score,int trump_color):_diff_score(diff_score),_trump_color(trump_color){}
        void operator()(Datas* data) //update the data properly
        {
            data->update_gen(_diff_score,_trump_color);
        }
    private :
        int _diff_score;
        int _trump_color;

};
class Reset_Stats
{
    public :
        Reset_Stats(){}
        void operator()(Datas* data) //update the data properly
        {
            data->reset_choices();
        }
};
class Obtain_value
{
    public :
        Obtain_value(bool memorize):_memorize(memorize){}
        double operator()(Datas& data,int number_colon_concerned)
        {
            if (_memorize) return data.Value_memo(number_colon_concerned);
            return data.Value(number_colon_concerned);
        }
    private :
        bool _memorize;
};
#endif // FONCTEURS_DATAS_H_INCLUDED
