#include "Game_Coinche.h"

using namespace std;

Game_Coinche::Game_Coinche(Uint16 screenWidth, Uint16 screenHeight) :
	Quit()
	, _infos()
	, _backSide("images/tapis_moyen_2.jpg")
	, _begin(&_backSide, screenWidth, screenHeight)
	, _end(&_backSide, screenWidth, screenHeight, "Merci d'avoir joué !")
	, _error(&_backSide, screenWidth, screenHeight, "An unexpected error occurred :-(")
	, _deck(screenWidth, screenHeight)
	, _currentPhase(BEGINNING)
	, _bid(array<Player*, 4>(), &_deck)
	, _timeNextAction(0)
	, _trick(&_deck, array<Player*, 4>(), screenWidth, screenHeight)
	, _endGame(screenWidth, screenHeight)
	, _saveGame()
{
	_nbGame = 0;
	PositionGraphic pos(0, 0, TOP_LEFT);
	_backSide.SetPosition(pos);
	PLAYER_ID posPlayer[4] = { PLAYER0,PLAYER1,PLAYER2,PLAYER3 };
	for (Uint i = 0; i < 4; i++)
	{
		_players[i] = static_cast<Player*>(new AIMCRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, &_backSide));

		//if (i % 2 == 0) //AIMCScores
		// _players[i] = static_cast<Player*>(new AIMCScores(Player_ID(posPlayer[i]), screenWidth, screenHeight, &_backSide));
		//else //AIMCRandom
		// _players[i] = static_cast<Player*>(new AIMCRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, &_backSide));

		//if (i % 2 == 0)
		//_players[i] = static_cast<Player*>(new AIMCRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, &_backSide));
		//else
		//_players[i] = static_cast<Player*>(new AIScores(Player_ID(posPlayer[i]), screenWidth, screenHeight, &_backSide));

		//if (i % 2 == 0)
		//    _players[i] = static_cast<Player*>(new AIScores(Player_ID(posPlayer[i]), screenWidth, screenHeight, &_backSide));
		//else
		//    _players[i] = static_cast<Player*>(new AIRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, &_backSide));
	}
	_bid.SetPlayers(_players);
	_trick.SetPlayers(_players);
}

#if COMPLETE_GAME > 0
Game_Coinche::Game_Coinche(SDL_Surface* screen,Uint16 screenWidth, Uint16 screenHeight):
    Quit()
    ,_infos()
    ,_pScreen(screen)
	,_backSide("images/tapis_moyen_2.jpg")
    ,_begin(&_backSide,_event,screenWidth,screenHeight)
    ,_end(&_backSide,_event,screenWidth,screenHeight,"Merci d'avoir joué !")
    ,_error(&_backSide,_event,screenWidth,screenHeight,"An unexpected error occurred :-(")
    ,_deck(_event,_pScreen,screenWidth,screenHeight)
    ,_currentPhase(BEGINNING)
    ,_bid(array<Player*,4>(),_event,_pScreen,&_deck)
    ,_timeNextAction(0)
    ,_trick(&_deck,array<Player*,4>(),screenWidth,screenHeight,_event,_pScreen)
    ,_endGame(_event,_pScreen,screenWidth,screenHeight)
    ,_saveGame()
{
    _nbGame = 0;
    PositionGraphic pos(0,0,TOP_LEFT);
    _backSide.SetPosition(pos);
    PLAYER_ID posPlayer[4] = {PLAYER0,PLAYER1,PLAYER2,PLAYER3};
#if PLAY_HUMAN > 0
    _players[0] = static_cast<Player*>(new Player_Human(Player_ID(posPlayer[0]),screenWidth,screenHeight,_event,&_backSide,_pScreen));
    for (Uint i = 1; i < 4; i++)
#else
	for (Uint i = 0; i < 4; i++)
#endif
    {
#if PLAY_HUMAN > 0
        _players[i] = static_cast<Player*>(new AIMCRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));
        //_players[i] = static_cast<Player*>(new AIScores(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));
#else
		_players[i] = static_cast<Player*>(new AIMCRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));

		//if (i % 2 == 0) //AIMCScores
           // _players[i] = static_cast<Player*>(new AIMCScores(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));
		//else //AIMCRandom
           // _players[i] = static_cast<Player*>(new AIMCRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));

		//if (i % 2 == 0)
            //_players[i] = static_cast<Player*>(new AIMCRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));
		//else
            //_players[i] = static_cast<Player*>(new AIScores(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));

		//if (i % 2 == 0)
        //    _players[i] = static_cast<Player*>(new AIScores(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));
        //else
        //    _players[i] = static_cast<Player*>(new AIRandom(Player_ID(posPlayer[i]), screenWidth, screenHeight, _event, &_backSide, _pScreen));
