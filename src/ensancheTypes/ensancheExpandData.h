/*
 *  ensancheExpandData.h
 *  ensanche_2.0
 *
 *  Created by Chris on 5/12/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ensancheBuilding.h"
#include "polySimple.h"

typedef struct endPoints{
	vector<int> ids;
	vector<ofPoint> pts;
	map<int,ofPoint> sideToEndPt;
};

class EnsancheExpandData
{
	public:
		EnsancheExpandData();
		~EnsancheExpandData();
		EnsancheExpandData( const EnsancheExpandData & mom );
		
		void setup(EnsancheBuilding & origBuilding);
		void clear();
		void clearEnds();
		
		void drawEnds(float scale, ofTrueTypeFont * momfont);
		void drawSideIds();
		
		// data for side Ids
		vector<int> sideIds;
		
		// data for end points of sides
		endPoints ends;
		
		// original poly
		EnsancheBuilding momBuilding;
		
		// new poly with changes
		EnsancheBuilding building;
	
		// new poly with only end points
		//EnsancheBuilding buildingEndsOnly;
		
		// new poly with minimum expansions
		//EnsancheBuilding buildingExpanded;
		
	protected:
	
	
	

};
