/*
 *  ensancheModelBuilding.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/8/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

/*
Contains a 2D and 3D representation of a building.
Possible to render as 3D model.
Each floor can be different.
*/

#pragma once

#include "ofEvents.h"
#include "polyEditable.h"
#include "ensancheBuilding.h"

#define MODEL_EDIT_MODE_NORMAL 0
#define MODEL_EDIT_MODE_SHIFT 1

#define MODEL_DRAW_MODE_3D	0
#define MODEL_DRAW_MODE_2D	1

/*
possibility to texture parts differently
each wall marked as a type and apply that texture accordingly
define different texture types

*/

class EnsancheModelBuilding{
	
	public:
		EnsancheModelBuilding();
		~EnsancheModelBuilding();
		EnsancheModelBuilding( const EnsancheModelBuilding & mom );
		
		void setup();
		void clear();
		
		void draw( int drawMode = MODEL_DRAW_MODE_3D, bool bDrawWOffset = false );
		void draw2D(bool bDrawWOffset);
		void draw3D(bool bDrawWOffset);
		
		// setup the model from the original building (with nFloor info + offsets etc.) 
		void setupFromBuilding(EnsancheBuilding buildingOriginal );
		
		// set new floor (expansion, facade etc)
		void setFloor(EnsancheBuilding buildingFloor, int floorNum );
		
		// add a floor to building
		void addBuildingFloor(polySimple & buildingPoly,  bool bOffset = true );
		
		// generate 3D model from current data
		void generateModel( float wallHeight );
		
		int getNumPts(){ return nPts;}
		int getNumFloorPts(){ return nFloorPts; }
		
		void mouseDragged(ofMouseEventArgs& event);
		void mousePressed(ofMouseEventArgs& event);
		
		void enable(){  bEnabled = true; }
		void disable(){ bEnabled = false; }
		
		void setWallTexture(ofTexture * tex);
		
		//---
		bool bEnabled;
		ofPoint offSet;
		ofPoint center;
		ofPoint rotation;
		float scale;
		int editMode;
		
		int nFloors;
		
		//--- 2d representation
		vector<polySimple> buildingFloors;
		
		//--- texture
		ofTexture * textureWall;
		bool bSetWallTexture;
		
	protected:
		
		//--- for 3d rendering
		int nPts;
		int nFloorPts;
		int nTexPts;
		float * pts;
		float * floorPts;
		float * texPts;
		ofPoint lastMouse;
		

};