#ifndef FONCTIONS_ANNEXES_H_INCLUDED
#define FONCTIONS_ANNEXES_H_INCLUDED

//#include "Carte.h"
#include "Random.h"
int random(int,int);  //to return a aleatory number between 2 boundaries, upper boundarie excluded
int random(int);
//void echange_32(int,int,std::array<Carte*,32>&);
//void echange_5(int,int,std::array<Carte*,5> &);
//void echange_4(int,int,std::array<Carte*,4> &);
//int modulo(int,int); //because sometimes the operator %renvoit return negative number...
//void erase (std::list<Carte*>&,Carte*);
//void tri_carte(std::array<Carte*,5>&,int,std::array<bool,64>); //sort a array by rising value
//void tri_carte_atout(std::array<Carte*,5>&,int);
//void tri_carte_normal(std::array<Carte*,5>&,int);
int sel_i_max(std::array<int,4>);
double pow (int,double);
//to shuffle a list of carte
//void shuffle(std::list<Carte*>& li);
#endif // FONCTIONS_ANNEXES_H_INCLUDED
