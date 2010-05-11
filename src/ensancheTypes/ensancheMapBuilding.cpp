/*
 *  ensancheBuilding.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 1/26/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheMapBuilding.h"

EnsancheMapBuilding::EnsancheMapBuilding()
{
	polyEditable::polyEditable();
	nFloors = 0;
	type = EN_BUILDING_NEIGHBOR;
	uid = getUid();
}

EnsancheMapBuilding::EnsancheMapBuilding( const EnsancheMapBuilding & mom )
{
	polyEditable::polyEditable((polyEditable)mom);
	nFloors = mom.nFloors;
	type = mom.type;
	uid = mom.uid;
}

void EnsancheMapBuilding::drawData( )
{
	
	if( bClosed )
	{
		ofPoint center = getCentroid();
		ofFill();
		ofSetColor(255,255,255,180);
		ofRect(center.x-2,center.y-12,80,16);
		
		ofSetColor(0,0,0,255);
		ofDrawBitmapString( "Floors: " + ofToString(nFloors), center.x, center.y);
	}
}
