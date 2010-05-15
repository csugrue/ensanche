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

void AnalysisExpander::expand( EnsancheBuilding & buildingExpanded, int sideToExpand, enExpandAreas expandAreas, enExpandData epData, endPoints * nEnds )
{
	// assumes the endpoint bulding is copied into expanded already
	
	/*ofPoint ends = ;
	
	ofPoint endPtA = ;
	ofPoint endPtB = ;
	
	// calculate the position of each endpoint to the min rect line
	ofPoint newEndPtA = distanceToSegment(ofPoint p1, ofPoint p2, ofPoint p3);
	ofPoint newEndPtB;
	
	*/

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
		findEndPoints(building, sideToExpand, sideIds, epData,nEnds);
	
		// remove the in between points
		
		// expand outwards
}

void AnalysisExpander::findEndPoints(EnsancheBuilding building, int sideToExpand, vector<int>sideIds,enExpandData epData,endPoints * nEnds)
{
	int endFirst = -1;
	int endLast  = -1;
	
	// find end points
	for( int i = 0; i < sideIds.size(); i++)
	{
		cout << i << " sideid: " << sideIds[i] << " mySide " << sideToExpand << endl;
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

void AnalysisExpander::findAllEndPoints(EnsancheBuilding building, vector<int>sideIds,enExpandData epData,endPoints * nEnds)
{
	nEnds->pts.clear();
	nEnds->ids.clear();
	nEnds->sideToEndPt.clear();
	
	for( int i = 0; i < 4; i++)
	{
		findEndPoints(building, i, sideIds, epData, nEnds);
	}
}

void AnalysisExpander::removeInBetweenPoints(EnsancheBuilding & building, int sideToExpand, vector<int> * sideIds, endPoints * nEnds)
{
	ofPoint eIDs	= nEnds->sideToEndPt.find(sideToExpand)->second;
	int startPt		= (int)(eIDs.x);
	int endPt		= (int)(eIDs.y);
	
	if( startPt > endPt )
	{
		cout << "loop over" << endl;
	}else if( (endPt - startPt) == 1 ){
		cout << "nothing to remove" << endl;
	}else{
		
		polySimple tempBuilding;
		tempBuilding.pts.assign(building.buildingPoly.pts.begin(),building.buildingPoly.pts.end());
		building.buildingPoly.clear();
		
		for( int i = startPt+1; i < endPt; i++)
		{
			cout << "remove pt: " << i << endl;			
		}
		
		for( int i = 0; i < tempBuilding.pts.size(); i++)
		{
			if( i > startPt && i < endPt)
			;
			else{
				cout << "addpoint " << i << endl;
				building.buildingPoly.pushVertex(tempBuilding.pts[i]);
			}
		}
		
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

