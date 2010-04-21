
#include "MessageManager.h"

//--------------------------------------------------------------------------------------
// Instantiate all static data members
//--------------------------------------------------------------------------------------
MessageManager*		 MessageManager::pInstance = NULL;
GameObj*			 MessageManager::m_Game = NULL;
Simulator*			 MessageManager::m_Simulator = NULL;
VictoryCalculator*	 MessageManager::m_VictoryCalculator = NULL;
EntityManager*		 MessageManager::m_EntityManager = NULL;
RacerApp*			 MessageManager::m_App = NULL;
Clock*				 MessageManager::m_Clock = NULL;
Renderer*			 MessageManager::m_Renderer = NULL;


//--------------------------------------------------------------------------------------
// Singleton instance of message manager
//--------------------------------------------------------------------------------------
MessageManager* MessageManager::Inst( )
{
  if( pInstance == NULL )
	  pInstance = new MessageManager( );
  
  return pInstance;
}


//--------------------------------------------------------------------------------------
// Function: ProcessMessage
// This function processes an incoming message with a parameter
// INPUTS:	 int message - an id from the Events enum list
//			 long param	 - different for each message (can be address, number )
//--------------------------------------------------------------------------------------
void MessageManager::ProcessMessage( int message, long param )
{
	int playerNum;
	string temp;

	switch( message )
	{
	case EWrongWay:
		playerNum = param;
		m_Renderer->adjustWrongWay( playerNum, true );
		break;

	case EWrongWayCancel:
		playerNum = param;
		m_Renderer->adjustWrongWay( playerNum, false );
		break;

	case EPlayerFinished:
		playerNum = param;
		m_Game->disableVibrate(playerNum);
		temp = m_Clock->getFormattedTime();
		m_VictoryCalculator->setFinishTime( playerNum, temp );
		break;
	case EStartOrStopRace:
		//if param != 1, start race, else stop race
		if (param == 1) {
			m_Simulator->startOrStopRace(true);
			m_Clock->start();
		}
		else
			m_Simulator->startOrStopRace(false);
	}
}


//--------------------------------------------------------------------------------------
// Function: ProcessMessage
// This function processes an incoming message with two parameters
// INPUTS:	 int message - an id from the Events enum list
//			 long param	 - different for each message (can be address, number )
//			 long param	 - different for each message (can be address, number )
//--------------------------------------------------------------------------------------
void MessageManager::ProcessMessage( int message, long param1, long param2 )
{
	int playerNum, lapNum;
	string temp;

	switch( message )
	{
	case ELapFinished:
		playerNum = param1;
		lapNum = param2;
		m_Renderer->adjustLapImage( playerNum, lapNum );
		break;
	case EVehicleCollision:
		//param1 is the id of the first vehicle, param2 is the id of the second vehicle
		break;
	}
}

//--------------------------------------------------------------------------------------
// Function: ProcessMessage
// This function processes an incoming message with two parameters
// INPUTS:	 int message	 - an id from the Events enum list
//			 long param1	 - different for each message (can be address, number )
//			 string param2	 - different for each message (can be address, number )
//--------------------------------------------------------------------------------------
void MessageManager::ProcessMessage( int message, long param1, string param2 )
{
	int playerNum, lapNum;
	string temp;

	switch( message )
	{
	case ENameEntered:
		m_VictoryCalculator->closeFile(param1, param2);
		m_App->m_AppState = APP_SHOWTIMES;
		m_App->m_uiCurrentButton = GUI_BTN_MAINMENU;
		m_Renderer->adjustButtonImage( GUI_BTN_MAINMENU, +1 );
		break;
	}
}

void MessageManager::ProcessMessage( int message, long param1, long param2, long param3 )
{
	switch( message )
	{
	case EVibrate:
		//param1 is controller id, param2 is intensity (scale from 0-100), param3 is duration in seconds/100
		m_Game->vibrate((int)param1, param2, param3);
		break;
	}
}

//--------------------------------------------------------------------------------------
// Function: ProcessMessage
// This function processes an incoming message with no parameter.
// INPUTS:	 int message - an id from the Events enum list
//--------------------------------------------------------------------------------------
void MessageManager::ProcessMessage( int message )
{
	switch( message )
	{
	case EPauseGame:
		if ( m_App->m_iCount < 0 ) {
			m_Clock->togglePause();
			m_Simulator->pause( true );
			m_App->m_AppState = APP_PAUSED;
			m_App->m_uiCurrentButton = GUI_BTN_UNPAUSE;
			m_Renderer->adjustButtonImage( GUI_BTN_UNPAUSE, +1 );
			m_Game->vibrate(0, 0, 0);
			m_Game->vibrate(1, 0, 0);
		}
		break;

	case EUnpauseGame:
		m_Clock->togglePause();
		m_Simulator->pause( false );
		break;

	case EStartClock:
		m_Clock->start( );
		break;

	case EGameFinished:
		if (m_App->m_bIsTimeTrial) {
			string time = m_Clock->getFormattedTime();
			int result = m_VictoryCalculator->recordTime(time);
			if (result < 5) {
				m_Renderer->resetBestName();
				m_App->m_AppState = APP_ENTERNAME;
				m_Renderer->adjustButtonImage( GUI_BTN_MAINMENU, -1 );
				m_App->m_uiCurrentButton = A_LETTER;
			}
			else {
				m_App->m_AppState = APP_SHOWTIMES;
				m_App->m_uiCurrentButton = GUI_BTN_MAINMENU;
				m_Renderer->adjustButtonImage( GUI_BTN_MAINMENU, +1 );
			}
			m_Renderer->adjustBestTimes( m_VictoryCalculator->getRecordedTimes(), m_VictoryCalculator->getRecordedNames(), time, result);
		}
		else {
			m_Renderer->adjustVictoryRank (m_VictoryCalculator->getRanks(), m_VictoryCalculator->getFinishTimes());
			m_App->m_AppState = APP_VICTORY;
			m_App->m_uiCurrentButton = GUI_BTN_MAINMENU;
			m_Renderer->adjustButtonImage( GUI_BTN_MAINMENU, +1 );
		}
		m_Game->vibrate(0, 0, 0);
		m_Game->vibrate(1, 0, 0);
		break;

	case EVictoryScreenUpdate:
		m_App->m_AppState = APP_VICTORY;
		m_Renderer->adjustVictoryRank (m_VictoryCalculator->getRanks(), m_VictoryCalculator->getFinishTimes());
		break;
	}
}


//--------------------------------------------------------------------------------------
// Function: AddEmitter
// This function adds the RacerApp and Renderer emitter. 
// INPUTS:	 RacerApp* app - a pointer to an initialized application
//--------------------------------------------------------------------------------------
void MessageManager::AddEmitter( RacerApp* app )
{
	m_App = app;
	m_Renderer = app->m_Renderer;
}


//--------------------------------------------------------------------------------------
// Function: AddEmitter
// This function adds some emitters and receivers based on an initialized GameObj. 
// INPUTS:	 GameObj* game - a pointer to an initialized GameObj
//--------------------------------------------------------------------------------------
void MessageManager::AddEmitter( GameObj* game )
{
	m_Game = game;
	m_Simulator = game->m_Simulator;
	m_VictoryCalculator = &game->m_Victory;
	m_EntityManager = game->m_Entities;
	m_Clock = &game->m_Clock;
}


//--------------------------------------------------------------------------------------
// Function: deleteManager( )
// This function deletes the singleton instance.
//--------------------------------------------------------------------------------------
void MessageManager::deleteManager( )
{
	if( pInstance )
		delete pInstance;
	pInstance = NULL;
}