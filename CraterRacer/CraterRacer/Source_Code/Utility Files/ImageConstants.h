#pragma once
#ifndef IMAGE_CONSTANTS_H
#define IMAGE_CONSTANTS_H

//--------------------------------------------------------
//		DEFINITIONS
//--------------------------------------------------------

//Mesh files
#define OBJ_FILE						L"Media\\Meshes\\CarBox.x"
#define CAR_BODY_FILE					L"Media\\Meshes\\CarBody.x"
#define OBJ_EFFECT						L"Source Code\\Effect Files\\Scene.fx"
#define TERRAIN_FILE					L"Media\\Meshes\\Track.x"
#define TERRAIN_EFFECT_FILE				L"Source Code\\Effect Files\\Terrain.fx"

//Image files

//HUD image files
#define SPEEDOMETER_IMAGE_FILE			"Media\\Images\\Speedometer.png"
#define SPEEDWAND_IMAGE_FILE			"Media\\Images\\Speedwand.png"
#define MINIMAP_IMAGE_FILE				"Media\\Images\\MiniMap.png"
#define FIRST_IMAGE_FILE				"Media\\Images\\Numbers\\First.png"
#define SECOND_IMAGE_FILE				"Media\\Images\\Numbers\\Second.png"
#define THIRD_IMAGE_FILE				"Media\\Images\\Numbers\\Third.png"
#define FOURTH_IMAGE_FILE				"Media\\Images\\Numbers\\Fourth.png"

//Number image files
#define ZERO_IMAGE_FILE					"Media\\Images\\Numbers\\Zero.png"
#define ONE_IMAGE_FILE					"Media\\Images\\Numbers\\One.png"
#define TWO_IMAGE_FILE					"Media\\Images\\Numbers\\Two.png"
#define THREE_IMAGE_FILE				"Media\\Images\\Numbers\\Three.png"
#define FOUR_IMAGE_FILE					"Media\\Images\\Numbers\\Four.png"
#define FIVE_IMAGE_FILE					"Media\\Images\\Numbers\\Five.png"
#define SIX_IMAGE_FILE					"Media\\Images\\Numbers\\Six.png"
#define SEVEN_IMAGE_FILE				"Media\\Images\\Numbers\\Seven.png"
#define EIGHT_IMAGE_FILE				"Media\\Images\\Numbers\\Eight.png"
#define NINE_IMAGE_FILE					"Media\\Images\\Numbers\\Nine.png"
#define COLON_IMAGE_FILE				"Media\\Images\\Numbers\\Colon.png"

//Startup screen image files
#define MENU_IMAGE_FILE					"Media\\Images\\Logo.png"
#define ONEPLAYER_NOTACTIVE_IMAGE_FILE	"Media\\Images\\OnePlayerNotActive.png"
#define ONEPLAYER_ACTIVE_IMAGE_FILE		"Media\\Images\\OnePlayerActive.png"
#define TWOPLAYER_IMAGE_FILE			"Media\\Images\\TwoPlayer.png"
#define TWOPLAYER_ACTIVE_IMAGE_FILE		"Media\\Images\\TwoPlayerActive.png"
#define TIMETRIAL_IMAGE_FILE			"Media\\Images\\TimeTrial.png"
#define TIMETRIAL_ACTIVE_IMAGE_FILE		"Media\\Images\\TimeTrialActive.png"
#define GAMERULES_IMAGE_FILE			"Media\\Images\\GameRules.png"
#define GAMERULES_ACTIVE_IMAGE_FILE		"Media\\Images\\GameRulesActive.png"
#define EXIT_NOTACTIVE_IMAGE_FILE		"Media\\Images\\ExitNotActive.png"
#define EXIT_ACTIVE_IMAGE_FILE			"Media\\Images\\ExitActive.png"

//Loading screen image files
#define LOADING_IMAGE_FILE				"Media\\Images\\Loading.png"
#define LOADING_SMALLBALL_IMAGE_FILE	"Media\\Images\\LoadingSmallBall.png"
#define LOADING_BIGBALL_IMAGE_FILE		"Media\\Images\\LoadingBigBall.png"
#define CONTROLS_IMAGE_FILE				"Media\\Images\\Controls.png"

//Pause screen image files
#define UNPAUSE_IMAGE_FILE				"Media\\Images\\Unpause.png"
#define UNPAUSE_ACTIVE_IMAGE_FILE		"Media\\Images\\UnpauseActive.png"

//Game rules screens
#define GAMERULES_INFO_IMAGE_FILE		"Media\\Images\\GameRulesInfo.png"
#define GAMERULES_INFO_SMALL_IMAGE_FILE	"Media\\Images\\GameRulesInfoSmall.png"

#define NUM_BUTTONS				8
#define NUM_LOADING_BALLS		5
#define NUM_IMAGES				41

enum ImageNames{	MENU_IMAGE, CONTROLS_IMAGE, GAMERULES_INFO_IMAGE, 
					LOADING_IMAGE, LOADING_SMALLBALL_IMAGE, LOADING_BIGBALL_IMAGE,
					GAMERULES_INFO_SMALL_IMAGE, 
					
					SPEEDOMETER_IMAGE, SPEEDWAND_IMAGE, MINIMAP_IMAGE, 
					FIRST_IMAGE, SECOND_IMAGE, THIRD_IMAGE, FOURTH_IMAGE,

					ZERO_IMAGE, ONE_IMAGE, TWO_IMAGE, THREE_IMAGE, FOUR_IMAGE, FIVE_IMAGE,
					SIX_IMAGE, SEVEN_IMAGE, EIGHT_IMAGE, NINE_IMAGE, COLON_IMAGE,

					ONEPLAYER_NOTACTIVE_IMAGE, ONEPLAYER_ACTIVE_IMAGE, 
					TWOPLAYER_IMAGE, TWOPLAYER_ACTIVE_IMAGE,
					TIMETRIAL_IMAGE, TIMETRIAL_ACTIVE_IMAGE,
					GAMERULES_IMAGE, GAMERULES_ACTIVE_IMAGE,
					EXIT_NOTACTIVE_IMAGE, EXIT_ACTIVE_IMAGE, 

					UNPAUSE_IMAGE, UNPAUSE_ACTIVE_IMAGE,
					GAMERULES2_IMAGE, GAMERULES2_ACTIVE_IMAGE,
					EXIT2_NOTACTIVE_IMAGE, EXIT2_ACTIVE_IMAGE };


enum GuiControls{	GUI_BTN_SINGLE_PLAYER, GUI_BTN_TWO_PLAYER, GUI_BTN_TIMETRIAL,
					GUI_BTN_GAMERULES, GUI_BTN_EXIT, 
					GUI_BTN_UNPAUSE, GUI_BTN_GAMERULES2, GUI_BTN_EXIT2 };

#endif