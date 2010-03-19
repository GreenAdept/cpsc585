
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
	vector<int> ranks;
	vector<string> times;

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
		temp = m_Clock->getFormattedTime();
		m_VictoryCalculator->setFinishTime( playerNum, temp );
		break;

	case EGameFinished:
		ranks = *((vector<int>*)param);
		times = m_VictoryCalculator->getFinishTimes( );
		m_Renderer->adjustVictoryRank( ranks, times );
		break;
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
		lapNum = 2 - param2 + 1;
		m_Renderer->adjustLapImage( playerNum, lapNum );
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

		m_Clock->togglePause();
		m_Simulator->pause( true );
		m_App->m_AppState = APP_PAUSED;
		m_App->m_uiCurrentButton = GUI_BTN_UNPAUSE;
		break;

	case EUnpauseGame:
		m_Clock->togglePause();
		m_Simulator->pause( false );
		break;

	case EStartClock:
		m_Clock->start( );
		break;

	case EGameFinished:
		m_App->m_AppState = APP_VICTORY;
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