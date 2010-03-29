-----------------------------------------------------------------------
                               Crater Racer
                               Moon or Bust
-----------------------------------------------------------------------

 Troy Alderson, Robert Hilton, Florence Ng, Josh Ulmer, Amber Wetherill
 
 Milestone 4
 Mar 29, 2010
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
 - XBox Controller (or two for two player)

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

 - Menu screen:  - UP/DOWN on left joystick to move to menu buttons
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
		 - "X" brake
		 - Right Trigger to accelerate
		 - Left Trigger to brake or reverse
		 - Start button to pause game
		 - Back button to reset the vehicle

 - Pause Screen: - Left Thumbstick to move to menu buttons
		 - Press "A" to select the menu choice

-----------------------------------------------------------------------
                         Description of milestone
-----------------------------------------------------------------------

 The menu screen is loaded first.  Here is where you will see the option
 for "Single Player", "Two Player", "Time Trial" or "Exit".

 New to this milestone are the "Two Player" and "Time Trial" options.
 In time trial, you race by yourself against the clock.  If your time is
 within the top 5, it is recorded in times.txt.

 When the game loads, there are eight cars loaded.  One car will be
 controlled by you, the other are AI vehicles that follows the
 waypoints defined in OnePlayer.SCENE. If a vehicle leaves the track, 
 it will move very slowly, and the controller will rumble.  Falling off 
 the edge of the track will cause the vehicle to respawn.

 The meteors trigger when a vehicle is within a certain range of them.
 meteors spawn in groups.  Meteors can land on vehicles, and may cause a large
 force to be applied the vehicle.  This is not an undesired side-effect.  When 
 the meteors hit the ground, a crater forms where
 the meteor lands.  The craters are static, unmoving objects, and can be used
 as ramps, or may just act as a wall.

 The track in this milestone is very close to complete.  However, we may 
 do some minor editing of it. A HUD is also present, showing the FPS, game 
 time, and current ranking, and a spedometer. After two laps, the victory
 screen is shown.

 The sound system plays background music and loops through various
 soundtracks. Engine noise and meteor crashing sound effects also
 present.

 The AI difficulty has been tweaked as well. If the AI is in first place
 during the race, it gets a speed penalty, so that other players can have a
 chance to catch up. If it is the player who is in front, then there
 is no speed reduction. Similarly, if any vehicle is in last place,
 it will get a speed boost to let them catch up.

-----------------------------------------------------------------------
				Issues
-----------------------------------------------------------------------

 - Logic for engine sounds
 - Need to follow the AI track to win game
 - You have to wait before being able to use menu at victory screen
 - Choosing main menu from victory screen doesn't allow you to play again
 - Can drive off track, and skip parts, but you won't be able to win
 - Some craters stick out of ground, and cause hard collisions
 - AI gets temporarily stuck on some meteors
 - Victory screen doesn't show correct ranking or times
 - Shadows are not present
 - Need to fix spedometer to show km/hr rather than m/s
