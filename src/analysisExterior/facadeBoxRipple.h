/*
 *  facadeBoxRipple.h
 *  ensanche_2.0
 *
 *  Created by Chris on 4/24/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVec2f.h"

class FacadeBoxRipple{

	public:

		FacadeBoxRipple();
		~FacadeBoxRipple();
		
		vector<ofPoint> getFacadeLine(ofPoint ptA, ofPoint ptB, ofPoint pp);
		
		
		float	minLong, maxLong;
		float	minShort, maxShort;
		float	minExtrude, maxExtrude;
		bool	bEvenNumPts;
		//? float variability ?? 
		
		// temp?? for debug
		vector<ofPoint> newPoints;
		vector<ofPoint> newExPoints;
		vector<ofPoint> newLinePoints;
		
		ofxVec2f vPtA,vPtB;
		void draw();
		
};