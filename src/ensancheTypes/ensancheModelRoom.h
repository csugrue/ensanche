/*
 *  ensancheModelRoom.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/6/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

/*
- create walls 
*/
#include "ofEvents.h"
#include "polyEditable.h"
#include "ensancheBuilding.h"

class EnsancheModelRoom{

	public:
		EnsancheModelRoom();
		~EnsancheModelRoom();
		
		void setup();
		void clear();
		void draw();
		
		void setFromPolygon(polyEditable poly, float wallHeight, bool bOffset = true  );
		void setFromBuilding(EnsancheBuilding * building, float wallHeight, bool bOffset = true );
		
		int getNumPts(){ return nPts;}
		int getNumFloorPts(){ return nFloorPts; }
		
		void mouseDragged(ofMouseEventArgs& event);
		void mousePressed(ofMouseEventArgs& event);
	
		void enable(){  bEnabled = true; }
		void disable(){ bEnabled = false; }
	
		bool bEnabled;
		bool bDrawFloor, bDrawCeiling;
		ofPoint offSet;
		ofPoint center;
		ofPoint rotation;
		
	protected:
	
		int nPts;
		int nFloorPts;
		float * pts;
		float * floorPts;
		ofPoint lastMouse;
	
};