#include "spaceship.h"

uint64_t getTimestamp() {
	uint64_t tsh;
	uint64_t tsl;
	uint64_t ts = 0;
	asm volatile ("rdtsc\n\t"
			: "=a" (tsl), "=d" (tsh)
		     );
	ts |= (tsh << 32);
	ts |= tsl;
	return ts;
}

void initSpaceship(struct spaceship* vessel) {
	int32_t x;
	int32_t y;
	int32_t z;
	x = rand() % 10000;
	y = rand() % 10000;
	z = rand() % 10000;
	vessel->x = x;
	vessel->y = y;
	vessel->z = z;
	vessel->power = 1000;
	vessel->hullIntegrity = 3000;
	vessel->destroyed = 0;
	return;	
}

void initFleet(struct fleet* fleetToInit, uint32_t size) {
	uint32_t i;
	fleetToInit->fleetSize = size;
	fleetToInit->numAlive = size;
	fleetToInit->ships = malloc(size*sizeof(struct spaceship));
	for (i = 0; i < size; i++) {
		initSpaceship(&(fleetToInit->ships[i]));
	}
	return;
}

uint32_t countDestroyed(struct fleet fleetToCheck) {
	uint32_t i;
	uint32_t count = 0;
	for (i = 0; i < fleetToCheck.fleetSize; i++) {
		if (fleetToCheck.ships[i].destroyed) {
			count++;
		}
	}
	return count;
}

float distance3(struct spaceship vessel1, struct spaceship vessel2) {
	return sqrt(pow(vessel2.x - vessel1.x, 2) + pow(vessel2.y - vessel1.y, 2) + pow(vessel2.z - vessel1.z, 2));
}

struct spaceship* findNearest(struct spaceship vessel, struct fleet targetFleet) {
	uint32_t i;
	float currDistance;
	float smallestDistance = INFINITY;
	struct spaceship* currTarget = NULL;
	for (i = 0; i < targetFleet.fleetSize; i++) {
		if (!targetFleet.ships[i].destroyed) {
			currDistance = distance3(vessel, targetFleet.ships[i]);
			if (currDistance < smallestDistance) {
				currTarget = &(targetFleet.ships[i]);
				smallestDistance = currDistance;
			}
		}
	}
	return currTarget;
}

void fireWeapons(struct spaceship vessel) {
	if(vessel.target != NULL) {
		vessel.target->hullIntegrity -= vessel.power;
	}
	return;
}

void* fleetAction(void* fleetToCommand) {
	uint32_t i;
	for (i = 0; i < ((struct fleet*)fleetToCommand)->fleetSize; i++) {
		if (!((struct fleet*)fleetToCommand)->ships[i].destroyed) {
			((struct fleet*)fleetToCommand)->ships[i].target = findNearest(((struct fleet*)fleetToCommand)->ships[i], *(((struct fleet*)fleetToCommand)->targetFleet));
			fireWeapons(((struct fleet*)fleetToCommand)->ships[i]);
		}
	}
	pthread_exit(NULL);
}

void* cleanup(void* fleetToClean) {
	uint32_t i;
	for (i = 0; i < ((struct fleet*)fleetToClean)->fleetSize; i++) {
		if (!((struct fleet*)fleetToClean)->ships[i].destroyed && ((struct fleet*)fleetToClean)->ships[i].hullIntegrity <= 0) {
			((struct fleet*)fleetToClean)->ships[i].destroyed = 1;
			((struct fleet*)fleetToClean)->numAlive--;
		}
	}
	pthread_exit(NULL);
}

uint32_t fleetIsAlive(struct fleet fleetToCheck) {
	return fleetToCheck.numAlive;
}

void fightMain(uint32_t size1, uint32_t size2) {
	uint64_t t1;
	uint64_t t2;
	uint64_t numClks;
	float numClksPerShip;
	struct fleet fleet1;
	struct fleet fleet2;
	FILE * data;
	uint8_t i;
	pthread_t threads[2];
	pthread_attr_t attr;
	void* status;	

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

	srand(time(NULL));
	initFleet(&fleet1, size1);
	initFleet(&fleet2, size2);
	fleet1.targetFleet = &fleet2;
	fleet2.targetFleet = &fleet1;
	t1 = getTimestamp();
	while(fleetIsAlive(fleet1) && fleetIsAlive(fleet2)) {
		pthread_create(&threads[0], &attr, fleetAction, (void*)&fleet1);
		pthread_create(&threads[1], &attr, fleetAction, (void*)&fleet2);
		for (i = 0; i < 2; i++) {
			pthread_join(threads[i], &status);
		}
		pthread_create(&threads[0], &attr, cleanup, (void*)&fleet1);
		pthread_create(&threads[0], &attr, cleanup, (void*)&fleet2);
		for (i = 0; i < 2; i++) {
			pthread_join(threads[i], &status);
		}
	}
	t2 = getTimestamp();
	numClks = t2 - t1;
	numClksPerShip = (float)(t2 - t1)/(float)(fleet1.fleetSize + fleet2.fleetSize);
	printf("Remaining fleet1 ships: %d\n", fleet1.numAlive);
	printf("Remaining fleet2 ships: %d\n", fleet2.numAlive);
	printf("Number Clock Cycles: %lu\n", numClks);
	printf("Clock Cycles Per Ship: %f\n", numClksPerShip);
	data = fopen("pclockcycles.csv","a");
	fprintf(data,"%lu,",numClks);
	fclose(data);
	data = fopen("pclockcyclespership.csv","a");
	fprintf(data,"%f,",numClksPerShip);
	fclose(data);
	return;
}
