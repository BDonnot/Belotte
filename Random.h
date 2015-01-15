#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <ctime>
#include <boost/random.hpp>

class Random
{
    public:
    boost::mt19937 gen;
    boost::uniform_int<int> dst;
    boost::variate_generator< boost::mt19937, boost::uniform_int<int> > rand;
    Random(unsigned int N ):// call instance:
    gen( static_cast<unsigned long>(std::rand()) ), dst( 0, N ), rand(gen,dst) {}
    Random():// call instance:
    gen( static_cast<unsigned long>(std::rand()) ), dst( 0, 1024 ), rand(gen,dst) {}

    Random(int n, int N ):// call instance:
    gen( static_cast<unsigned long>(std::rand()) ), dst( n, N ), rand(gen,dst) {}

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
        int _max;
        boost::mt19937 _gen;
    public:
    Random_Decreasing( int N ):// call instance:
     _max(N),_gen( static_cast<unsigned long>(std::rand()) ){}
    int operator()()
    {
        boost::random::uniform_int_distribution<> dist(0, _max);
        _max--;
        return dist(_gen);
    }
};
#endif // RANDOM_H
