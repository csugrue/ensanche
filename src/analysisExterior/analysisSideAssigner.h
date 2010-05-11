/*
 *  analysisSideAssigner.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ensancheBarrio.h"
#include "ensancheUtils.h"
#include "ensancheConstants.h"
#include "analysisExpandAreaFinder.h"

/*
assigns each side or "wall"  to a side of the building (minimum enclosing rect)
uses a system of dividing the enclosing rectangle into  quarter quadrants and first assigning based on lines position in quadrant
*/

class AnalysisSideAssigner{
	
	public:
	
		AnalysisSideAssigner();
		~AnalysisSideAssigner();
		
		// assigns sides by direction of perpendicular, and sorts (unfinished assignment);
		void assignSides(EnsancheBuilding * building);
		
		// assigns sides by location in quadrants first and then differences in perpendiculars
		void assignSidesQuadrants(EnsancheBuilding * building, enExpandData expanders);
		
		void draw();
		
		polySimple poly;
		vector<int> sideIds;
		vector<ofPoint> ppPts;
		polySimple polyHalfQuads[4];
		polySimple polyQuarterQuads[4];
		ofPoint centerPoints[4];
	
	protected:
		
		// lops through poly and checks for non-assigned sides that are sandwiched between two of the same sides (assumes this is part of that side)
		void checkBeforeAfter();
		
		int getBefore(int i);
		int getAfter(int i);
		
		// check for before and after, calculate length of side in each quadrant, choose greater
		void checkLengthInQuadrant();
		
		// check bef/after and take closer angle
		void checkAngleDifference(enExpandData expanders);
		
};
