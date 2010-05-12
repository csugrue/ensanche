/*
 *  analysisExpandAreaFinder.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

/*
Finds the possible expand area for the buildings.
*/

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ensancheBarrio.h"
#include "ensancheUtils.h"
#include "ensancheConstants.h"

typedef struct enShortestDistances{
	int mySide;
	int sisterSide;
	int sisterIndex;
	string sisterUid;
	ofPoint sPt;
	ofPoint ePt;
	bool bSideStopper;
};


typedef struct enIntersectWSister{
	map<int,int> sister;
};

typedef struct enExpandData{
	
	CvBox2D box;					// rectangle that encloses min area of poly/building
	vector<ofxVec2f> pps;			// perpendiculr vectors for each side of box
	CvPoint2D32f pt[4];				// points of enclosing rectangle
	polySimple poly;				// points of enclosing rectangle as poly object
	vector<ofPoint>sPts;			// start points on box of outward expand vectors
	vector<ofPoint>ePts;			// end points of intersections with other buildings
	string uid;						// uid associated with the corresponding building
	int nFloors;					// number flors from original building (for faster calculation of max dists)
	
	vector<enShortestDistances> iSides;		// shortest distances between each building, and each side
	enIntersectWSister sisMeSides[4];		// map that matches if side of mine should check against side of sister. used so we don't check sides that dont make sense
	
	
};

typedef struct enExpandAreas{
	
	bool bSideMaxed[4];				// test if side cannot move any farther bc either at max dist or too close to neighbor
	polySimple poly;				// polygon defining total possible expand area
	string uid;
	// float expandDist[4]; distance each expands??
};


class AnalysisExpandAreaFinder{
	
	public:
	
		AnalysisExpandAreaFinder();
		~AnalysisExpandAreaFinder();

		void setup( EnsancheBarrio barrioOriginal );
		void clear();
		void draw();
		
		// seperate draws
		void drawIntersections( int me = -1 );
		void drawShortest( int me = -1 );
		void drawExpandAreas( int me = -1 );
		
		// expand the area in all directions possible. step = how far to move each interval while testing against neighbors
		void expand( float step );
		
		
		vector<enExpandData> expanders;
	
	protected:
		
		// find the minium enclosing rectangle of each building, for faster calculation of the expansion
		void findMinRect(EnsancheBarrio	barrioOriginal);
	
		// find perpendicular vector of each enclosing rectangle to determine which sisters to check against
		void findPPs( vector<enExpandData> & epData );
	
		// find shortest intersection points to other buildings to test if expansion is too close to neighbor
		void findIntersectingLines(vector<enExpandData> & epData);
	
		// find shortest dist between each building
		void findShortestDistances(vector<enExpandData> & epData);
		
		// stop expansion when we are too close
		void limitExpansion(vector<enExpandData> & epData);
		
		// minimum circumbscribed rectangle for each building, pts to expand etc.
		vector<enExpandData> expandersOriginal;
		vector<EnsancheBuilding> buildings; // save a copy of the buildings for later analysis
		
		bool bSetup;
		
		// expanding params
		vector<enExpandAreas> expandersAreas;
				
	
};
