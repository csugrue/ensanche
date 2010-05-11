/*
 *  ensancheFloorPlan.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheFloorPlan.h"

EnsancheFloorPlan::EnsancheFloorPlan()
{
	boundingbox = ofRectangle(0,0,0,0);

}

EnsancheFloorPlan::~EnsancheFloorPlan()
{
}

EnsancheFloorPlan::EnsancheFloorPlan(const EnsancheFloorPlan & mom)
{
	walls.assign( mom.walls.begin(),mom.walls.end() );
	pilars.assign( mom.pilars.begin(),mom.pilars.end() );
	windows.assign( mom.windows.begin(),mom.windows.end() );
	entries.assign( mom.entries.begin(),mom.entries.end() );
	
	// safe??
	polyLimitsTrans = mom.polyLimitsTrans;
	boundingbox = mom.boundingbox;
}

void EnsancheFloorPlan::clear()
{
	walls.clear();
	pilars.clear();
	windows.clear();
	entries.clear();
	//boundingbox.set(0,0,0,0);
}

void EnsancheFloorPlan::setup()
{
}

void EnsancheFloorPlan::draw()
{
	for( int i = 0; i < walls.size(); i++)
	{
		ofLine(walls[i].pts[0].x,walls[i].pts[0].y,walls[i].pts[1].x,walls[i].pts[1].y);
	}
	
}

//--- set from editor functions
void EnsancheFloorPlan::addWalls(polyEditable & fpLimits, float pixelsPerMeter)
{
	if( fpLimits.pts.size() <= 0 ) return;
	
	// clear walls
	walls.clear();
	
	//polySimple poly;
	polyLimitsTrans.clear();
	
	for( int i = 0; i < fpLimits.pts.size(); i++)
	{
		polyLimitsTrans.pushVertex(fpLimits.pts[i]);
	}
	
	// add in last wall
	if(fpLimits.bClosed && polyLimitsTrans.pts.size() > 0) polyLimitsTrans.pushVertex(polyLimitsTrans.pts[0]);
	
	// apply draw transformations
	for( int i = 0; i < polyLimitsTrans.pts.size(); i++)
	{
		//poly.pts[i] = getTransformsApplied( poly.pts[i],ofPoint(0,0),ofPoint(0,0),fpLimits.gRotation);//
		polyLimitsTrans.pts[i] = getTransformsApplied( polyLimitsTrans.pts[i],fpLimits.transPreRotate,fpLimits.transPstRotate,fpLimits.gRotation);
	}
	
	// get bounding box and translate to 0,0, scale to meters
	boundingbox = polyLimitsTrans.getBoundingBox();
	
	for( int i = 0; i < polyLimitsTrans.pts.size(); i++)
	{
		polyLimitsTrans.pts[i].x -= boundingbox.x;
		polyLimitsTrans.pts[i].y -= boundingbox.y;
		
		polyLimitsTrans.pts[i].x *= pixelsPerMeter;
		polyLimitsTrans.pts[i].y *= pixelsPerMeter;
	}
	
	// make walls
	cout << "en floor plan " << endl;
	for( int i = 0; i < polyLimitsTrans.pts.size()-1; i++)
	{
		enWall tempWall;
		tempWall.pts[0] = polyLimitsTrans.pts[i];
		tempWall.pts[1] = polyLimitsTrans.pts[i+1];
		tempWall.posInPoly = i;
		tempWall.uid = getUid();
		walls.push_back(tempWall);

	}
}

void EnsancheFloorPlan::addPilar(polyFixWidth * fpPilar, float pixelsPerMeter)
{
	if( fpPilar->pts.size() < 2 ) return;
	if(boundingbox.width <= 0) return;
	
	ofPoint pt0 = ofPoint( fpPilar->pts[0].x, fpPilar->pts[0].y);
	ofPoint pt1 = ofPoint(fpPilar->pts[1].x,fpPilar->pts[1].y);
	
	pt0 = getTransformsApplied( pt0,fpPilar->transPreRotate,fpPilar->transPstRotate,fpPilar->gRotation);
	pt1 = getTransformsApplied( pt1,fpPilar->transPreRotate,fpPilar->transPstRotate,fpPilar->gRotation);
	
	pt0.x -= boundingbox.x;
	pt0.y -= boundingbox.y;
	
	pt1.x -= boundingbox.x;
	pt1.y -= boundingbox.y;
	
	pt0.x *= pixelsPerMeter;
	pt0.y *= pixelsPerMeter;
	
	pt1.x *= pixelsPerMeter;
	pt1.y *= pixelsPerMeter;
	
	enPilar tempPilar;
	tempPilar.pts[0] = pt0;
	tempPilar.pts[1] = pt1;
	tempPilar.width = fpPilar->width*pixelsPerMeter;
	tempPilar.uid = getUid();
	
	pilars.push_back( tempPilar );
}

void EnsancheFloorPlan::addWindow(polyFixWidth * fpWindow, windowToWall closeWindowWall,float pixelsPerMeter)
{
	if( fpWindow->pts.size() < 2 ) return;
	if(boundingbox.width <= 0) return;
	
	
		ofPoint pt0 = ofPoint( fpWindow->pts[0].x, fpWindow->pts[0].y);
		ofPoint pt1 = ofPoint(fpWindow->pts[1].x,fpWindow->pts[1].y);
		
		pt0 = getTransformsApplied( pt0,fpWindow->transPreRotate,fpWindow->transPstRotate,fpWindow->gRotation);
		pt1 = getTransformsApplied( pt1,fpWindow->transPreRotate,fpWindow->transPstRotate,fpWindow->gRotation);
		
		pt0.x -= boundingbox.x;
		pt0.y -= boundingbox.y;
		
		pt1.x -= boundingbox.x;
		pt1.y -= boundingbox.y;
		
		pt0.x *= pixelsPerMeter;
		pt0.y *= pixelsPerMeter;
		
		pt1.x *= pixelsPerMeter;
		pt1.y *= pixelsPerMeter;
		
		enWindow tempWindow;
		tempWindow.pts[0] = pt0;
		tempWindow.pts[1] = pt1;
		tempWindow.width = fpWindow->width*pixelsPerMeter;
		tempWindow.uid = getUid();
		tempWindow.wallIndex = closeWindowWall.wallindex;

		windows.push_back( tempWindow );
	
	
}

void EnsancheFloorPlan::addEntry(EnsancheRoomLabel fpEntry, float pixelsPerMeter)
{
}

ofPoint	EnsancheFloorPlan::getTransformsApplied(ofPoint me, ofPoint pre, ofPoint pst, float rotation)
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

void EnsancheFloorPlan::loadFromXml( string filename)
{
	ofxXmlSettings xml;
	xml.loadFile(filename);
	
	if( xml.pushTag("ensanche") )
	{
		if( xml.pushTag("floorplan") )
		{
			if( xml.pushTag("architecture") )
			{
				int nWalls = xml.getNumTags("wall");
				cout << "TOTAL WALLS " << nWalls << endl;
				
				for( int i = 0; i < nWalls; i++)
				{
					enWall tempWall;
					tempWall.uid = xml.getAttribute("wall","uid",0,i);
					
					xml.pushTag("wall", i);
						
						tempWall.pts[0].x = xml.getAttribute("pt","x",0.f,0);
						tempWall.pts[0].y = xml.getAttribute("pt","y",0.f,0);
						tempWall.pts[1].x = xml.getAttribute("pt","x",0.f,1);
						tempWall.pts[1].y = xml.getAttribute("pt","y",0.f,1);
					xml.popTag();
					
					walls.push_back(tempWall);
				}
				xml.popTag();
			}
			   
			xml.popTag();
		}
		
		xml.popTag();
	}
	
}

