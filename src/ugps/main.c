#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

time_t _gps_time;
#if 0
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
#endif

#define GPS_IDX_TIME       0
#define GPS_IDX_STATUS     1
#define GPS_IDX_LATITUDE   2
#define GPS_IDX_NS         3
#define GPS_IDX_LONGITUDE  4
#define GPS_IDX_EW         5
#define GPS_IDX_SPEED      6
#define GPS_IDX_DIRECTION  7
#define GPS_IDX_DATE       8

char _gps_isvalid;
double _gps_latitude;
double _gps_longitude;
char _gps_ns;
char _gps_ew;
double _gps_speed;
double _gps_course;
time_t _gps_time;

void parse_gprmc(char * line) {
	struct tm tm;
	char * ptr;
	int count = 0;
	char * items[20];
	int tmp;
	
	if (strncmp(line, "$GPRMC,", 7)) {
		return;
	}
	
	line += 7;
	
	ptr = strtok(line, ",");
	while(ptr != NULL) {
		items[count++] = ptr;
		ptr = strtok(NULL, ",");
	}
	
	if (count != 10) { /* Brakuje jakichś danych, brak fixa? */
		return;
	}
	
	/* Godzina */
	tmp = atoi(items[GPS_IDX_TIME]);
	tm.tm_sec = tmp % 100;
	tmp /= 100;
	tm.tm_min = tmp % 100;
	tm.tm_hour = tmp / 100;
	
	/* Status */
	_gps_isvalid = (items[GPS_IDX_STATUS][0] == 'A');
	
	/* Szerogokośc geograficzna */
	_gps_latitude = atof(items[GPS_IDX_LATITUDE]);

	/* Północ / Południe */
	_gps_ns = items[GPS_IDX_NS][0];
	
	/* Długość geograficzna */
	_gps_longitude = atof(items[GPS_IDX_LONGITUDE]);
	
	/* Wschód / zachód */
	_gps_ew = items[GPS_IDX_EW][0];
	
	/* Szybkość */
	_gps_speed = atof(items[GPS_IDX_SPEED]);
	
	/* Kierunek */
	_gps_course = atof(items[GPS_IDX_DIRECTION]);
	
	/* Data */
	tmp = atoi(items[GPS_IDX_DATE]);	
	tm.tm_year = (tmp % 100) + 2000 - 1900;
	tmp /= 100;
	tm.tm_mon = tmp % 100;
	tm.tm_mday = tmp / 100;
	tm.tm_isdst = -1; 
	
	/* Data i czas na timestamp */
	_gps_time = mktime(&tm);
	if (_gps_time < 0) {
		return;
	}
	
	if (tm.tm_isdst > 0)
		_gps_time += 3600;
}

void set_time(void) {
	struct timeval timeval;
	timeval.tv_sec = _gps_time - timezone;
	timeval.tv_usec = 0;
	settimeofday(&timeval, NULL);
}

int main(int argc, char * argv[]) {
	char line[255];
	int len;
	FILE * fp;
	int resync_timeout = 0;
	
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [gpsTTY]\n", argv[0]);
		return 1;
	}
	
	if (!(fp = fopen(argv[1], "r"))) {
		perror(argv[1]);
		return 2;
	}
	
	while(!feof(fp)) {
		fgets(line, sizeof(line), fp);
		len = strlen(line);
		if (write(1, line, len) < 0)
			break;
		
		if (!strncmp(line, "$GPRMC", 6)) {
			parse_gprmc(line);
		}
		
		if (resync_timeout <= 0) {
			if (_gps_isvalid) {
				set_time();
				resync_timeout = 2000;
			}
		}
		else {
			resync_timeout--;
		}
	}
	
	fclose(fp);
	return 0;
}
