/*
 *  ensancheExpandData.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 5/12/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheExpandData.h"

EnsancheExpandData::EnsancheExpandData()
{
}

EnsancheExpandData::~EnsancheExpandData()
{
}

EnsancheExpandData::EnsancheExpandData( const EnsancheExpandData & mom )
{
	clear();
	
	sideIds.assign( mom.sideIds.begin(), mom.sideIds.end() );
	ends.pts.assign( mom.ends.pts.begin(), mom.ends.pts.end() );
	ends.ids.assign( mom.ends.ids.begin(), mom.ends.ids.end() );
	ends.sideToEndPt = mom.ends.sideToEndPt;
	building = mom.building;
	buildingEndsOnly = mom.buildingEndsOnly;
	buildingExpanded = mom.buildingExpanded;
	
}

void EnsancheExpandData::clearEnds()
{
	ends.pts.clear();
	ends.ids.clear();
	ends.sideToEndPt.clear();
}

void EnsancheExpandData::setup(EnsancheBuilding & origBuilding)
{
	building = origBuilding;
	buildingEndsOnly = origBuilding;
	buildingExpanded = origBuilding;
	
	cout << "setup exdata " << endl;
	
}

void EnsancheExpandData::clear()
{
	sideIds.clear();
	clearEnds();
	building.clear();
	buildingEndsOnly.clear();
	buildingExpanded.clear();
}

void EnsancheExpandData::drawEnds(float scale, ofTrueTypeFont * font)
{
		ofSetColor(0,0,0);
		
		for( int j = 0; j < ends.pts.size(); j++)
		{
			glPushMatrix();
				glTranslatef(ends.pts[j].x,ends.pts[j].y,0);
				glScalef(1/scale,1/scale,1);
				font->drawString( ofToString(ends.ids[j]),0,0  );
			glPopMatrix();
		}
			
}

void EnsancheExpandData::drawSideIds()
{
	ofNoFill();
	for( int i = 0; i < building.buildingPoly.pts.size(); i++)
	{
		if(sideIds.size() > i )
		{
			
			switch(sideIds[i])
			{
				case 0: ofSetColor(255,0,0); break;
				case 1: ofSetColor(0,255,0); break;
				case 2: ofSetColor(0,0,255); break;
				case 3: ofSetColor(255,0,255); break;
				case -2: ofSetColor(0,255,255); break;
				default: ofSetColor(0,0,0); break;
					
			}
	
		}
		
		int nxt = (i==building.buildingPoly.pts.size()-1) ? 0 : i+1;
		ofLine(building.buildingPoly.pts[i].x,building.buildingPoly.pts[i].y,building.buildingPoly.pts[nxt].x,building.buildingPoly.pts[nxt].y);
	}
	
}
