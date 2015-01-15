#ifndef DATAS_H
#define DATAS_H

#include <vector>
#include <string>

class Datas
{
    public:
        Datas();
        Datas(const std::string& path,int num_datas);
        Datas(const std::string& path,int num_datas,int num_colon);
        virtual ~Datas();
        void update_gen(int diff_score,int color_trump);
        double value(int num_line,int color_selected);
        void reset_choices();
    protected:
        std::vector<double> _datas;
        std::vector<int> _datas_selected_0;
        std::vector<int> _datas_selected_1;
        std::vector<int> _datas_selected_2;
        std::vector<int> _datas_selected_3;
        std::string _path;
        int _number_of_lines;
        int _number_colon_concerned;

        std::vector<std::string> splittexte(const std::string& str, const std::string& delim);
        double coeffmul_win(int);
        double coeffadd_win(int);
        double coeffmul_win(int,int);
        double coeffadd_win(int,int);
        static void win(double& data, double coeffmult,double coeffadd);
        static void lose(double& data, double coeffmult,double coeffadd);
        void write(std::vector<double> datas);
        std::string write_aux(std::vector<std::string> vedata_load_vect,std::string delim);
        std::vector<double> load();
        std::vector<int>* choose_selected(int color_trump);
    private:
};
#endif // DATAS_H
