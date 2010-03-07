/*
 *  ensancheModelRoom.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/6/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheModelRoom.h"

EnsancheModelRoom::EnsancheModelRoom()
{
	bool bDrawFloor = false;
	bool bDrawCeiling = true;
	float * pts[3];
	float * floorPts[3];
	
	offSet.set(0,0);
	center.set(0,0);
	rotation.set(0,0,0);
	lastMouse.set(0,0);
	
	nPts		= 0;
	nFloorPts	= 0;
}

EnsancheModelRoom::~EnsancheModelRoom()
{
	if( nPts > 0 )		delete [] pts;
	if(nFloorPts > 0)	delete [] floorPts;
	
	ofRemoveListener(ofEvents.mousePressed, this, &EnsancheModelRoom::mousePressed);
	ofRemoveListener(ofEvents.mouseDragged, this, &EnsancheModelRoom::mouseDragged);
}

void EnsancheModelRoom::setup()
{
	ofAddListener(ofEvents.mousePressed, this, &EnsancheModelRoom::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &EnsancheModelRoom::mouseDragged);
	
}

void EnsancheModelRoom::clear()
{
	
	nPts = 0;
	nFloorPts = 0;
	offSet.set(0,0);
	center.set(0,0);
	rotation.set(0,0,0);
}

void EnsancheModelRoom::draw()
{
	int nDrawPts = nPts/3.f;
	
	glPushMatrix();
		glTranslatef(center.x,0,center.y);
		glRotatef(rotation.x,1,0,0);
		glRotatef(rotation.y,0,1,0);
		glRotatef(rotation.z,0,0,1);
		glTranslatef(-center.x,0,-center.y);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		
			glVertexPointer(3, GL_FLOAT, 0, pts);
			//glColorPointer( 4, GL_FLOAT, 0, color);
		
			//glDrawArrays(GL_TRIANGLE_FAN, 0, nPts);
			glDrawArrays(GL_QUADS, 0, nDrawPts);
		
		//glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
	glPopMatrix();
}

void EnsancheModelRoom::setFromPolygon( polyEditable poly, float wallHeigh, bool bOffset  )
{

}

void EnsancheModelRoom::setFromBuilding( EnsancheBuilding * building, float wallHeight, bool bOffset )
{
	clear();
	
	// allocate memory for all walls
	int total = building->walls.size() * 4 * 3;
	pts = new float[total];
	
	ofRectangle boundingBox = ofRectangle(0,0,0,0);
	if(bOffset) building->buildingPoly.getBoundingBox();
	
	center = building->buildingPoly.getCentroid();
	
	for( int i = 0; i < building->walls.size(); i++)
	{
		
		pts[nPts] = building->walls[i].pts[0].x-boundingBox.x;
		pts[nPts+1] = 0.f;
		pts[nPts+2] = building->walls[i].pts[0].y-boundingBox.y;
		nPts += 3;
		
		pts[nPts] = building->walls[i].pts[1].x-boundingBox.x;
		pts[nPts+1] = 0.f;
		pts[nPts+2] = building->walls[i].pts[1].y-boundingBox.y;
		nPts += 3;
		
		pts[nPts] = building->walls[i].pts[1].x-boundingBox.x;
		pts[nPts+1] = wallHeight;
		pts[nPts+2] = building->walls[i].pts[1].y-boundingBox.y;
		nPts += 3;
		
		pts[nPts] = building->walls[i].pts[0].x-boundingBox.x;
		pts[nPts+1] = wallHeight;
		pts[nPts+2] = building->walls[i].pts[0].y-boundingBox.y;
		nPts += 3;  
	}
	
}


void EnsancheModelRoom::mouseDragged(ofMouseEventArgs& event)
{
	if(!bEnabled) return;
	
	if( event.button != 0 )
	{
		rotation.x += event.y-lastMouse.y;
	}else{
		rotation.y += event.x-lastMouse.x;
	}
	
	lastMouse.set( event.x, event.y );
}

void EnsancheModelRoom::mousePressed(ofMouseEventArgs& event)
{
	if(!bEnabled) return;
	lastMouse.set( event.x, event.y );
}

