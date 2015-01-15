#ifndef NOMS_CARTES_H_INCLUDED
#define NOMS_CARTES_H_INCLUDED

#include <string>
#include <array>

int CARREAU=0;
int COEUR=1;
int PIQUE = 2;
int TREFLE=3;
int SEPT=0;
int HUIT=1;
int NEUF=2;
int DIX=3;
int VALET=4;
int DAME=5;
int ROI=6;
int AS=7;

std::array<std::string,4> Name_Colour = {"Carreau","Coeur","Pique","Trefle"};
std::array<std::string,8> Name_Height = {"Sept","Huit","Neuf","Dix","Valet","Dame","Roi","As"};

std::array<int,8> Value_Trump = {0,0,14,10,20,3,4,11};
std::array<int,8> Value_No_Trump={0,0,0,10,2,3,4,11};

#endif // NOMS_CARTES_H_INCLUDED
