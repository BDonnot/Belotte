#ifndef DATAS_H
#define DATAS_H

/**
** This class handles the input of the score from the files
** on the hard drive, to the usage in the AIPlayScore.
** This old class (not recode since I created it
** for the 2nd year project, has then some defaults.
**/

#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

///Readability :
//TO DO : remove _datas_selected_0, which was done for multiple kind of AI player regarding
//to score, but has no more usage.
//TO DO : specialize among cases :
//1 -> the scores can be update [on the hardrive] (not currently used but why not)
//2 -> the scores cannot be are just read, and won't be change (on the hard drive)
//TO DO : remove the deprecated functions, keep only the necessary, depending
//on the specialization above.
class Datas
{
    protected:
        std::vector<double> _datas;
        std::vector<int> _datas_selected_0;
        std::vector<int> _datas_selected_1;
        std::vector<int> _datas_selected_2;
        std::vector<int> _datas_selected_3;
        int* _selected;

        std::string _path;
        int _number_of_datas;
        int _number_line_concerned;
    public:
        Datas();
        Datas(const std::string& path);
        Datas(const std::string& path,int num_line);
        virtual ~Datas();
        const std::vector<double>& GetDatas() const {return _datas;}
        void Update_gen(int diff_score,int color_trump);
        double Value(int num_colon) const {return _datas[num_colon];}
        double Value_memo(int num_colon); //deprecated

        double Value(int num_line,int color_selected); //deprecated
        void Reset_choices();
    protected :

        std::vector<std::string> splittexte(const std::string& str, const std::string& delim);
        double coeffmul_win(int);
        double coeffadd_win(int);
        double coeffmul_win(int,int);
        double coeffadd_win(int,int);
        static void win(double& data, double coeffmult,double coeffadd);
        static void lose(double& data, double coeffmult,double coeffadd);
        void write();
        std::string write_aux(std::vector<std::string> vedata_load_vect,std::string delim);
        void load();
        std::vector<int>* choose_selected(int color_trump);
        double pow(int exposant,double number);
        std::vector<std::string> convert_to_str();

        //prevent copy and assigment
        Datas& operator=(const Datas& other);
        Datas(const Datas& other);
    private:
};
#endif // DATAS_H
