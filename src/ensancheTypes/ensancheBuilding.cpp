/*
 *  ensancheBuilding.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/4/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheBuilding.h"

EnsancheBuilding::EnsancheBuilding()
{
	nFloors = 1;
	uid = getUid();
}

EnsancheBuilding::~EnsancheBuilding()
{
}

EnsancheBuilding::EnsancheBuilding( const EnsancheBuilding & mom )
{
	walls.assign(mom.walls.begin(),mom.walls.end());
	buildingPoly = mom.buildingPoly;
	
	nFloors = mom.nFloors;
	uid = mom.uid;
	
}

void EnsancheBuilding::clear()
{
	nFloors = 1;
	walls.clear();
	buildingPoly.clear();
}

void EnsancheBuilding::setup()
{
}

void EnsancheBuilding::draw()
{
	ofBeginShape();
	for( int i = 0; i < walls.size(); i++)
	{
		//ofLine(walls[i].pts[0].x,walls[i].pts[0].y,walls[i].pts[1].x,walls[i].pts[1].y);
		ofVertex(walls[i].pts[0].x,walls[i].pts[0].y);
		ofVertex(walls[i].pts[1].x,walls[i].pts[1].y);
	}
	ofEndShape(true);
	
	//buildingPoly.draw();
}

/*void EnsancheBuilding::loadFromXml( string filename )
{
	
}*/

void EnsancheBuilding::addWall( ofPoint m1, ofPoint m2, int uid )
{
	enWall tempWall;
	tempWall.pts[0] = m1;
	tempWall.pts[1] = m2;
	tempWall.posInPoly = buildingPoly.pts.size();
	tempWall.type = 0;
	tempWall.uid = getUid();
	
	walls.push_back(tempWall);
	
	buildingPoly.pushVertex(tempWall.pts[0]);
	
	
}

void EnsancheBuilding::addClosingWall()
{
	if( walls.size() > 1 )
	{
		ofPoint m1,m2;
		
		m1 = walls[walls.size()-1].pts[ 1 ];
		m2 = walls[0].pts[0];
		
		string uid = getUid(); // get Uid();
		
		enWall tempWall;
		
		tempWall.pts[0] = m1;
		tempWall.pts[1] = m2;
		tempWall.posInPoly = buildingPoly.pts.size();
		tempWall.type = 0;
		tempWall.uid = uid;
		
		walls.push_back(tempWall);
		
		buildingPoly.bClosed = true;
		buildingPoly.pushVertex(m1);
	}
	
}

void EnsancheBuilding::addWalls(EnsancheMapBuilding & mapBuilding, float pixelsPerMeter, bool bRemoveBounding)
{
	
	nFloors = mapBuilding.nFloors;
	
	// copy polygon
	buildingPoly.clear();
	for( int i = 0; i < mapBuilding.pts.size(); i++)
	{
		ofPoint npt = getTransformsApplied(mapBuilding.pts[i], mapBuilding.transPreRotate, mapBuilding.transPstRotate, mapBuilding.gRotation);
		buildingPoly.pushVertex(npt);
	}
	
	
	ofRectangle boundingbox = buildingPoly.getBoundingBox();
	
	for( int i = 0; i < buildingPoly.pts.size(); i++)
	{
		if( bRemoveBounding)
		{
			buildingPoly.pts[i].x -= boundingbox.x;
			buildingPoly.pts[i].y -= boundingbox.y;
		}
		
		buildingPoly.pts[i].x *= pixelsPerMeter;
		buildingPoly.pts[i].y *= pixelsPerMeter;
	}
	
	// copy walls
	walls.clear();
	for( int i = 0; i < buildingPoly.pts.size()-1; i++)
	{
		enWall tempWall;
		tempWall.pts[0] = buildingPoly.pts[i];
		tempWall.pts[1] = buildingPoly.pts[i+1];
		tempWall.posInPoly = i;
		tempWall.uid = getUid();//getuId();
		tempWall.type = 0;
		walls.push_back(tempWall);
	}
}




ofPoint	EnsancheBuilding::getTransformsApplied(ofPoint me, ofPoint pre, ofPoint pst, float rotation)
{
	// add in translation
	me.x += pst.x;
	me.y += pst.y;
	
	// add in rotation
	ofxVec2f pt0 = ofxVec2f(me.x,me.y);		
	pt0.rotate(rotation);
	me.set(pt0.x,pt0.y);
	
	// add second translation
	me.x += pre.x;
	me.y += pre.y;
	
	return me;
}
