#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int sync_time(char * line) {
	char status;
	struct tm tm;
	time_t timestamp;
	struct timeval timeval;
	
	/* Dekodujemy czas podanych przez GPSa */
	status = line[18];	
	line[63] = '\0';
	tm.tm_year = atoi(&line[61]) + 2000 - 1900;
	line[61] = '\0';
	tm.tm_mon = atoi(&line[59]) - 1;
	line[59] = '\0';
	tm.tm_mday = atoi(&line[57]);
	line[13] = '\0';
	tm.tm_sec = atoi(&line[11]);
	line[11] = '\0';
	tm.tm_min = atoi(&line[9]);
	line[9] = '\0';
	tm.tm_hour = atoi(&line[7]);
	tm.tm_isdst = -1; 
	
	timestamp = mktime(&tm);
	if (timestamp < 0) {
		return 0;
	}
	
	if (tm.tm_isdst > 0)
		timestamp += 3600;

	timeval.tv_sec = timestamp - timezone;
	timeval.tv_usec = 0;
	settimeofday(&timeval, NULL);
	
	if (status == 'A')
		return 2000;
	else
		return 20;
}

int main(int argc, char * argv[]) {
	char line[255];
	int len;
	int done;
	
	FILE * fp;
	
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [gpsTTY]\n", argv[0]);
		return 1;
	}
	
	if (!(fp = fopen(argv[1], "r"))) {
		perror(argv[1]);
		return 2;
	}
	
	done = 0;
	
	while(1) {
		fgets(line, sizeof(line), fp);
		len = strlen(line);
		if (write(1, line, len) < 0)
			break;
		
		/* Synchronizacja czasu z GPSem */
		if ((--done <= 0) && (len == 72) && (!strncmp(line, "$GPRMC,", 7))) {
			done = sync_time(line);
		}		
	}
	
	fclose(fp);
	return 0;
}
