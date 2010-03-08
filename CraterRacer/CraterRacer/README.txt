-----------------------------------------------------------------------
                               Crater Racer
                               Moon or Bust
-----------------------------------------------------------------------

 Troy Alderson, Robert Hilton, Florence Ng, Josh Ulmer, Amber Wetherill
 
 Milestone 3
 Mar 8, 2010
 CPSC 585
 http://www.googlecode.com/p/cpsc585

-----------------------------------------------------------------------
                         Included in the submission
-----------------------------------------------------------------------

 - CraterRacer source code
 - CraterRacer.exe
 - Readme.txt

-----------------------------------------------------------------------
			       Requirements
-----------------------------------------------------------------------

 - DirectX 9.0c (Aug 2008)
 - Visual Studio 2008
 - XBox Controller

-----------------------------------------------------------------------
	                    How to run the game
-----------------------------------------------------------------------

 1. Resave BgMusic.xap using XACT. (if there is a versioning problem)
 2. Run CraterRacer.exe in the Debug folder
 3. How to exit:
    Choose the exit menu option in the startup screen.
    To exit during gameplay, get to the pause screen (by pressing the
    spacebar or by pressing the XBox Controller "Start" button) and
    choose exit.  
    OR, at any time, press ESC to exit.
 
 KeyBoard Game Controls

 - Menu screen:  - UP/DOWN Arrow Keys to move to menu buttons
		 - ENTER to select menu choice

 - Game Play:
		 - "d" key to drop the car from a position above the ground
		 - "s" key to stop the vehicle motion
		 - "spacebar" to pause the game

 - Pause Screen: - UP/DOWN Arrow Keys to move to menu buttons
		 - ENTER to select menu choice

 XBox Controller

 - Menu Screen:  - Left Thumbstick to move to menu buttons
		 - Press "A" to select the menu choice

 - Game Play:	 - Left Thumbstick for steering
		 - "A" button - nothing
		 - "B" button - nothing
		 - "X" butyon to stop (for debugging purposes)
		 - "Y" button to print the current location of P1's vehicle
		   to debug.txt
		 - Right Trigger to accelerate
		 - Left Trigger to brake or reverse
		 - Start button to pause game
		 - Back button to drop car from position above the ground

 - Pause Screen: - Left Thumbstick to move to menu buttons
		 - Press "A" to select the menu choice

-----------------------------------------------------------------------
                         Description of milestone
-----------------------------------------------------------------------

 The menu screen is loaded first.  Here is where you will see the option
 for "Single Player" game or "Exit". Choosing "Single Player" will start
 the game.

 When the game loads, there are three cars loaded.  One car will be
 controlled by you, the other two are AI vehicles that follows the
 waypoints defined in OnePlayer.SCENE. Both vehicles are dropped from 
 above the ground. If a vehicle drops into a crater, it gets respawned
 at the last waypoint it reached.

 The meteors are wheels at the moment, and they fall onto the
 destination location correctly. There are two in this milestone.
 Triggered by a trigger volume, and it is possible to trigger multiple
 meteors at a time.

 The track in this milestone will be the basis of the final track. A
 basic HUD is also present, showing the FPS, game time, and current
 ranking. After one lap, your ranking is shown.

 The sound system plays background music and loops through various
 soundtracks. Engine noise and meteor crashing sound effects also
 present.

 The keyboard's controls do not work for this milestone anymore. The
 controller layout has been changed to reflect standard racing controls.

-----------------------------------------------------------------------
				Issues
-----------------------------------------------------------------------

 - Logic for engine sounds
 - AI navigation buggy
 - no meteor mesh
 - no "spacey"-ness feel
 - driving up walls
 - sticky walls
 - getting off the walls
 - jumps cause vehicles to run into walls on occasion
 - flipping still occurs
 - need to follow the AI track to win game
 - track has small gaps that the vehicle can get stuck in
