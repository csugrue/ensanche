/*
 *  analysisExpander.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 5/11/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "analysisExpander.h"

AnalysisExpander::AnalysisExpander()
{
	font.loadFont("fonts/verdana.ttf", 8,true,true);
	//font.setLineHeight(8+2);
}


AnalysisExpander::~AnalysisExpander()
{
}

void AnalysisExpander::clear()
{
	buildings.clear();
	ends.clear();
	uidToIndex.clear();
}

float AnalysisExpander::explandToMinimum(EnsancheExpandData & expandBuilding, int sideToExpand, polySimple polyMin, polySimple polyMax, float minDist, float minForRipple)
{
	
	float diffDist = 0.f;
	
	// calculate distance between min rect and max rect
	ofxVec2f centerMin = ofxVec2f( .5*(polyMin.pts[sideToExpand].x+polyMin.pts[sideToExpand+1].x),.5*(polyMin.pts[sideToExpand].y+polyMin.pts[sideToExpand+1].y) );
	ofxVec2f centerMax = ofxVec2f( .5*(polyMax.pts[sideToExpand].x+polyMax.pts[sideToExpand+1].x),.5*(polyMax.pts[sideToExpand].y+polyMax.pts[sideToExpand+1].y) );
	
	float distCenters = (centerMax-centerMin).length();
	
	// if distance minus the minDist is too small ( < minRipple), expand to max and return 0
	// else expand to minDist and return difference distance
	if( (distCenters - minDist) < minForRipple )
	{
		expandWallToMinRect(expandBuilding, sideToExpand, polyMax);
	}
	else{
		
		// make new poly with the expanded side to the minDist
		polyInitExpand.clear();
		//polySimple poly;
		polyInitExpand = polyMin;
		
		vector<ofxVec2f> pps;
		for( int i = 0; i < polyInitExpand.pts.size(); i++)
		{
			int nxt = (i==polyInitExpand.pts.size()-1) ? 0: i+1;
			ofxVec2f polyPP = ofxVec2f( polyInitExpand.pts[i].x-polyInitExpand.pts[nxt].x,polyInitExpand.pts[i].y-polyInitExpand.pts[nxt].y);
			polyPP = polyPP.perpendicular();
			pps.push_back(polyPP);
			
		}
		
		for( int i = 0; i < polyInitExpand.pts.size(); i++)
		{
			int nxt = (i==polyInitExpand.pts.size()-1) ? 0: i+1;

			polyInitExpand.pts[i].x = polyInitExpand.pts[i].x + minDist * pps[i].x;
			polyInitExpand.pts[i].y = polyInitExpand.pts[i].y + minDist * pps[i].y;
			polyInitExpand.pts[nxt].x = polyInitExpand.pts[nxt].x + minDist * pps[i].x;
			polyInitExpand.pts[nxt].y = polyInitExpand.pts[nxt].y + minDist * pps[i].y;
			
		}
		
		/*int nxt = (sideToExpand==polyInitExpand.pts.size()-1) ? 0: sideToExpand+1;
		ofxVec2f polyPP = ofxVec2f( polyInitExpand.pts[sideToExpand].x-polyInitExpand.pts[nxt].x,polyInitExpand.pts[sideToExpand].y-polyInitExpand.pts[nxt].y);
		polyPP = polyPP.perpendicular();
		
		polyInitExpand.pts[sideToExpand].x = polyInitExpand.pts[sideToExpand].x + minDist * polyPP.x;
		polyInitExpand.pts[sideToExpand].y = polyInitExpand.pts[sideToExpand].y + minDist * polyPP.y;
		polyInitExpand.pts[nxt].x = polyInitExpand.pts[nxt].x + minDist * polyPP.x;
		polyInitExpand.pts[nxt].y = polyInitExpand.pts[nxt].y + minDist * polyPP.y;
		*/
		// expand
		expandWallToMinRect(expandBuilding, sideToExpand, polyInitExpand);
		
		// set remaining distance
		diffDist = distCenters - minDist;
	}
	
	return diffDist;
}

