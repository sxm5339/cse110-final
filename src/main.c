#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#define PIN 17

#define MAX 25

FILE *fp;

int inputTemp(int temps[], int min, int max);
void changeTempRange(int* min, int* max);
void calcAvg(int temps[], int max);
void displayStat(int temps[], int range, int min, int max);
void saveToFile(int temps[], int range, int min, int max);

void clearFile(void);

int inRange(int value, int min, int max) {
	if((value >= min) && (value <= max)) {
		return 0; //NORMAL
	}else {
		return 1; //WARNING
	}
}


int main(int argc, char* argv[]) {
	gpioInitialise();
	gpioSetMode(PIN, PI_OUTPUT);
	//gpioWrite(PIN, 1);
	//getchar();
	gpioWrite(PIN, 0);
	//getchar();
	//
	
	
	int input, amountInArray;
	int tempMin = 68, tempMax = 76;
	
	int temps[MAX] = {};
	
	printf("1. input temperature \n 2. Change temp. range\n 3. Calculate avg.\n 4. Display status\n 5. Save readings & status\n 6. Clear text file\n  7. Exit\n");
	scanf("%d", &input);
	
	while(input != 7) {
		switch(input) {
			case 1: 
				amountInArray = inputTemp(temps, tempMin, tempMax);
				break;
			case 2:
				changeTempRange(&tempMin, &tempMax);
				break;
			case 3:
				calcAvg(temps, amountInArray);
				break;
			case 4:
				displayStat(temps, amountInArray, tempMin, tempMax);
				break;
			case 5:
				saveToFile(temps, amountInArray, tempMin, tempMax);
				break;
			case 6:
				clearFile();
				break;
			case 7:
				return 0;
			default:
				break;
		}
		
		printf("\n\n1. input temperature \n 2. Change temp. range\n 3. Calculate avg.\n 4. Display status\n 5. Save readings & status\n 6. Clear text file\n  7. Exit\n");
		scanf("%d", &input);
		printf("\n");
	
	}
	
	gpioWrite(PIN, 0);
	gpioTerminate();
	return 0;
}


int inputTemp(int temps[], int min, int max) {
	int input;
	
	printf("Enter Classroom temperature reading in F, press 0 to quit: \n");
	
	for(int i = 0; i < MAX; ++i) {
		
		printf("%d. ", i + 1);
		scanf("%d", &input);
		
		if(input == 0) {
			return i;
		}else {
			temps[i] = input;
			if(inRange(temps[i], min, max)) {
				gpioWrite(PIN, 1);
			}
		}
	}
	return MAX;
}

void changeTempRange(int* min, int* max) {
	printf("Current temperature range %dF - %dF\n", *min, *max);
	printf("Change temp., input min and max (inclusive): ");
	scanf("%d %d", min, max);
	
	printf("New temperature range is %dF - %dF\n", *min, *max);
}

void calcAvg(int temps[], int max) {
	int total = 0;
	
	for(int i = 0; i < max; ++i) {
		total += temps[i];
	}
	
	printf("The average temperature is: %.2f\n", ((float)total)/max);
}

void displayStat(int temps[], int range, int min, int max) {
	
	printf("Temperature    System Status    Warning LED\n");
	
	for(int i = 0; i < range; ++i) {
		printf("%dF	  | ", temps[i]);
		
		if((temps[i] >= min) && (temps[i] <= max)) {
			printf("  NORMAL    |  OFF\n");
		}else {
			printf("  WARNING   |  ON\n"); 
		}
	}
}		

void saveToFile(int temps[], int range, int min, int max) {
	fp = fopen("../status.txt", "w");
	
	fputs("Temperature    System Status    Warning LED\n", fp);
	
	for(int i = 0; i < range; ++i) {
		fprintf(fp, "%dF	  | ", temps[i]);
		
		if((temps[i] >= min) && (temps[i] <= max)) {
			fputs("  NORMAL    |  OFF\n", fp);
		}else {
			fputs("  WARNING   |  ON\n", fp); 
		}
	}
	fclose(fp);
}

void clearFile(void) {
	fp = fopen("../status.txt", "w");
	
	fclose(fp);
}
