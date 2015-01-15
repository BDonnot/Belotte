#ifndef POSITIONGRAPHIC_H
#define POSITIONGRAPHIC_H
#include <array>
#include "Definitions.h"

class PositionGraphic
{
    protected :
        Uint _pos_x;
        Uint _pos_y;
        POSITION_OPTION _opt;

    public:
        PositionGraphic();
        PositionGraphic(Uint x, Uint y, POSITION_OPTION opt);
        const Uint& Getx() const;
        const Uint& Gety() const;
        Uint& Getx();
        Uint& Gety();
        const POSITION_OPTION& GetOpt() const;

        void Set(Uint x, Uint y, POSITION_OPTION opt);
        void Set(const PositionGraphic& pos);
        void Setx(Uint x);
        void Sety(Uint y);
        void SetOpt(POSITION_OPTION opt);

        PositionGraphic Shift(Uint number,const std::array<Uint,2>& pos) const;

        bool operator==(const PositionGraphic& pos) const;

        void SetProperPosition(Uint width, Uint height);

        virtual ~PositionGraphic();
    protected:
    private:
};

#endif // POSITIONGRAPHIC_H
