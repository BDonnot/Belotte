#ifndef POSITIONGRAPHIC_H
#define POSITIONGRAPHIC_H

/**
** This class serve only to display at the right place
** images on the screen.
**/

#include <array>

#include "Definitions.h"
	class PositionGraphic
	{
	protected:
		Uint _pos_x;
		Uint _pos_y;
		POSITION_OPTION _opt;

	public:
		PositionGraphic() :_pos_x(0), _pos_y(0) {}
		PositionGraphic(Uint x, Uint y, POSITION_OPTION opt) :_pos_x(x), _pos_y(y), _opt(opt) {}
		const Uint& Getx() const { return _pos_x; }
		const Uint& Gety() const { return _pos_y; }
		Uint& Getx() { return _pos_x; }
		Uint& Gety() { return _pos_y; }

		const POSITION_OPTION& GetOpt() const { return _opt; }

		void Set(Uint x, Uint y, POSITION_OPTION opt);
		void Set(const PositionGraphic& pos);
		void Setx(Uint x) { _pos_x = x; }
		void Sety(Uint y) { _pos_y = y; }
		void SetOpt(POSITION_OPTION opt) { _opt = opt; }

		PositionGraphic Shift(Uint number, const std::array<Uint, 2>& pos) const;

		bool operator==(const PositionGraphic& pos) const { return (_pos_x == pos._pos_x) && (_pos_y == pos._pos_y); }

		void SetProperPosition(Uint width, Uint height);

		virtual ~PositionGraphic() {}
	protected:
	private:
	};
#endif // POSITIONGRAPHIC_H
