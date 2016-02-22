#ifndef TEXT_TYPING_H
#define TEXT_TYPING_H

/**
** This class allows the human player to interact with
** the game with the keyboard.
** It is useful for changing the name of the players at
** the beginning of the game for example.
**/

#include <list>

#include "Basic_Text.h"
#include "Images_Click.h"

	class Text_Typing : public Images_Click, public Basic_Text
	{
	public:
		Text_Typing() {}
#if COMPLETE_GAME > 0
		Text_Typing(std::string path_font, int size, char colR, char colG, char colB, SDL_Event* event);
		Text_Typing(std::string path_font, int size, char colR, char colG, char colB, SDL_Event* event, std::string text);
#endif //#if COMPLETE_GAME > 0
		Text_Typing(std::string path_font, int size, char colR, char colG, char colB);
		Text_Typing(std::string path_font, int size, char colR, char colG, char colB, std::string text);
		virtual ~Text_Typing() {}
		void Update_Text_OnIt();
		void Change_Text(std::string text);
	protected:
		std::string _text_old;

		void act();
	private:
	};
#endif // TEXT_TYPING_H
