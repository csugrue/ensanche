/*
 *  ensancheBuilding.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/4/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ensancheFloorPlan.h"
#include "ensancheMapBuilding.h"
#include "uidGenerator.h"

class EnsancheBuilding{

	public:
		EnsancheBuilding();
		~EnsancheBuilding();
		EnsancheBuilding( const EnsancheBuilding & mom );
		
		void clear();
		void setup();
		void draw();
	
		void addWall( ofPoint m1, ofPoint m2, int uid );
		void addClosingWall();
		
		//--- set from xml
		//void loadFromXml( string filename);
	
		//--- set from editor functions
		void addWalls(EnsancheMapBuilding & mapBuilding, float pixelsPerMeter, bool bRemoveBounding);
	
		//--- architectural elements
		vector<enWall>		walls;
		polySimple			buildingPoly;
		
		// hmm makes sense if dont have polys for all floors?
		int nFloors;
		
		string uid;
		
	protected:
		
		ofPoint	getTransformsApplied(ofPoint me, ofPoint pre, ofPoint pst, float rotation);
};