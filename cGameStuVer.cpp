/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	
	//OpeningScreenF.
	theAreaClicked = { 0, 0 };
	// Store the textures
	textureName = { "sea", "bottle", "ship","theBackground", "OpeningScreen", "ClosingScreen","Asteroid"}; 
	texturesToUse = { "Images/Sprites/Space.png", "Images/Sprites/Ore.png", "Images/Sprites/Rocket.png", "Images/Bkg/BkgndA.png", "Images/Bkg/ClosingScreenF1.png", "Images/Bkg/ClosingScreenF1.png","Images/Sprites/Asteroid.png" }; //Changed the sprites to fit my game.
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	tempTextTexture = theTextureMgr->getTexture("sea");
	aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	aColour = { 0, 255, 255, 255 };
	// Store the textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn","Highscore_btn" }; 
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png","Images/Buttons/Button_hscore.png" }; 
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 },{ 400,325 } }; //POSTION OF BUTTON
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = gameState::menu;
	theBtnType = btnTypes::exit;
	// Create textures for Game Dialogue (text)
	fontList = { "pirate", "skeleton" };
	fontsToUse = { "Fonts/ProFontWindows.ttf", "Fonts/Walkway_SemiBold.ttf" }; //Changed the texts to suit the game better. this was linked in the font files.
	for (unsigned int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 48);
	}
	// Create text Textures
	gameTextNames = { "TitleTxt", "CollectTxt", "InstructTxt", "ThanksTxt", "SeeYouTxt", "BottleCount", "Highscore"};
	gameTextList = { "Asteroid adventure", "Collect the Floating ore!", "Use the arrow keys to navigate the map.", "Looks like the ore run is over!", "Go to menu?", "Collected: ", "Highscore: "}; //Changed some of the text to suite my game
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("pirate")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("skeleton")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
	}
	// Load game sounds
	soundList = { "theme", "click","Collect", "Explosion"};  //Adds more soundeffects from my own use
	soundTypes = { soundType::music, soundType::sfx,soundType::sfx,soundType::sfx };
	soundsToUse = { "Audio/Theme/Kevin_MacLeod_-_Winter_Reflections.wav", "Audio/SFX/ClickOn.wav","Audio/SFX/Collect.wav","Audio/SFX/Explosion.wav" }; //linked my own sounds
	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);
	theTileMap.setMapStartXY({ 150,100 }); //Sets up where the map starts (in referance to the top left)

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
	
	//SCORE SETUP
	bottlesCollected = 0;
	strScore = gameTextList[gameTextList.size() - 2];
	strScore += to_string(bottlesCollected).c_str();
	theTextureMgr->deleteTexture("BottleCount");
	
	
	ifstream myfile("Files/Highscore.txt"); //Opening the file.
	if (myfile.is_open())
	{
		myfile >> Checkscore; //Taking the score
		myfile.close(); //Close that file!
		//std::string test1 = std::to_string(Checkscore); //Thistakes the highscore Int and puts it into a string.
		//MessageBoxA(NULL,  test1.c_str(), "This is the Highscore!", MB_OK);
	}
	else //if there is an issue it wont open - to prevent loading issues.
	{
		cout << "Unable to open";
	}
	
	strHscore = gameTextList[gameTextList.size() - 1];
	strHscore += to_string(Checkscore).c_str();
	theTextureMgr->deleteTexture("Highscore");

	Gameover = false;


}

	void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case gameState::menu:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("CollectTxt");
		pos = { 50, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("InstructTxt");
		pos = { 50, 175, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 425 }); //Moved to fit my game.
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		//This changes the background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		Gameover = false;
		
		if (Oneload == false) //Only will load this once.
		{
			ifstream myfile("Files/Highscore.txt"); //Opening the file.
			if (myfile.is_open())
			{
				myfile >> Checkscore; //Taking the score everytime so if the user gets a new highscore it will be set to the new checkscore.
				myfile.close(); //Close that file!
				//std::string test1 = std::to_string(Checkscore); //Thistakes the highscore Int and puts it into a string.
				//MessageBoxA(NULL,  test1.c_str(), "This is the Highscore!", MB_OK);
			}
			else
			{
				cout << "Unable to open";
			}
			Oneload = true; //This will flip so the file is ONLY opened once. having it open every second its in menu is very bad. 
		}

		

	}
	break;
	case gameState::playing: //When the player presses the "play button"
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theTextureMgr->addTexture("BottleCount", theFontMgr->getFont("pirate")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("BottleCount");
		pos = { 600, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTileMap.renderGridLines(theRenderer, aRect, aColour); 
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 935, 700 }); //Edited the button to fit my game board.
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		//This changes the background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
	    
        	
	}
	break;
	case gameState::end: //When the player presses the "exit" button
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Thanks for playing txt set up
		tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
		pos = { 50, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//See you Txt set up
		tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
		pos = { 50, 175, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		//Adds the score to the end of the game
		theTextureMgr->addTexture("BottleCount", theFontMgr->getFont("pirate")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 })); //THIS HERE IS IMPORTANT
		tempTextTexture = theTextureMgr->getTexture("BottleCount"); //THIS HERE IS IMPORTANT
		pos = { 600, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h }; //THIS HERE IS IMPORTANT
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale); //THIS HERE IS IMPORTANT
		
		//Highscore text
		theTextureMgr->addTexture("Highscore", theFontMgr->getFont("pirate")->createTextTexture(theRenderer, strHscore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 })); //THIS HERE IS IMPORTANT
		tempTextTexture = theTextureMgr->getTexture("Highscore"); //THIS HERE IS IMPORTANT
		pos = { 60, 220, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h }; //THIS HERE IS IMPORTANT
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
																																	 //This changes the background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("ClosingScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("ClosingScreen")->getTWidth(), theTextureMgr->getTexture("ClosingScreen")->getTHeight());
		
	    
		
		
		if (Checkscore < bottlesCollected && Oneload == true) //If The score you got is higher than the last score saved. it will Save the new score to the file.
		{
			ofstream Writefile("Files/Highscore.txt"); //Opening the file located in the game code.
			if (Writefile.is_open()) //if it can be accually open (file is found)
			{
				Writefile << bottlesCollected; //Writes The score of the game to the file.
				Writefile.close(); //Close that file!
			}
			else
			{
				cout << "Unable to open"; //Stopping very bad issues.
			}
			Oneload = false; //This varible will Stop it from constantly opening the file.
		}
		
			
		
			
		
		//CHECK IF THE HIGH SCORE IS HIGHER THAN THE STORED HIGH SCORE.
		//IF SO OPEN A TEXT BOX ASKING FOR YOUR NAME.
		//EMPTY THE FILE
		//ADD NAME AND SCORE TO THE TEXT FILE.
		//CLOSE THE FILE.
	}
	break;
	case gameState::quit: //When the player exit the game.
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// Check Button clicked and change state (Bobby's Code)
	//IMPORTANT - USED TO CHANGE THE STATE OF THE GAME.
	if (theGameState == gameState::menu || theGameState == gameState::end)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);
		//If the user clicks the button - 
	}
	else
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::end, theAreaClicked); // Change update
	}
	


	if (theGameState == gameState::menu)
	{
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked); //When the player clicks the play button
		Gameover = false;
		//This code allows replayability.
		if (theGameState == gameState::playing &&  Gameover == false)
		{
			//Hiding the sprites.
			theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			theTileMap.update(theBottle.getMapPosition(), 1, 0.0f);
			theTileMap.update(thePirate.getMapPosition(), 1, 0.0f);
			//Setting the accual starting points.
			theShip.setMapPosition(spriteRandomX(gen), spriteRandomY(gen)); //Changed to fix the new game map.
			theBottle.genRandomPos(theBottle.getMapPosition().R, theBottle.getMapPosition().C); //Changed from your code - should of been the bottle, NOT the ship.
			thePirate.genRandomPos(thePirate.getMapPosition(), thePirate.getMapPosition());
			//Updating the tilemap to accualy show the sprites.
			theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
			theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation()); 
			bottlesCollected = 0;
			//Updating the score.
		
		}
	}
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);

	if (theGameState == gameState::playing)
	{
	
		// Check if ship has collided with the bottle
		if (theShip.getMapPosition() == theBottle.getMapPosition()) //When the ship touches the bottle
		{
			theSoundMgr->getSnd("Collect")->play(0); //Plays the collect bottle collect sound on colision
			bottlesCollected = bottlesCollected++; //Adds one to the score
			theBottle.genRandomPos(theShip.getMapPosition().R, theShip.getMapPosition().C); //NEW POSTITION
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation()); //UPDATE	
			theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation()); //The ship keeps its rotation when it collects a bottle
			//Updating the score.
			strScore = gameTextList[gameTextList.size() - 2]; 
			strScore += to_string(bottlesCollected).c_str();
			theTextureMgr->deleteTexture("BottleCount");
		}

		if (thePirate.getMapPosition() == theBottle.getMapPosition())
		{
			theBottle.genRandomPos(thePirate.getMapPosition().R, thePirate.getMapPosition().C); //NEW POSTITION. UPDATES IN REFERNCE TO THE PIRATE
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation()); //UPDATE
			theSoundMgr->getSnd("Explosion")->play(0);
			}

		if (theShip.getMapPosition() == thePirate.getMapPosition())
		{
			theGameState = gameState::end;
		}
		
		if (Gameover == true)
		{
			
			//theGameState = gameState::end;
		}
		

	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
					//if (theGameState == gameState::playing)
					//{
					//	theTilePicker.update(theAreaClicked);
					//	if (theTilePicker.getTilePicked() > -1)
					//	{
					//		dragTile.setSpritePos(theAreaClicked);
					//		dragTile.setTexture(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1]));
					//		dragTile.setSpriteDimensions(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTWidth(), theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTHeight());
					//	}
					//}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					//if (theGameState == gameState::playing)
					//{
					//	theAreaClicked = { event.motion.x, event.motion.y };
					//	theTileMap.update(theAreaClicked, theTilePicker.getTilePicked());
					//	theTilePicker.setTilePicked(-1);
					//}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				dragTile.setSpritePos({ event.motion.x, event.motion.y });
			}
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					if (theGameState == gameState::playing) //If you accualy in the game - prevents bugs
					{
						//Hides the sprites for movement.
						theTileMap.update(thePirate.getMapPosition(), 1, thePirate.getEnemyRotation());
						theTileMap.update(theShip.getMapPosition(), 1, theShip.getShipRotation());
						if (theShip.getMapPosition().R != 9) //UNLESS ITS ROW IS 9. IT WONT MOVE THE SHIP.
						{
							theShip.setShipRotation(180); //Makes it point Down
							theShip.setMapPosition(theShip.getMapPosition().R + 1, theShip.getMapPosition().C); //Moves it down
						}
						thePirate.setEnemyRotation(thePirate.getEnemyRotation()+90);
						if (thePirate.getMapPosition().R == 9 && thePirate.getMapPosition().C == 11) //PIRATE MOVEMENT.RESTTING BACK TO THE START
						{
							thePirate.setMapPosition(0, 0); //Back to the start
							theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation()); //Updating its location
						}
						else
						{
							if (thePirate.getMapPosition().R == 9)//RESTTING BACK TO THE NEXT COLLUM
							{
								thePirate.setMapPosition(0, thePirate.getMapPosition().C + 1); //Makes it moveto the next collum
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation()); //Updating its location
							}
							else //MOVING IT DOWN
							{
								thePirate.setMapPosition(thePirate.getMapPosition().R + 1, thePirate.getMapPosition().C); //Moving it down 1
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation()); //Updating the screen.
							}

						}
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());//Updating for ship rotation. 

					}
				}
				break;

				case SDLK_UP: //THIS CODE IS SIMILAR TO THE SDLK_DOWN CODE BUT CHANGED TO MOVE UP.
				{
					
					if (theGameState == gameState::playing)
					{

						//DELETE THE SHIPS LAST LOCATION
						theTileMap.update(theShip.getMapPosition(), 1, theShip.getShipRotation());
						if (theShip.getMapPosition().R != 0) //UNLESS ITS ROW IS 0
						{
							theShip.setShipRotation(0);
							theShip.setMapPosition(theShip.getMapPosition().R - 1, theShip.getMapPosition().C);
						}
						thePirate.setEnemyRotation(thePirate.getEnemyRotation() + 90);
						//ENEMY MOVEMENT IS THE SAME FOR ALL MOVEMENT KEYS.
						theTileMap.update(thePirate.getMapPosition(), 1, thePirate.getEnemyRotation());
						if (thePirate.getMapPosition().R == 9 && thePirate.getMapPosition().C == 11)
						{
							thePirate.setMapPosition(0, 0);
							theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
						}
						else
						{
							if (thePirate.getMapPosition().R == 9)
							{
								thePirate.setMapPosition(0, thePirate.getMapPosition().C + 1);
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
							}
							else
							{
								thePirate.setMapPosition(thePirate.getMapPosition().R + 1, thePirate.getMapPosition().C);
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
							}

						}
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
					}
				}
				break;
				case SDLK_RIGHT://THIS CODE IS SIMMILAR TO SDLK_DOWN. BUT CHANGED TO MOVE RIGHT
				{
					if (theGameState == gameState::playing)
					{

						theTileMap.update(theShip.getMapPosition(), 1, theShip.getShipRotation());
						if (theShip.getMapPosition().C != 11) //UNLESS ITS COLLUM IS 11
						{
							theShip.setShipRotation(90);
							theShip.setMapPosition(theShip.getMapPosition().R, theShip.getMapPosition().C + 1);
						}
						thePirate.setEnemyRotation(thePirate.getEnemyRotation() + 90);
						//ENEMY MOVEMENT IS THE SAME FOR ALL MOVEMENT KEYS
						theTileMap.update(thePirate.getMapPosition(), 1, thePirate.getEnemyRotation());
						if (thePirate.getMapPosition().R == 9 && thePirate.getMapPosition().C == 11)
						{
							thePirate.setMapPosition(0, 0);
							theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
						}
						else
						{
							if (thePirate.getMapPosition().R == 9)
							{
								thePirate.setMapPosition(0, thePirate.getMapPosition().C + 1);
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
							}
							else
							{
								thePirate.setMapPosition(thePirate.getMapPosition().R + 1, thePirate.getMapPosition().C);
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
							}

						}
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
					}
				}
				break;

				case SDLK_LEFT://MOVEMENT IS SIMMILAR TO SDLK_DOWN.JUST CHANGED TO MOVE LEFT
				{
					if (theGameState == gameState::playing)
					{

						//DELETE THE SHIPS LAST LOCATION
						theTileMap.update(theShip.getMapPosition(), 1, theShip.getShipRotation());
						if (theShip.getMapPosition().C != 0) //UNLESS ITS COLLUM IS 0
						{
							theShip.setShipRotation(270);
							theShip.setMapPosition(theShip.getMapPosition().R, theShip.getMapPosition().C - 1);
						}
						thePirate.setEnemyRotation(thePirate.getEnemyRotation() + 90);
						//ENEMY MOVEMENT IS THE SAME FOR ALL MOVEMENT KEYS.
						theTileMap.update(thePirate.getMapPosition(), 1, thePirate.getEnemyRotation());
						if (thePirate.getMapPosition().R == 9 && thePirate.getMapPosition().C == 11)
						{
							thePirate.setMapPosition(0, 0);
							theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
						}
						else
						{
							if (thePirate.getMapPosition().R == 9)
							{
								thePirate.setMapPosition(0, thePirate.getMapPosition().C + 1);
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
							}
							else
							{
								thePirate.setMapPosition(thePirate.getMapPosition().R + 1, thePirate.getMapPosition().C);
								theTileMap.update(thePirate.getMapPosition(), 7, thePirate.getEnemyRotation());
							}

						}
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
					}
				}
				break;
				case SDLK_SPACE:
				{
				}
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

