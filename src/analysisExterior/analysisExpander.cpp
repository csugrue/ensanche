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


void AnalysisExpander::expand( 
								EnsancheBuilding building, 
								EnsancheBuilding & dstBuilding, 
								int sideToExpand,
								vector<int>sideIds,
								enExpandData epData,
								endPoints * nEnds)
{

		if(sideIds.size() < building.buildingPoly.pts.size()-1 ) return;
		
		// find end points for this side			
		findEndPoints(building, sideToExpand, sideIds,nEnds);
	
		// remove the in between points
		
		// expand outwards
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
	
	// find endpoints again
	//findAllEndPoints(building, sideIds, nEnds);
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