void AnalysisExpander::expandWallToMinRect(EnsancheExpandData & expandBuilding, int sideToExpand, polySimple poly)
{
	// tries to expand using the angle of the preceding and following sides of the buildings
	
	// find the side we want to expand. stops at first found because should be one each at this point
	int sideIndex = -1;
	for( int i = 0; i < expandBuilding.sideIds.size(); i++)
	{
		if( expandBuilding.sideIds[i] == sideToExpand )
		{
			sideIndex = i;
			break;
		}
		
	}
	
	if(sideIndex == -1 )
	{
		cout << "ERROR: side not found. " << endl;
		return;
	}
	
	// find the end pts of the side before
	int ptsB[2];
	ptsB[0] = (sideIndex==0) ? expandBuilding.sideIds.size()-1 : sideIndex-1;
	ptsB[1] = sideIndex;
	
	// find the end pts of the side after
	int ptsA[2];
	ptsA[0] = (sideIndex==expandBuilding.sideIds.size()-1) ? 1 : sideIndex+2;
	ptsA[1] = (sideIndex==expandBuilding.sideIds.size()-1) ? 0 : sideIndex+1;
	
	// find vectors of before and after
	ofxVec2f vecBefore, vecAfter;
	
	// NOTE: error check these points exist!!!!
	vecBefore.x = expandBuilding.building.buildingPoly.pts[ ptsB[0] ].x - expandBuilding.building.buildingPoly.pts[ ptsB[1] ].x;
	vecBefore.y = expandBuilding.building.buildingPoly.pts[ ptsB[0] ].y - expandBuilding.building.buildingPoly.pts[ ptsB[1] ].y;
	vecBefore = vecBefore.normalize();
	
	vecAfter.x = expandBuilding.building.buildingPoly.pts[ ptsA[1] ].x - expandBuilding.building.buildingPoly.pts[ ptsA[0] ].x;
	vecAfter.y = expandBuilding.building.buildingPoly.pts[ ptsA[1] ].y - expandBuilding.building.buildingPoly.pts[ ptsA[0] ].y;
	vecAfter = vecAfter.normalize();
	
	// find points that intersect with min rectangle
	ofPoint iPt;
	
	// get bounding box so we know that we create a line long enought to intersect
	ofRectangle boundingBox = poly.getBoundingBox();
	float len = 10 * MAX(boundingBox.width , boundingBox.height);
	
	ofPoint afterPt;
	afterPt.x = expandBuilding.building.buildingPoly.pts[ ptsA[1] ].x + len * vecAfter.x;
	afterPt.y = expandBuilding.building.buildingPoly.pts[ ptsA[1] ].y + len * vecAfter.y;
	
	if( intersectionTwoLines(expandBuilding.building.buildingPoly.pts[ ptsA[1] ], afterPt, poly.pts[sideToExpand], poly.pts[sideToExpand+1], &iPt) )
	{
	expandBuilding.building.buildingPoly.pts[ ptsA[1] ].set(iPt.x,iPt.y);
	}
	
	ofPoint befPt;
	befPt.x = expandBuilding.building.buildingPoly.pts[ ptsB[1] ].x - len * vecBefore.x;
	befPt.y = expandBuilding.building.buildingPoly.pts[ ptsB[1] ].y - len * vecBefore.y;
	
	if( intersectionTwoLines2(expandBuilding.building.buildingPoly.pts[ ptsB[1] ], befPt, poly.pts[sideToExpand], poly.pts[sideToExpand+1], &iPt) )
	{
		//cout << "FOUND INTERSECTION" << endl;
		expandBuilding.building.buildingPoly.pts[ ptsB[1] ].set(iPt.x,iPt.y);
	}
	
}

void AnalysisExpander::findEndPoints(EnsancheBuilding building, int sideToExpand, vector<int>sideIds,endPoints * nEnds)
{
	int endFirst = -1;
	int endLast  = -1;
	
	// find end points
	for( int i = 0; i < sideIds.size(); i++)
	{
		//cout << i << " sideid: " << sideIds[i] << " mySide " << sideToExpand << endl;
		if( sideIds[i] == sideToExpand && endFirst == -1 )		endFirst = i;
		if( sideIds[i] == sideToExpand ) endLast = i;
		if( endFirst != -1 && sideIds[i] != sideToExpand ) break;
	}
	
	
	bool bLoopOver = false;
	
	// check in case loops over
	if( endFirst == 0 )
	{
		for(int i = sideIds.size()-1; i > 0; i--)
		{
			if( sideIds[i] != sideToExpand ) break;
			else	endFirst = i;
			bLoopOver = true;
		}
	}
	
	if( endLast == sideIds.size()-1 )
	{
		for(int i = 0; i < sideIds.size(); i++)
		{
			if( sideIds[i] != sideToExpand ) break;
			else	endFirst = i;
			bLoopOver = true;
		}		
	}
	
	endLast+=1;
	//cout << "bLoopOver " << bLoopOver << " endfirst " << endFirst << " endLast " << endLast << endl;
	
	int eL= (endLast > building.buildingPoly.pts.size()-1) ?  0: endLast;
	
	nEnds->pts.push_back( building.buildingPoly.pts[endFirst] );
	nEnds->ids.push_back( endFirst );
	
	nEnds->pts.push_back( building.buildingPoly.pts[eL] );
	nEnds->ids.push_back( endLast );
	
	ofPoint endIds;
	endIds.x = endFirst;
	endIds.y = eL;
	nEnds->sideToEndPt.insert ( pair<int,ofPoint>(sideToExpand,endIds) );
}

