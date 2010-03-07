/*
 *  polyAdv.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 1/15/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "polyAdv.h"

polyAdv::polyAdv()
{
	polySimple::polySimple();
	nPts = 0;
}

polyAdv::~polyAdv()
{
}

/*polyAdv::polyAdv(  const polyAdv &  mom )
{
	pts.clear();
	for( int i = 0; i < pts.size(); i++) pts.push_back(mom.pts[i]);				
	nPts = mom.nPts;
}*/

bool polyAdv::bHitTest( float x, float y)
{
	
	return pointInPolygon(x, y, pts);
}

void polyAdv::createFromRect( ofRectangle rect )
{
	pts.clear();
	pushVertex( ofPoint(rect.x, rect.y) );
	pushVertex( ofPoint(rect.x, rect.y+rect.height) );
	pushVertex( ofPoint(rect.x+rect.width, rect.y+rect.height) );
	pushVertex( ofPoint(rect.x+rect.width, rect.y) );
	
}

