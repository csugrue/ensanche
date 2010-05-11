/*
 *  ensancheBarrio.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/4/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheBarrio.h"

EnsancheBarrio::EnsancheBarrio()
{
}

EnsancheBarrio::~EnsancheBarrio()
{
	clear();
}

EnsancheBarrio::EnsancheBarrio( const EnsancheBarrio & mom )
{
	buildings.assign(mom.buildings.begin(),mom.buildings.end());
	boundingBox = mom.boundingBox;
}

void EnsancheBarrio::clear()
{
	for( int i =0; i < buildings.size(); i++)
		buildings[i].clear();
	
	//for( int i =0; i < buildings.size(); i++)
	//	delete buildings[i];
	
	buildings.clear();
	
}

void EnsancheBarrio::setup()
{
}

void EnsancheBarrio::draw()
{
	for( int i =0; i < buildings.size(); i++)
		buildings[i].draw();
}

void EnsancheBarrio::loadFromXml( string filename)
{
	cout << "loading barrio data from " << filename << endl;
	
	ofxXmlSettings xml;
	xml.loadFile(filename);
	
	if( xml.pushTag("ensanche") )
	{
		if( xml.pushTag("exterior") )
		{
			if( xml.pushTag("architecture") )
			{
				int nBuildings = xml.getNumTags("building");
				cout << "barrio loading " << nBuildings << " buildings" << endl;
				
				if( nBuildings > 0 )
				{
					clear();
					
					for( int i = 0; i < nBuildings; i++)
					{
						// new building
						addBuilding();
						int nB =  buildings.size()-1;
						
						buildings[nB].nFloors = xml.getAttribute("building","nFloors",1,i);
						buildings[nB].uid = xml.getAttribute("building","uid",getUid(),i);
						
						// add walls
						xml.pushTag("building", i);
							
							xml.pushTag("floor",0);
							int nWalls = xml.getNumTags("wall");
							
							for( int j = 0; j < nWalls; j++)
							{
								ofPoint m1,m2;
								int uid = xml.getAttribute("wall","uid",0,j);
								xml.pushTag("wall",j);
									m1.x = xml.getAttribute("pt","x",0.f,0);
									m1.y = xml.getAttribute("pt","y",0.f,0);
									m2.x = xml.getAttribute("pt","x",0.f,1);
									m2.y = xml.getAttribute("pt","y",0.f,1);
								xml.popTag();
								
								buildings[nB].addWall(m1,m2,uid);
							}
							
							buildings[nB].addClosingWall();
							xml.popTag();
						xml.popTag();
						
						
					}
					
				}
				xml.popTag(); // arch
			}
			
			xml.popTag(); //exterior
		}
		xml.popTag();
	}
	
}

void EnsancheBarrio::addBuilding()
{
	buildings.push_back( EnsancheBuilding());
}

void EnsancheBarrio::addBuilding(EnsancheBuildingGroup * mapBuildings, float pixelsPerMeter)
{
	
	//buildings.clear();
	
	EnsancheMapBuilding tempMapB;
	int t = 0;
	for( int i =0; i < mapBuildings->buildings.size(); i++)
	{
		if(!mapBuildings->buildings[i]->bClosed) continue;
//		EnsancheBuilding tempB;
		buildings.push_back( EnsancheBuilding());

		tempMapB.clear();
		// copy pts
		for( int j= 0;j<mapBuildings->buildings[i]->pts.size();j++)
			tempMapB.pushVertex(mapBuildings->buildings[i]->pts[j]);
		
		// copy translation + rotation
		tempMapB.transPreRotate = mapBuildings->buildings[i]->transPreRotate;
		tempMapB.transPstRotate = mapBuildings->buildings[i]->transPstRotate;
		tempMapB.gRotation = mapBuildings->buildings[i]->gRotation;
		
		buildings[t].addWalls(tempMapB, pixelsPerMeter, false);
		buildings[t].nFloors = mapBuildings->buildings[i]->nFloors;
		t++;
	}
	
	
	ofRectangle boundingBox = getGroupBoundingBox();
	
	for( int i =0; i < buildings.size(); i++)
	{
		for( int j =0; j < buildings[i].walls.size(); j++)
		{
			buildings[i].walls[j].pts[0].x -= boundingBox.x;
			buildings[i].walls[j].pts[0].y -= boundingBox.y;
			
			buildings[i].walls[j].pts[1].x -= boundingBox.x;
			buildings[i].walls[j].pts[1].y -= boundingBox.y;
		}
		
		for( int j = 0; j < buildings[i].buildingPoly.pts.size(); j++)
		{
			buildings[i].buildingPoly.pts[j].x -= boundingBox.x;
			buildings[i].buildingPoly.pts[j].y -= boundingBox.y;
		}
		
	}

}

ofRectangle EnsancheBarrio::getGroupBoundingBox()
{
	ofPoint minPt = ofPoint(0,0);
	ofPoint maxPt = ofPoint(0,0);
	
	for( int i = 0; i < buildings.size(); i++)
	{
		//cout << "bb " << i << " " << buildings[i].buildingPoly.pts.size() << endl;
		ofRectangle bb = buildings[i].buildingPoly.getBoundingBox();
		if( i == 0 || bb.x < minPt.x ) minPt.x = bb.x;
		if( i == 0 || bb.y < minPt.y ) minPt.y = bb.y;
		if( i == 0 || bb.x+bb.width > maxPt.x ) maxPt.x = bb.x+bb.width;
		if( i == 0 || bb.y+bb.height > maxPt.y ) maxPt.y = bb.y+bb.height;
	}
	
	ofRectangle rect = ofRectangle(minPt.x,minPt.y,maxPt.x-minPt.x,maxPt.y-minPt.y);
	//cout << "min x " << rect.x << " y " << rect.y << " w " << rect.width << " h " << rect.height << endl;
	return rect;
}
