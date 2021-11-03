//Zoltan Sojtory
//2442602s
//SP Exercise 1a
//This is my own work as defined in the Academic Ethics agreement I have signed.

#include "date.h"
#include <stdlib.h>
#include <string.h>

struct date {
	//consists of three integers for comparison
	int day;
	int month;
	int year;
};

Date *date_create(char *datestr){
	//uses strtok() to split string with delimiter "/"
	struct date *d;
	char * token = strtok(datestr, "/");
	d = (struct date *)malloc(sizeof(struct date));
	d->day = atoi(token);
	if(token == NULL || d->day == 0 || d == NULL){
		free(d);
		return NULL;
	}
	token = strtok(NULL, "/");
	d->month = atoi(token);
	if(token == NULL || d->month == 0){
		free(d);
		return NULL;
	}
	token = strtok(NULL, "/");
	d->year = atoi(token);
	if(token == NULL || d->year ==0){
		free(d);
		return NULL;
	}
	return d;
}

Date *date_duplicate(Date *d){
	struct date *new_d;
	if((new_d = (struct date *)malloc(sizeof(struct date))) != NULL){
		new_d->day = d->day;
		new_d->month = d->month;
		new_d->year = d->year;
		return new_d;
	}
	else{
		free(new_d);
		return NULL;
	}
}

int date_compare(Date *date1, Date *date2){
	//difference between values gives result, with years taking highest priority
	int year_diff, month_diff, day_diff;
	year_diff = date1->year - date2->year;
	month_diff = date1->month - date2->month;
	day_diff = date1->day - date2->day;
	if(year_diff != 0){
		return year_diff;
	}
	else if(month_diff != 0){
		return month_diff;
	}
	else if(day_diff != 0){
		return day_diff;
	}
	else{
		return 0;
	}
}

void date_destroy(Date *d){
	free(d);
}
