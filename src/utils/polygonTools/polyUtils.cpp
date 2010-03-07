/*
 *  polyUtils.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 1/17/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "polyUtils.h"

bool pointInPolygon(float x, float y, vector<ofPoint>pts)
{
	
	// code via Zach Lieberman via Randolph Franklin...
	int i, j, c = 0;
	int nPts = pts.size();
	
	for (i = 0, j = nPts-1; i < nPts; j = i++) {
		if ((((pts[i].y <= y) && (y < pts[j].y)) ||
			 ((pts[j].y <= y) && (y < pts[i].y))) &&
			(x < (pts[j].x - pts[i].x) * (y - pts[i].y) / (pts[j].y - pts[i].y) + pts[i].x))
			c = !c;
	}
	return (bool) c;
}


bool isInsideRect(float x, float y, ofRectangle rect){
    return ( x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height );
}


bool intersectionTwoLines( ofPoint pt1, ofPoint pt2, ofPoint pt3, ofPoint pt4, ofPoint * iPt )
{
	
    iPt->x = 0;
    iPt->y = 0;
	
    float ua =  ( (pt4.x-pt3.x)*(pt1.y-pt3.y) - (pt4.y-pt3.y)*(pt1.x-pt3.x) ) /
	( (pt4.y-pt3.y)*(pt2.x-pt1.x) - (pt4.x-pt3.x)*(pt2.y-pt1.y) );
	
    float ub =  ( (pt2.x-pt1.x)*(pt1.y-pt3.y) - (pt2.y-pt1.y)*(pt1.x-pt3.x) ) /
	( (pt4.y-pt3.y)*(pt2.x-pt1.x) - (pt4.x-pt3.x)*(pt2.y-pt1.y) );
	
	
    if( ua > 0 && ua < 1 && ub > 0 && ub < 1)
        ;
    else
        return false;
	
    iPt->x = pt1.x + ua * (pt2.x-pt1.x);
    iPt->y = pt1.y + ua * (pt2.y-pt1.y);
	
    return true;
}
