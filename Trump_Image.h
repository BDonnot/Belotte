#ifndef TRUMP_IMAGE_H
#define TRUMP_IMAGE_H
/**
** It is the image of the trump (of the set) in the right
** corner of the windows with the name of the taker.
** if the mouse is over, it displays the last trick played.
** This functionality is handle by trick.
**/

#include "Basic_Click.h"
#include "Images_Text.h"
#include "Images_Sprite.h"
#include "Player_Name.h"

	class Trump_Image : public Images_Sprite<4>, public Basic_Click
	{
	protected:
		int _trunk;
		int _taker;
	public:
		Trump_Image() {}
#if COMPLETE_GAME > 0
		Trump_Image(SDL_Event* event);
#endif //#if COMPLETE_GAME > 0
		void Set_Taker(int taker);
		virtual ~Trump_Image() {}
	protected:
		void Update_on_it();
	private:
		Trump_Image(const Trump_Image& other) {}
		Trump_Image& operator=(const Trump_Image& other) { return *this; }
	};
#endif // TRUMP_IMAGE_H
