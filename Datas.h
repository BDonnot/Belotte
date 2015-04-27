#ifndef DATAS_H
#define DATAS_H

#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Datas
{
    public:
        Datas();
        Datas(const std::string& path);
        Datas(const std::string& path,int num_line);
        virtual ~Datas();
        std::vector<double> GetDatas();
        void update_gen(int diff_score,int color_trump);
        double Value(int num_colon);
        double Value_memo(int num_colon);

        double value(int num_line,int color_selected);
        void reset_choices();
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