#endif
    }
    _bid.SetPlayers(_players);
    _trick.SetPlayers(_players);
}
void Game_Coinche::Display()
{
	_backSide.Display(_pScreen);
	for (Uint i = 0; i < 4; i++)
	{
		_players[i]->Display(_currentPhase);
	}
	_deck.Display(_currentPhase);
	_bid.Display(_currentPhase);
	_trick.Display(_currentPhase);
	_endGame.Display(_currentPhase);
}
void Game_Coinche::updateEvent(bool& keep_playing)
{
	while (SDL_PollEvent(_event))
	{
		_begin.Update();
		_deck.UpdateEvent(_currentPhase);
		_trick.UpdateEvent(_currentPhase);
		for (Uint i = 0; i < 4; i++)
		{
			_players[i]->Update_Mouse(_currentPhase);
		}
		keep_playing = keep_playing && KeepLooping();
		_bid.UpdateEvent(_currentPhase);
		_endGame.UpdateEvent(_currentPhase);
	}
}
bool Game_Coinche::waitForClick()
{
	while (true)
	{
		while (SDL_PollEvent(_event))
		{
			if (_event->type == SDL_KEYDOWN)
			{
				if (_event->key.keysym.sym == SDLK_SPACE) return true;
			}
			if (!KeepLooping())
				return false;
		}
	}
}
void Game_Coinche::playGame(bool& keep_playing)
{
	if (_infos.Time() < _timeNextAction) return;
	switch (_currentPhase)
	{
	case BEGINNING:
#if PLAY_HUMAN > 0
		switch (_begin.Display(_pScreen))
		{
		case QUIT:
			keep_playing = false;
			return;
		case PLAY:
			_currentPhase = (_deck.FirstGame()) ? SELECT_NAMES : GIVING;
			_deck.BeginGame();
			return;
		default:
			return;
		}
#else
		_currentPhase = GIVING;
#endif
		return;
	case SELECT_NAMES: case GIVING:
#if PLAY_HUMAN > 0
		if (_deck.Click(_currentPhase))
		{
			_deck.Reset();
			_currentPhase = PREBET;
		}
#else
		_deck.Reset();
		_currentPhase = PREBET;
#endif
		return;
	case PREBET:
		_deck.GiveCards(_players);
		_saveGame.SaveHands(_players);
		_trick.Reset();
#if PLAY_HUMAN > 0
		_timeNextAction = _infos.Time() + 2000; //HERE
#endif
		_currentPhase = BIDDING;
		_bid.Reset();
		return;
	case BIDDING:
		if (_bid.Bid(_currentPhase))
		{
			_saveGame.SaveBet(_bid);
			_currentPhase = AFTER_BET;
			_bid.SummarizeBet();
		}
		return;
	case AFTER_BET:
#if PLAY_HUMAN > 0
		if (_bid.Click(true))
#else
		if (true)
#endif
		{
			_currentPhase = _bid.NextPhase();
			if (_currentPhase == PLAYING)
			{
				_saveGame.SaveTake(_infos.Taker().ToInt(), _infos.TrumpColor().ToInt(), _infos.MaxBid());
				_trick.Update();
			}
			else _saveGame.EndGame();
		}
		return;
	case PLAYING:
		_printf("playing trick %d\n", _trick.TrickNumber());
		if (_trick.Play()) _currentPhase = AFTER_TRICK0; //HERE : changer dans trickFinished
		return;
	case AFTER_TRICK0:
		_printf("AFTER_TRICK0 %d \n", _trick.TrickNumber());
		_saveGame.SaveTrick(_trick);
		_trick.GatherCards();
#if PLAY_HUMAN > 0
		_timeNextAction = _infos.Time() + 600;//HERE //TO DO : improve here ...
#endif
		_currentPhase = AFTER_TRICK1;
		return;
	case AFTER_TRICK1:
		_printf("AFTER_TRICK1 %d \n", _trick.TrickNumber());
		_trick.WinnerTakeCards();
		_currentPhase = PLAYING;
		if (_trick.TrickNumber() == 8)
		{
			_printf("game over\n");
			_endGame.Update();
			const array<Uint, 2>& scores = _infos.FinalScores();
			_saveGame.SaveScores(scores[0], scores[1]);
			IntIntPair score = _trick.CurrentScores();
			printf("game %d, score 0 : %d, score 1 : %d\n", _nbGame, score.first, score.second);
			_nbGame++;
			if (_nbGame >= NBGAMEMAX)
				keep_playing = false;
			_currentPhase = SCORES;
		}
		return;
	case SCORES:
		_printf("Should be displaying scores\n");
		_currentPhase = _endGame.Next(); //HERE (inside function)
		if (_currentPhase != SCORES)
		{
			_saveGame.EndGame();
		}
		return;
	default:
		return;
	}
}
void Game_Coinche::Play()
{
	bool keep_playing = true;
	Uint32 startLoop = 0, endLoop = 0;
	Uint32 min_time_loop = static_cast<Uint32>(1000. / FRAMES_PER_SECOND_FOR_ME);
	_timeNextAction = 0;
	bool error = false;
	while (keep_playing)
	{
		updateTime(endLoop - startLoop);
		try
		{
			if (error) throw 0;
			startLoop = SDL_GetTicks();
			updateEvent(keep_playing);
			Display();
			playGame(keep_playing);
			keep_playing = keep_playing && _currentPhase != EXIT;
		}
		catch (...)
		{
			error = true;
			keep_playing = false;
		}
		if (!keep_playing)
		{
			if (error) _error.Display(_pScreen);
#if PLAY_HUMAN > 0
			else _end.Display(_pScreen);
#endif
		}
		endLoop = SDL_GetTicks();
#if PLAY_HUMAN > 0
		if (endLoop < min_time_loop + startLoop) SDL_Delay((min_time_loop + startLoop) - endLoop);
#endif
		SDL_Flip(_pScreen);
		endLoop = SDL_GetTicks();
	}
	_saveGame.Quit();
}
#else 
void Game_Coinche::playGame(bool& keep_playing)
{
	switch (_currentPhase)
	{
	case BEGINNING:
		_currentPhase = GIVING;
		return;
	case SELECT_NAMES: case GIVING:
		_deck.Reset();
		_currentPhase = PREBET;
		return;
	case PREBET:
		_deck.GiveCards(_players);
		_saveGame.SaveHands(_players);
		_trick.Reset();
		_currentPhase = BIDDING;
		_bid.Reset();
		return;
	case BIDDING:
		if (_bid.Bid(_currentPhase))
		{
			_saveGame.SaveBet(_bid);
			_currentPhase = AFTER_BET;
			_bid.SummarizeBet();
		}
		return;
	case AFTER_BET:
		_currentPhase = _bid.NextPhase();
		if (_currentPhase == PLAYING)
		{
			_saveGame.SaveTake(_infos.Taker().ToInt(), _infos.TrumpColor().ToInt(), _infos.MaxBid());
			_trick.Update();
		}
		else _saveGame.EndGame();
		return;
	case PLAYING:
		_printf("playing trick %d\n", _trick.TrickNumber());
		if (_trick.Play()) _currentPhase = AFTER_TRICK0; //HERE : changer dans trickFinished
		return;
	case AFTER_TRICK0:
		_printf("AFTER_TRICK0 %d \n", _trick.TrickNumber());
		_saveGame.SaveTrick(_trick);
		_trick.GatherCards();
		_currentPhase = AFTER_TRICK1;
		return;
	case AFTER_TRICK1:
		_printf("AFTER_TRICK1 %d \n", _trick.TrickNumber());
		_trick.WinnerTakeCards();
		_currentPhase = PLAYING;
		if (_trick.TrickNumber() == 8)
		{
			_printf("game over\n");
			_endGame.Update();
			const array<Uint, 2>& scores = _infos.FinalScores();
			_saveGame.SaveScores(scores[0], scores[1]);
			IntIntPair score = _trick.CurrentScores();
			printf("game %d, score 0 : %d, score 1 : %d\n", _nbGame, score.first, score.second);
			_nbGame++;
			if (_nbGame >= NBGAMEMAX)
				keep_playing = false;
			_currentPhase = SCORES;
		}
		return;
	case SCORES:
		_printf("Should be displaying scores\n");
		_currentPhase = _endGame.Next(); //HERE (inside function)
		if (_currentPhase != SCORES)
		{
			_saveGame.EndGame();
		}
		return;
	default:
		return;
	}
}
void Game_Coinche::Play()
{
	bool keep_playing = true;
	Uint32 startLoop = 0, endLoop = 0;
	Uint32 min_time_loop = static_cast<Uint32>(1000. / FRAMES_PER_SECOND_FOR_ME);
	_timeNextAction = 0;
	bool error = false;
	updateTime(endLoop - startLoop);
	if (error) throw 0;
	updateEvent(keep_playing);
	Display();
	playGame(keep_playing);
	keep_playing = keep_playing && _currentPhase != EXIT;
	_saveGame.Quit();
}
#endif //#if COMPLETE_GAME > 0

Game_Coinche::~Game_Coinche()
{
    for (Uint i = 0; i < 4; i++)
    {
        delete _players[i];
    }
}
void Game_Coinche::updateTime(Uint32 diffTime)
{
    _infos.AddClock(diffTime);
}


