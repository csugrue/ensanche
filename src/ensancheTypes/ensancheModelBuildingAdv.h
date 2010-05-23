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
#include "ensancheConstants.h"

#define MODEL_EDIT_MODE_NORMAL 0
#define MODEL_EDIT_MODE_SHIFT 1

#define MODEL_DRAW_MODE_3D	0
#define MODEL_DRAW_MODE_2D	1

#define MODEL_T_TEXTURES	5

typedef struct wallTextureIds{
	vector<int> tId;
};

class EnsancheModelBuildingAdv{
	
	public:
		EnsancheModelBuildingAdv();
		~EnsancheModelBuildingAdv();
		EnsancheModelBuildingAdv( const EnsancheModelBuildingAdv & mom );
		
		void setup();
		void clear();
		
		void draw( int drawMode = MODEL_DRAW_MODE_3D, bool bDrawWOffset = false );
		void draw2D(bool bDrawWOffset);
		void draw3D(bool bDrawWOffset);
		void draw3D2(bool bDrawWOffset);
		
		// setup the model from the original building (with nFloor info + offsets etc.) 
		void setupFromBuilding(EnsancheBuilding buildingOriginal );
	
		// set new floor (expansion, facade etc)
		void setFloor(EnsancheBuilding buildingFloor, int floorNum );
		
		void addBuildingFloor(EnsancheBuilding & building, bool bOffset = true );
		void generateModel( float wallHeight );
		
		int getNumPts(){ return nPts;}
		int getNumFloorPts(){ return nFloorPts; }
		
		void mouseDragged(ofMouseEventArgs& event);
		void mousePressed(ofMouseEventArgs& event);
		
		void enable(){  bEnabled = true; }
		void disable(){ bEnabled = false; }
		
		void setWallTexture(ofTexture * tex, int type = 0);
		//int getWallTextureId(int wallIndex, polySimple & buildingPoly);
		
		//---
		bool bEnabled;
		ofPoint offSet;
		ofPoint center;
		ofPoint rotation;
		float scale;
		int editMode;
		
		//--- 2d representation
		vector<polySimple> buildingFloors;
		
		//--- texture
		vector<wallTextureIds> wallTexIds;
		ofTexture * textureWall;
		bool bSetWallTexture;
		
		ofTexture * textureWalls[MODEL_T_TEXTURES];
		bool bSetWallTextures[MODEL_T_TEXTURES];
		int nFloors;

	protected:
		
		//-- for model with varying textures
		float * ptsv[MODEL_T_TEXTURES];
		float * texptsv[MODEL_T_TEXTURES];
		int  nptsv[MODEL_T_TEXTURES];
		int  nptsflv[MODEL_T_TEXTURES];
		int  ntexptsv[MODEL_T_TEXTURES];
		
		//--- for 3d rendering
		int nPts;
		int nFloorPts;
		int nTexPts;
		float * pts;
		float * floorPts;
		float * texPts;
		ofPoint lastMouse;

};