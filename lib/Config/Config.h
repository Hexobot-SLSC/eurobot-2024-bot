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

#define RADIO_CE 7
#define RADIO_CSN 8

#define LEFT_PUSHER NOT_A_PIN
#define RIGHT_PUSHER NOT_A_PIN

#define LEFT_SBR 3
#define RIGHT_SBR 6

#define SCORE_DIO A0
#define SCORE_CLK A1

// END USER DEFINED //

#endif