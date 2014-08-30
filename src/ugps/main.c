#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

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
	
    ptr = line;
    items[count++] = ptr;
    while(*ptr) {
        if (*ptr == ',') {
            *ptr = '\0';
            items[count++] = ptr + 1;
        }
        ptr++;
    }

    //printf ("count=%d\n", count);

    if (count != 12) { /* Brakuje jakichś danych, brak fixa? */
		return;
	}
	
	/* Godzina */
	tmp = atoi(items[GPS_IDX_TIME]);
	tm.tm_sec = tmp % 100;
	tmp /= 100;
	tm.tm_min = tmp % 100;
	tm.tm_hour = tmp / 100;
	
    //printf("h=%d,m=%d,s=%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
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
    tm.tm_mon = (tmp % 100) - 1;
	tm.tm_mday = tmp / 100;
	tm.tm_isdst = -1; 
    //printf("d=%d,m=%d,y=%d\n", tm.tm_mday, tm.tm_mon, tm.tm_year);

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
            set_time();
            if (_gps_isvalid)
				resync_timeout = 2000;
            else
                resync_timeout = 0;
		}
		else {
			resync_timeout--;
		}
	}
	
	fclose(fp);
	return 0;
}
