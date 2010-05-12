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
}

void AnalysisExpander:: expand( 
								EnsancheBuilding building, 
								EnsancheBuilding & dstBuilding, 
								int sideToExpand,
								vector<int>sideIds,
								enExpandData epData
								)
{

		if(sideIds.size() < building.buildingPoly.pts.size()-1 ) return;
		
		// copy original to destination
		dstBuilding.clear();
		dstBuilding = building;
		
		// set winding
		//dstBuilding.buildingPoly.setWinding(WIND_CLOCKWISE);
		
		int endFirst = -1;
		int endLast  = -1;
		
		// find end points
		for( int i = 0; i < sideIds.size(); i++)
		{
			cout << i << " sideid: " << sideIds[i] << " mySide " << sideToExpand << endl;
			if( sideIds[i] == sideToExpand && endFirst == -1 )		endFirst = i;
			if( sideIds[i] == sideToExpand ) endLast = i;
		}
		
		
		//if(endLast < sideIds.size()-1 ) endLast+=1;
		//else endLast = 0;
		
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
		cout << "bLoopOver " << bLoopOver << " endfirst " << endFirst << " endLast " << endLast << endl;
		
		if(!bLoopOver)
		{
			// remove points in between
			
		}
		
		if( uidToIndex.find( building.uid ) == uidToIndex.end() )
		{
			endPoints ep;
			ends.push_back(ep);
			buildings.push_back(dstBuilding);
			uidToIndex.insert( pair<string,int>( building.uid,(int)(ends.size()-1) ) );
		}
		
		int epId = uidToIndex.find( building.uid )->second;
		ends[epId].pts.push_back( building.buildingPoly.pts[endFirst] );
		ends[epId].ids.push_back( endFirst );
		
		int eL= (endLast > building.buildingPoly.pts.size()-1) ?  0: endLast;
		ends[epId].pts.push_back( building.buildingPoly.pts[eL] );
		ends[epId].ids.push_back( endLast );
		//buildings.push_back(dstBuilding);
	//if ( epData[i].sisMeSides[j].sister.find(k) == epData[i].sisMeSides[j].sister.end( ) ){

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

