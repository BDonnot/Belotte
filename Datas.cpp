#include "Datas.h"
using namespace std;

Datas::Datas()
{
    _selected = new int[1];
}
Datas::Datas(const string& path)
{
    _path = path;
    _number_line_concerned = 0;
    load();
    _number_of_datas = _datas.size();
    vector<int> nul(_number_of_datas,0);
    _datas_selected_0 = nul;
    _datas_selected_1 = nul;
    _datas_selected_2 = nul;
    _datas_selected_3 = nul;

    _selected = new int[_number_of_datas];
    //reset_choices();
}
vector<double> Datas::GetDatas()
{
    return _datas;
}
Datas::Datas(const string& path,int num_line)
{
    _path = path;
    _number_line_concerned = num_line;
    load();
    _number_of_datas = _datas.size();
    vector<int> nul(_number_of_datas,0);
    _datas_selected_0 = nul;
    _datas_selected_1 = nul;
    _datas_selected_2 = nul;
    _datas_selected_3 = nul;
    _selected = new int[_number_of_datas];
}
Datas::~Datas()
{
    //dtor
    delete [] _selected;
}
void Datas::reset_choices()
{
    /*vector<int> nul(_number_of_datas,0);
    _datas_selected_0 = nul;
    _datas_selected_1 = nul;
    _datas_selected_2 = nul;
    _datas_selected_3 = nul;*/
    for (int i = 0; i < _number_of_datas;i++)
    {
        _datas_selected_0[i] = 0;
        _datas_selected_1[i] = 0;
        _datas_selected_2[i] = 0;
        _datas_selected_3[i] = 0;
        _selected[i] = 0;
    }

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
vector<string> Datas::convert_to_str()
{
    vector<string> res;
    stringstream string_of_double;
    for (vector<double>::iterator it = _datas.begin(); it != _datas.end(); ++it)
    {
        string_of_double.clear();
        string_of_double.str(string());
        string_of_double << *it;
        res.push_back(string_of_double.str());
    }
    return res;
}
void Datas::write() //to write new datas (the one in 'datas') in the file.
{
    fstream infile;
    fstream outfile;
    infile.open(_path.c_str(), fstream::in | fstream::out | fstream::app);
    outfile.open("copie.txt", fstream::in | fstream::out | fstream::app);
    string data_load ="";
    vector<string> concerned_line = convert_to_str();
    for (int i = 0; i < _number_line_concerned; i++) //copy the first lines of the file
    {
        getline(infile,data_load);
        outfile << data_load <<endl;
    }
    getline(infile,data_load); //move to the line to change
    outfile << write_aux(concerned_line,";") <<endl;
    while (!infile.eof())
    {
        getline(infile,data_load);
        //outfile << data_load <<endl;
        outfile << data_load ;
        if (!infile.eof()) outfile << endl;
        //else outfile << data_load ;
    }
    //close the files
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
    //int number_data_per_line = concerned_line.size();
    for (int i = 0; i < _number_of_datas-1; i++)
    {
        res += concerned_line[i];
        res += delim;
    }
    res+=concerned_line[_number_of_datas-1];
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
    load();
    for (int i=0;i<_number_of_datas;i++)
    {
        coeffmult = coeffmul_win(diff_score,(*pcases_selected)[i]);
        coeffadd = coeffadd_win(diff_score,(*pcases_selected)[i]);
        f(_datas[i],coeffmult,coeffadd);
    }
    write();
}
void Datas::win(double& data, double coeffmult,double coeffadd)
{
    if (data >=0 ) data = data*coeffmult+coeffadd;
    else
    {
        double diff = coeffadd+(coeffmult-1.)*data;
        data += diff;
    }
}
void Datas::lose(double& data, double coeffmult,double coeffadd)
{
    if (data >= 0) data = (data-coeffadd)*1./coeffmult;
    else
    {
        double diff =  -coeffadd/coeffmult+(1.-1./coeffmult)*data;
        data += diff;
    }
}
void Datas::load()
{
    string data;
    vector<string> split_data;
    fstream infile;
    infile.open(_path.c_str(), fstream::in | fstream::out | fstream::app);
    for (int i = 0; i < _number_line_concerned+1;i++)
    {
        getline(infile,data);
    }
    split_data = splittexte(data,";");
    for (vector<string>::iterator it = split_data.begin(); it != split_data.end();++it)
    {
        _datas.push_back(atof((*it).c_str()));
    }
    infile.close();
}
double Datas::value(int num_col,int color_selected)
{
    if (color_selected != -1)
    {
        vector<int>* p_case_selected = choose_selected(color_selected);
        (*p_case_selected)[num_col] += 1;
    }
    return _datas[num_col];
}
double Datas::Value(int num_colon)
{
   return _datas[num_colon];
}
double Datas::Value_memo(int num_colon)
{
    _selected[num_colon]+=1;
    return _datas[num_colon];
}
vector<int>* Datas::choose_selected(int color_selected)
{
    switch(color_selected)
    {
        case 0 : return &_datas_selected_0;
        case 1 : return &_datas_selected_1;
        case 2 : return &_datas_selected_2;
        case 3 : return &_datas_selected_3;
        default : return NULL;
    }
}
double Datas::pow(int exposant,double number)
{
    if (exposant==0) return 1;
    if (exposant==1) return number;
    if (exposant%2==0) return pow(exposant/2,number*number);
    else return number*pow((exposant-1)/2,number*number);
}
