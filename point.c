#include <assert.h>
#include "common.h"
#include "point.h"
#include <math.h>

void
point_translate(struct point *p, double x, double y)
{
	// translate each dimension by required value
	p->x += x;
	p->y += y;
}

double
point_distance(const struct point *p1, const struct point *p2)
{
	// use eucledian distance formula
	double distance = sqrt(((p2->x - p1->x) * (p2->x - p1->x)) + ((p2->y - p1->y) * (p2->y - p1->y)));
	return distance;
}

int
point_compare(const struct point *p1, const struct point *p2)
{
	double len1, len2;
	len1 = sqrt(p1->x * p1->x + p1->y * p1->y);
	len2 = sqrt(p2->x * p2->x + p2->y * p2->y);

	if(len1 < len2){
		return -1;
	}
	else if(len1 == len2){
		return 0;
	}
	else{
		return 1;
	}
}
