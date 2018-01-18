#ifndef _SPACESHIP_H
#define _SPACESHIP_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/*
spaceship.h
Header file for spaceship struct and functions for finding targets and initializing spaceships
*/

struct spaceship {
	float x; //XYZ coordinates
	float y;
	float z;
	uint32_t power; //Placeholder for later weaponArray
	int32_t hullIntegrity;
	struct spaceship* target;
	uint8_t destroyed;
};

struct fleet {
	struct spaceship* ships;
	struct fleet* targetFleet;
	uint32_t fleetSize;
	uint32_t numDestroyed;	
};


/*
initSpaceship
Initializes spaceship position, weapon, and hull values
Current position is random in a 20000 unit cubed box, weapon is 1000, hull is 3000
*/

void initSpaceship(struct spaceship* vessel);

/*
initFleet
Initializes a fleet with an amount of spaceships equal to size.
*/

void initFleet(struct fleet* fleetToInit, uint32_t size);

/*
countDestroyed
Counts the number of destroyed spaceships in a fleet
*/

uint32_t countDestroyed(struct fleet fleetToCheck);

/*
distance
Finds the 3-space distance between vessel 1 and 2
*/

float distance3(struct spaceship vessel1, struct spaceship vessel2);

/*
findNearest
Finds the nearest spaceship out of an array of ships to the input vessel and 
returns the pointer to that spaceship.
*/

struct spaceship* findNearest(struct spaceship vessel, struct fleet targetFleet);

/*
fireWeapons
Subtracts vessel1's power from its current targets health
*/

void fireWeapons(struct spaceship vessel);

/*
fleetAction
For each ship in the fleet, targets a ship in the enemy fleet and fires weapons.
*/

void* fleetAction(void* fleetToCommand);

/*
cleanup
For each ship in the fleet, if its health is zero or below, sets destroyed status
byte
*/

void* cleanup(void* fleetToClean);

/*
fleetIsDestroyed
Counts the number of destroyed ships in a fleet. If its equal to the fleet size,
returns 1, else 0
*/

uint8_t fleetIsDestroyed(struct fleet fleetToCheck);

/*
fightMain
Makes two fleets based on the sizes in argv and has them fight
*/

void fightMain(uint32_t size1, uint32_t size2);

#endif