void AnalysisExpander::findAllEndPoints(EnsancheBuilding building, vector<int>sideIds,endPoints * nEnds)
{
	nEnds->pts.clear();
	nEnds->ids.clear();
	nEnds->sideToEndPt.clear();
	
	for( int i = 0; i < 4; i++)
	{
		findEndPoints(building, i, sideIds, nEnds);
	}
}

void AnalysisExpander::removeInBetweenPoints(EnsancheBuilding & building, int sideToExpand, vector<int> * sideIds, endPoints * nEnds)
{
	// removes any points that lie between the "ends" of a side so that only a singleline remains that can be replaced with the new facade
	
	// get the end points
	ofPoint eIDs	= nEnds->sideToEndPt.find(sideToExpand)->second;
	
	if( nEnds->sideToEndPt.find(sideToExpand) == nEnds->sideToEndPt.end() )
	{
	cout << "NO ENDS!" << endl;
	return;
	}
	
	int startPt		= (int)(eIDs.x);
	int endPt		= (int)(eIDs.y);
	
	cout << "side: " << sideToExpand << "startPt: " << startPt << " endPt: " << endPt << endl;
	
	int tBetween = endPt-startPt;
	
	// this is for the loop over end problem
	if( startPt > endPt)
	{
		int tPts = building.buildingPoly.pts.size();
		tBetween = (tPts-startPt) + endPt;
	}
	
	// test if there are points in between
	if( tBetween == 1 ){
	 return;
	}
	
	// record which points will be removed
	vector<bool> bRemoved;
	for( int i = 0; i < building.buildingPoly.pts.size(); i++) 
		bRemoved.push_back(false);
	
	// loop from start point to all points
	
	
	int endTotal = startPt + tBetween;
	
	// create temp building and copy over data
	EnsancheBuilding tempBuilding;
	tempBuilding = building;
	building.clear();
	
	// copy over the sideIds
	vector<int> tempSides;
	tempSides.assign(sideIds->begin(),sideIds->end());
	sideIds->clear();
	
	// loop thru and label "in between" points to be removed
	for( int i = startPt+1; i < endTotal; i++)
	{
		
		int myPt = i % tempBuilding.buildingPoly.pts.size();
		bRemoved[myPt] = true;
	}
	
	// loop thru and add points not marked for removal
	for( int i = 0; i < tempBuilding.buildingPoly.pts.size(); i++)
	{
		if( !bRemoved[i])
			building.buildingPoly.pushVertex(tempBuilding.buildingPoly.pts[i]);
	}
		
	// repeat for side ids
	for( int i = 0; i < tempSides.size(); i++)
	{
		if( !bRemoved[i])
				sideIds->push_back(tempSides[i]);
	}
	
	for( int i = 0; i < tempBuilding.walls.size(); i++)
	{
		if( !bRemoved[i+1] )
		{
			building.walls.push_back(tempBuilding.walls[i]);
		}
	}
	
	// NOTE: this is as of yet untested... might not be saving data correctly
	for( int i = 0; i < building.walls.size(); i++)
	{
		building.walls[i].pts[0] = building.buildingPoly.pts[i];
		building.walls[i].pts[1] = building.buildingPoly.pts[i+1];
		building.walls[i].posInPoly = i;
	}
	
}

void AnalysisExpander::draw(float scale)
{
	for( int i = 0; i < ends.size(); i++)
	{
		//buildings[i].draw();
		for( int j = 0; j < ends[i].pts.size(); j++)
		{
			glPushMatrix();
				glTranslatef(ends[i].pts[j].x,ends[i].pts[j].y,0);
				glScalef(1/scale,1/scale,1);
				font.drawString( ofToString(ends[i].ids[j]),0,0  );
			glPopMatrix();
		}
		
	}
}

