#ifndef IMAGES_TRANSPARENCY_H
#define IMAGES_TRANSPARENCY_H

/**
** This class define an specific type of image :
** an image with transparency.
**/

#include "Basic_Images.h"
#include "Basic_Transparency.h"

namespace SDL
{
	class Images_Transparency : public Basic_Images, public Basic_Transparency
	{
	public:
		Images_Transparency() {}
		Images_Transparency(std::string path) :Basic_Images(path), Basic_Transparency() {} //by default : the image is totally visible
		Images_Transparency(std::string path, Uint16 transparency) :Basic_Images(path), Basic_Transparency(transparency) {}
		virtual ~Images_Transparency() {}
	protected:
#if COMPLETE_GAME > 0
		virtual void act(); //overriding of the method act : ajust transparency
#else
		virtual void act() {} //overriding of the method act : ajust transparency
#endif //#if COMPLETE_GAME > 0
	private:
	};
}
#endif // IMAGES_TRANSPARENCY_H
