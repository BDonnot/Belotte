#ifndef IMAGES_CLICK_H
#define IMAGES_CLICK_H

/**
** This class define an specific type of image :
** an image we can click on
**/

#include "Basic_Images.h"
#include "Basic_Click.h"

namespace SDL
{
	class Images_Click : public Basic_Images, public Basic_Click
	{
	public:
		Images_Click() {}
		Images_Click(std::string path, SDL_Event* event) :Basic_Images(path), Basic_Click(event) {}
		virtual ~Images_Click() {}
		virtual void Update_on_it(); //true if the mouse is on it
	protected:

	private:
	};
};
#endif // IMAGES_CLICK_H
