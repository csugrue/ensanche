/*
 *  ensancheModelBarrio.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/8/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */
#pragma once

#include "polyEditable.h"
#include "ensancheModelBuilding.h"
#include "ensancheNewFacadeModel.h"
#include "ensancheBarrio.h"

class EnsancheModelBarrio{
	
	public:
		EnsancheModelBarrio();
		~EnsancheModelBarrio();
		EnsancheModelBarrio( const EnsancheModelBarrio & mom);
		
		
		void setup();
		void clear();
		void draw();
		void drawWithCamera();
		void draw2D();
		
		void loadFromXml( string filename);
		
		//void addBuildings(EnsancheBarrio * barrio);
		//void addBuilding(EnsancheBuilding * building, float wallHeight, bool bOffset);
		
		void mouseDragged(ofMouseEventArgs& event);
		void mousePressed(ofMouseEventArgs& event);
	
		void enable(){  bEnabled = true; }
		void disable(){ bEnabled = false; }
	
		bool	bEnabled;
		int		editMode;
		ofPoint center;
		ofPoint rotation;
		ofRectangle boundingBox;
		float	scale;
		ofPoint pov;
		
		
		
		//--- vector of buildings
		//vector<EnsancheModelBuilding> buildings;
		vector<EnsancheNewFacadeModel> buildings;
	
		void setDrawMode( int dMode) { drawMode = dMode;}
		int drawMode;
		
		ofTexture textureWall;
		ofTexture textureWalls[MODEL_T_TEXTURES];
		
	protected:
	

		ofPoint lastMouse;
	
};
