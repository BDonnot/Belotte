#include "Carte.h"
#include "fonctions_annexes.h"
#include "Random.h"
using namespace::std;

int random(int bas,int haut)
{
    int res= rand();
    res = res%(haut-bas);
    res+=bas;
    return res;
}
int random(int max)
{
    int res = rand();
    res = ((double) res )/ ((double) RAND_MAX);
    return (int) res*max;
}
void echange_32(int i,int j,array<Carte*,32>& all_cards)
{
    Carte* pcarte;
    pcarte = all_cards[i];
    all_cards.at(i)=all_cards[j];
    all_cards.at(j)=pcarte;
}
void echange_5(int i,int j,array<Carte*,5>& all_cards)
{
    Carte* pcarte;
    pcarte = all_cards[i];
    all_cards.at(i)=all_cards[j];
    all_cards.at(j)=pcarte;
}
void echange_4(int i,int j,array<Carte*,4> &all_cards)
{
    Carte* pcarte;
    pcarte = all_cards[i];
    all_cards.at(i)=all_cards[j];
    all_cards.at(j)=pcarte;
}

int modulo(int p,int q) //p modulo q
{
    if ((0<= p)&&(p<q)) return p;
    else
    {
        if (p < 0) return modulo(p+q,q);
        else return modulo(p-q,q);
    }
}
void erase(list<Carte*>& li, Carte* pcarte)
{
    Carte* pca;
    int n = li.size();
    int comp = 0;
    while (comp < n)
    {
        pca = li.front();
        if ((pca->GetColour()!=pcarte->GetColour())||(pca->GetHeight()!=pcarte->GetHeight()))
        {
            li.push_back(pca);
        }
        li.pop_front();
        comp++;
    }
}
void tri_carte(array<Carte*,5>& my_cards,int nb_cards,array<bool,64> tableau_valeur )
{
    Carte* pcarte1;
    Carte* pcarte2;
    for (int i = 0; i < nb_cards-1; i++)
    {
        for (int j = 0; j < nb_cards; j++)
        {
            pcarte1 = my_cards[j];
            pcarte2 = my_cards[j+1];
            if (tableau_valeur[ (pcarte1->GetHeight())+8*(pcarte2->GetHeight()) ]) echange_5(j,j+1, my_cards);
        };
    };
}
void tri_carte_atout(array<Carte*,5>& my_cards,int nb_cards)
{
    tri_carte(my_cards,nb_cards,Carte::If_Trump());
}

void tri_carte_normal(array<Carte*,5>& my_cards,int nb_cards)
{
    tri_carte(my_cards,nb_cards,Carte::No_Trump());
}
int sel_i_max(array<int,4> scores)
{
    int res = 0;
    int max = scores[0];
    for (int i = 1; i < 4; i++)
    {
        if (scores[i] > max)
        {
            res = i;
            max = scores[i];
        }
    }
    return res;
}
double pow(int exposant,double number)
{
    if (exposant==0) return 1;
    if (exposant==1) return number;
    if (exposant%2==0) return pow(exposant/2,number*number);
    else return number*pow((exposant-1)/2,number*number);
}
 void shuffle(list<Carte*>& li) //quadratic
{
    int n = li.size();
    Random_Decreasing rand(n-1);
    list<Carte*> aux;
    int nb_rand = 0;
    list<Carte*>::iterator it=li.begin();
    for (int i = 0; i < n; i++)
    {
        it = li.begin();
        nb_rand = rand();
        advance(it,nb_rand); //linear
        aux.splice(aux.begin(),aux,it); //constant
    }
    li.swap(aux); //constant
}
