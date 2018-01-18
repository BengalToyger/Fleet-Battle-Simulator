#include "spaceship.h"

int main(int argc, char** argv) {
	uint32_t trial = 1;
	uint32_t size = 1;
	FILE * clks;
	FILE * clksPerShip;
	clks = fopen("ompclockcycles.csv","a");
	clksPerShip = fopen("ompclockcyclespership.csv","a");
	fprintf(clks, "Total Number of Clock Cycles\n");
	fprintf(clks, "Number of Ships,");
	fprintf(clksPerShip, "Clock Cycles per Ship\n");
	fprintf(clksPerShip, "Number of Ships,");
	for (trial = 0; trial < 20; trial++) {
		fprintf(clks,"T%u,",trial+1);
		fprintf(clksPerShip,"T%u,",trial+1);
	}
	fprintf(clks,"\n");
	fprintf(clksPerShip,"\n");
	for (size = 1; size <= 10000; size*=10) {
		fprintf(clks,"%u,",size*2);
		fprintf(clksPerShip,"%u,",size*2);
		fclose(clks);
		fclose(clksPerShip);
		printf("\n%u Ship Trials\n", size*2);
		for (trial = 0; trial < 20; trial++) {
			fightMain(size,size);
		}	
		clks = fopen("ompclockcycles.csv","a");
		clksPerShip = fopen("ompclockcyclespership.csv","a");
		fprintf(clks,"\n");
		fprintf(clksPerShip,"\n");
	}
	return 1;
}
