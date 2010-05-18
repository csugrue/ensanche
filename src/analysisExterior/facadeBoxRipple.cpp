/*
 *  facadeBoxRipple.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 4/24/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "facadeBoxRipple.h"

FacadeBoxRipple::FacadeBoxRipple()
{
	
	minLong	= 2.f; // meters 
	maxLong  = 4.f;
	minExtrude	 = 1.f;		// should be set based on expand areas...
	maxExtrude	 = 3.f;
	/*
	minShort; 
	maxShort;
	
	bEvenNumPts;
	*/
}

FacadeBoxRipple::~FacadeBoxRipple()
{
}

vector<ofPoint> FacadeBoxRipple::getFacadeLine(ofPoint ptA, ofPoint ptB, ofPoint pp)
{
	newPoints.clear();
	newExPoints.clear();
	
	vPtA	= ofxVec2f( ptA.x, ptA.y );
	vPtB	= ofxVec2f( ptB.x, ptB.y );
	ofxVec2f vPP	= (vPtB-vPtA).normalize();//ofxVec2f( pp.x, pp.y );
	
	// generate a number of points based on length of line
	float length = sqrt( (ptA.x-ptB.x)*(ptA.x-ptB.x)+(ptA.y-ptB.y)*(ptA.y-ptB.y) );//.length();
	
	int maxPts = length / minLong;
	int minPts = length / maxLong;
	float tLength = 0;
	int nPts = 0;
	
	// create a random number of points between what is possible
	int totalPts = ofRandom(minPts, maxPts);
	
	// generate random lengths for each between min/max length possible
	vector<float> rLens;
	for( int i = 0; i < totalPts; i++)
	{
			rLens.push_back( ofRandom(minLong, maxLong));
			tLength += rLens[i];
	}
	
	
	
	// adjust the lngths so that it is more proportional, last one is not too short or too long
	float lengthLast = length-tLength;
	
	
	// check if we are not long enough
	cout << "length : " << length << " tlength: " << tLength << " difflen: " << lengthLast << endl;
	
	if( lengthLast < minLong )
	{
		while( (length-tLength) < minLong)
		{
			tLength = 0;
			for( int i = 0; i < totalPts; i++)
			{
				rLens[i] *= .999;
				if(rLens[i] < minLong) rLens[i] = minLong;
				tLength += rLens[i];
			}
			
		}
	
	}else if( lengthLast > maxLong )
	{
		while( (length-tLength) > maxLong)
		{
			tLength = 0;
			for( int i = 0; i < totalPts; i++)
			{
				rLens[i]*=1.0001;
				if(rLens[i] > maxLong) rLens[i] = maxLong;
				tLength += rLens[i];
			}
			
		}
		
	}
	
	
	// insert the first
	newPoints.push_back( ofPoint(ptA.x,ptA.y) );
	
	// create points at the new length
	tLength = 0;
	for( int i = 0; i < totalPts; i++)
	{
		tLength += rLens[i];
		newPoints.push_back( ofPoint(ptA.x + tLength*vPP.x, ptA.y + tLength*vPP.y) );		
	}
	
	// insert the last
	newPoints.push_back( ofPoint(ptB.x,ptB.y) );

	// ??!! save the extrusions, and use to create the next point, or do all the points here??
	
	// create extrusions
	for( int i = 0; i < newPoints.size()-1; i++)
	{
		// get a random extrude
		float extrude = ofRandom(minExtrude,maxExtrude);
		
		// 
		newExPoints.push_back( ofPoint( newPoints[i].x  + extrude * pp.x, newPoints[i].y + extrude * pp.y) );
		newExPoints.push_back( ofPoint( newPoints[i+1].x  + extrude * pp.x, newPoints[i+1].y + extrude * pp.y) );

	}
	
	
	// create line
	newLinePoints.clear();
	
	// insert first
	newLinePoints.push_back(newExPoints[0]);
	
	int t = 1;
	
	for( int i = 1; i < newExPoints.size(); i++)
	{
		newLinePoints.push_back( newExPoints[i] );
		//ofPoint(newExPoints[i].x,newExPoints[i-1].y) );
		//newLinePoints.push_back(newExPoints[i]);
	}
	
	//insert last
	//int lst = newExPoints.size()-1;
	//newLinePoints.push_back(ofPoint(newExPoints[lst].x,newExPoints[lst-1].y) );
	
	return newLinePoints;
	
}

void FacadeBoxRipple::draw()
{
	ofNoFill();
	for( int i = 0; i < newPoints.size(); i++)
	{
		ofCircle(newPoints[i].x, newPoints[i].y, .2);
	}
	
	ofFill();
	for( int i = 0; i < newExPoints.size(); i++)
	{
		ofCircle(newExPoints[i].x,newExPoints[i].y,.2);
		
	}
	
	ofNoFill();
	ofBeginShape();
		for( int i = 0; i < newLinePoints.size(); i++)
			ofVertex(newLinePoints[i].x, newLinePoints[i].y);
	ofEndShape(false);
	
	ofFill();
	ofSetColor(0xff0000);
	ofCircle(vPtA.x, vPtA.y, .4);
	ofCircle(vPtB.x, vPtB.y, .4);
	
}



