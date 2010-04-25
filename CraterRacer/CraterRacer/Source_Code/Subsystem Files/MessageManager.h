#pragma once
#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <vector>
#include <string>
#include "Constants.h"
#include "GameObj.h"
#include "Simulator.h"
#include "SceneLoader.h"
#include "VictoryCalculator.h"
#include "CraterRacerApp.h"
#include "Renderer.h"
#include "Clock.h"

//event ids
enum Events {	EWrongWay, EWrongWayCancel, EPauseGame, EUnpauseGame, EStartClock,
				EGameFinished, ELapFinished, EPlayerFinished, EVibrate, EStartOrStopRace, 
				EVehicleCollision, EVictoryScreenUpdate, ENameEntered, ESetTimeLimit };

using namespace std;
//--------------------------------------------------------
//		CLASS: MessageManager
//		
//		This class uses the Singleton design pattern.
//		It will be used to process event messages between
//		objects in the game.
//
//		Steps to create an event and use it:
//		1.  Add eventID to Events enum above
//		2.  Add a case statement in the ProcessMessage 
//			function below
//		3.	Make sure the objects you are emitting from
//			and updating are members of the MessageManager
//			(add them if not...and include the MessageManager
//			header in the object's .cpp file, not .h file.)
//		4.  Use "Emit( yourID, a_parameter )"
//--------------------------------------------------------
using namespace std;


class MessageManager
{
  public:

	//get the instance of the manager 
	static MessageManager* Inst( );

	static void ProcessMessage	( int message, long param );
	static void ProcessMessage	( int message, long param1, long param2 );
	static void ProcessMessage	( int message, long param1, string param2 );
	static void ProcessMessage  ( int message, long param1, long param2, long param3 );
	static void ProcessMessage	( int message, string param );
	static void ProcessMessage	( int message );
	static void AddEmitter		( RacerApp* app );
	static void AddEmitter		( GameObj* game );
	static void deleteManager	( );

	~MessageManager( ) { }

protected:

	MessageManager( ) { } // constructor

private:

    //Singleton instance of message manager
    static MessageManager* pInstance;

	//Event emitters and receivers
	static GameObj*				m_Game;
	static Simulator*			m_Simulator;
	static VictoryCalculator*	m_VictoryCalculator;
	static EntityManager*		m_EntityManager;
	static RacerApp*			m_App;
	static Renderer*			m_Renderer;
	static Clock*				m_Clock;
};

#define Emit MessageManager::Inst()->ProcessMessage
#define InitEmitter MessageManager::Inst()->AddEmitter

#endif //MESSAGE_MANAGER_H