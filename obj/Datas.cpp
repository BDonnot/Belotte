#include "Datas.h"

#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "fonctions_annexes.h"
using namespace std;

Datas::Datas()
{
    //ctor
}
Datas::Datas(const string& path,int num_datas)
{
    _path = path;
    _number_of_lines = num_datas;
    _number_colon_concerned = 0;
    fstream infile;
    infile.open(path.c_str(), fstream::in | fstream::out | fstream::app);
    string data;
    for (int i = 0; i < num_datas; i++)
    {
        infile >> data;
        _datas.push_back(atof(data.c_str()));
    }
    infile.close();
    reset_choices();
}
Datas::Datas(const string& path,int num_datas,int num_colon)
{
    _number_of_lines = num_datas;
    _path = path;
    _number_colon_concerned = num_colon;
    fstream infile;
    infile.open(path.c_str(), fstream::in | fstream::out | fstream::app);
    string data;
    vector<string> at_load;
    for (int i = 0; i < _number_of_lines; i++)
    {
        infile >> data; //load the line
        at_load = splittexte(data,";"); //split the ligne
        _datas.push_back(atof(at_load[_number_colon_concerned].c_str())); //stock the good caracter
    }
    infile.close();
    reset_choices();
}
Datas::~Datas()
{
    //dtor
}
vector<string> Datas::splittexte(const string& str, const string& delim)
{
    vector<string> parts;
    size_t start, end = 0;
    while (end < str.size())
    {
        start = end;
        while (start < str.size() && (delim.find(str[start]) != string::npos))
        {
            start++;  // skip initial whitespace
        }
        end = start;
        while (end < str.size() && (delim.find(str[end]) == string::npos))
        {
            end++; // skip to end of word
        }
        if (end-start != 0)
        {  // just ignore zero-length strings.
            parts.push_back(string(str, start, end-start));
        }
    }
    return parts;
}
void Datas::write(vector<double> datas) //to write new datas (the one in 'datas') in the file.
{
    fstream infile;
    fstream outfile;
    infile.open(_path.c_str(), fstream::in | fstream::out | fstream::app);
    outfile.open("copie.txt", fstream::in | fstream::out | fstream::app);
    string data_load ="";
    vector<string> concerned_line;
    stringstream string_of_double;
    for (int i = 0; i < _number_of_lines; i++)
    {
        //clear string_of_int
        string_of_double.clear();
        string_of_double.str(string());
        //load the line from the file
        infile >> data_load;
        //convert this line to a vector of string
        concerned_line = splittexte(data_load,";");
        //convert the int which must be written in a string
        string_of_double << datas[i];
        //change the value
        concerned_line[_number_colon_concerned] = string_of_double.str();
        //write the line on the new file
        outfile << write_aux(concerned_line,";") << endl;
    }
    //close the file
    infile.close();
    outfile.close();
    //delete the infile
    remove(_path.c_str());
    //rename the new file
    rename("copie.txt",_path.c_str());
}
string Datas::write_aux(vector<string> concerned_line,string delim)
{
    string res = "";
    int number_data_per_line = concerned_line.size();
    for (int i = 0; i < number_data_per_line-1; i++)
    {
        res += concerned_line[i];
        res += delim;
    }
    res+=concerned_line[number_data_per_line-1];
    return res;
}
double Datas::coeffmul_win(int diff_score,int nb_cards)
{
    return pow(nb_cards,coeffmul_win(diff_score));
}
double Datas::coeffmul_win(int diff_score)
{
    if (diff_score<30) return 1.01;
    if (diff_score<60) return 1.02;
    if (diff_score<90) return 1.03;
    if (diff_score<120) return 1.04;
    if (diff_score<150) return 1.05;
    if (diff_score<190) return 1.07;
    return 1.1;
}
double Datas::coeffadd_win(int diff_score,int nb_cards)
{
    return nb_cards*coeffadd_win(diff_score);
}
double Datas::coeffadd_win(int diff_score)
{
    if (diff_score<30) return 0.2;
    if (diff_score<60) return 0.3;
    if (diff_score<90) return 0.4;
    if (diff_score<120) return 0.5;
    if (diff_score<150) return 0.6;
    if (diff_score<190) return 0.8;
    return 1;
}
void Datas::update_gen(int diff_score,int color_trump) //gen for 'generic'
{
    int has_won = (diff_score >= 0);
    double coeffmult = coeffmul_win(diff_score);
    double coeffadd = coeffadd_win(diff_score);
    //to chose the right fonction, depends on if we won or if we lose
    void (*f) (double&,double,double);
    if (has_won) f = *win;
    else f = *lose;
    //to choose the right 'selected case'
    vector<int>* pcases_selected=choose_selected(color_trump);
    //int size = pcases_selected->size();
    vector<double> data=load();
    for (int i=0;i<_number_of_lines;i++)
    {
        coeffmult = coeffmul_win(diff_score,(*pcases_selected)[i]);
        coeffadd = coeffadd_win(diff_score,(*pcases_selected)[i]);
        f(data[i],coeffmult,coeffadd);
    }
    write(data);
}
void Datas::win(double& data, double coeffmult,double coeffadd)
{
    if (data >0 ) data = data*coeffmult+coeffadd;
    else
    {
        double diff = coeffadd+(1.-coeffmult)*data;
        data += diff;
    }
}
void Datas::lose(double& data, double coeffmult,double coeffadd)
{
    if (data > 0) data = (data-coeffadd)*1./coeffmult;
    else
    {
        double diff =  -coeffadd/coeffmult+(1.-1./coeffmult)*data;
        data += diff;
    }
}
vector<double> Datas::load()
{
    fstream infile;
    vector<double> res;
    infile.open(_path.c_str(), fstream::in | fstream::out | fstream::app);
    string concerned_line_str;
    vector<string> concerned_line_vect;
    for (int i = 0; i < _number_of_lines; i++)
    {
        infile >> concerned_line_str; //load the line
        concerned_line_vect = splittexte(concerned_line_str,";"); //split the ligne
        res.push_back(atof(concerned_line_vect[_number_colon_concerned].c_str())); //stock the good caracter
    }
    infile.close();
    return res;
}
double Datas::value(int num_line,int color_selected)
{
    vector<int>* p_case_selected = choose_selected(color_selected);
    (*p_case_selected)[num_line] += 1;
    return _datas[num_line];
}
vector<int>* Datas::choose_selected(int color_trump)
{
    switch(color_trump)
    {
        case 0 : return &_datas_selected_0;
        case 1 : return &_datas_selected_1;
        case 2 : return &_datas_selected_2;
        case 3 : return &_datas_selected_3;
        default : return NULL;
    }
}
void Datas::reset_choices()
{
    vector<int> nul(_number_of_lines,0);
    _datas_selected_0 = nul;
    _datas_selected_1 = nul;
    _datas_selected_2 = nul;
    _datas_selected_3 = nul;
}
