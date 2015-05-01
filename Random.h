#ifndef RANDOM_H
#define RANDOM_H

/**
** It creates the classes for simulating pseudo random number
** generator.
** Depends on Boost, which can be avoided thank to feature of c++11.
**/

#include <iostream>
#include <ctime>
#include <boost/random.hpp>

//Debugging
#include "DebugwithPrint.h"
#define RANDOM_DEBUG 0

///TO DO : remove boost dependency
class Random
{
    protected :
        WrapperPrint<RANDOM_DEBUG> _printf;
        int _maxRand;
        boost::mt19937 gen;
        boost::uniform_int<int> dst;
        boost::variate_generator< boost::mt19937, boost::uniform_int<int> > rand;
    public:
        Random(unsigned int N ):// call instance:
            _maxRand(N),gen( static_cast<unsigned long>(std::rand()) ), dst( 0, N ), rand(gen,dst) {_printf("constructor 1 called\n");}
        Random():// call instance:
            _maxRand(1023),gen( static_cast<unsigned long>(std::rand()) ), dst( 0, 1023 ), rand(gen,dst) {_printf("constructor 2 called\n");}

        Random(int n, int N ):// call instance:
            _maxRand(N),gen( static_cast<unsigned long>(std::rand()) ), dst( n, N ), rand(gen,dst) {_printf("constructor 3 called\n");}

        std::ptrdiff_t operator()( std::ptrdiff_t arg )
        {
            return static_cast< std::ptrdiff_t >( rand() );
        }
        int generate_number()
        {
            return rand();
        }
};
class Random_Decreasing
{
    private:
        WrapperPrint<RANDOM_DEBUG> _printf;
        int _max;
        boost::mt19937 _gen;
    public:
        Random_Decreasing( int N ):// call instance:
         _max(N),_gen( static_cast<unsigned long>(std::rand()) ){_printf("constructor 4 called\n");}
        int operator()()
        {
            boost::random::uniform_int_distribution<> dist(0, _max);
            _max--;
            return dist(_gen);
        }
};
#endif // RANDOM_H
