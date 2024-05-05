#ifndef CONFIG_H
#define CONFIG_H

// USER DEFINED //

/* Add // before #define to disable a setting. */

/* Delay before freezing the bot when none data is received from radio */
#define RADIO_TIMEOUT 1000

/* The radio address to use. */
#define RADIO_ADDRESS "912CR"

/* Define wether to enable info logs or not. */
#define INFO_STATE

/* Define wether to enable debug logs or not. */
#define DEBUG_STATE

/* Enable or disable test sequence.
Must be disabled for normal usage */
//#define TEST_MODE

/* Pourcentage of power for mover motors*/
#define MOVERS_POWER_PRCT 100.0

/* The default score to display on the robot.
Cannot disable. */
#define DEFAULT_SCORE 40

/* Delay between each cycle of the loop.
Should be sync with the one on the controller.
----------------------------------------------------------------
/!\ THIS IS A CRITICAL SETTING. DO NOT SET WITHOUT KNOWLEDGE /!\.
The lower it is set the higher the power consumption will be.
Making the delay too high will make the robot unresponsive.
Making the delay too low will make the robot consume too much power and
risks devices damages and power supply issues.
----------------------------------------------------------------
Cannot disable. */
#define HEARTBEAT_DELAY 50

// IO //

#define RADIO_CE 9
#define RADIO_CSN 10 

#define LEFT_PUSHER 5
#define RIGHT_PUSHER 6
 
#define LEFT_SBR 2
#define RIGHT_SBR 3

#define SCORE_DIO 5
#define SCORE_CLK 4

#define LEFT_GRABBER_OPEN_SERVO A6

// END USER DEFINED //

#endif