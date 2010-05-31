/*
 *  analysisSideExpandMethod.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */
#include "analysisSideAssigner.h"


AnalysisSideAssigner::AnalysisSideAssigner()
{
}

AnalysisSideAssigner::~AnalysisSideAssigner()
{
}

void AnalysisSideAssigner::clear()
{
	poly.clear();
	sideIds.clear();
	ppPts.clear();
	for( int i = 0; i < 4; i++) polyHalfQuads[i].clear();
	for( int i = 0; i < 4; i++) polyQuarterQuads[i].clear();
	//polySimple polyHalfQuads[4];
	//polySimple polyQuarterQuads[4];
	//ofPoint centerPoints[4];
}


void AnalysisSideAssigner::assignSidesQuadrants(EnsancheBuilding * building, enExpandData expanders, vector<int> * nSideIds)
{
	assignSidesQuadrants(building, expanders);
	nSideIds->clear();
	nSideIds->assign(sideIds.begin(),sideIds.end());
}

void AnalysisSideAssigner::assignSidesQuadrants(EnsancheBuilding * building, enExpandData expanders)
{
	// make sure we have proper min rect passed in
	if( expanders.poly.pts.size() < 4 ) return;
	
	// clear data
	poly.clear();
	sideIds.clear();
	ppPts.clear();
	
	// copy building points
	poly = building->buildingPoly;
	//poly.setWinding(WIND_CLOCKWISE);
	
	for( int i = 0; i < 4; i++) polyHalfQuads[i].clear();
	for( int i = 0; i < 4; i++) polyQuarterQuads[i].clear();
	for( int i = 0; i < 4; i++) polyQuarterQuads[i].bClosed = true;
	for( int i = 0; i < 4; i++) polyHalfQuads[i].bClosed = true;

	//--- expand the min rect slightly so we get larger quadrants
	float step = 1;
	for( int i = 0; i < expanders.poly.pts.size(); i++)
	{
		expanders.poly.pts[i].x += step * expanders.pps[i].x;
		expanders.poly.pts[i].y += step * expanders.pps[i].y;
	}
	
	//--- find center points of each side
	for( int i = 0; i < 4; i++)
	{
		int nxt = (i==3) ? 0 : i+1;
		centerPoints[i] = ofPoint( 
				.5*(expanders.poly.pts[i].x+expanders.poly.pts[nxt].x), 
				.5*(expanders.poly.pts[i].y+expanders.poly.pts[nxt].y) );
	}
	
	
	
	//--- create polygons that are the division of the min enclosing rect of the building. divide in halves
	// perhaps better way to do it than by hand?
	polyHalfQuads[0].pushVertex(expanders.poly.pts[0]);
	polyHalfQuads[0].pushVertex(expanders.poly.pts[1]);
	polyHalfQuads[0].pushVertex(centerPoints[1]);
	polyHalfQuads[0].pushVertex(centerPoints[3]);
	
	polyHalfQuads[1].pushVertex(centerPoints[0]);
	polyHalfQuads[1].pushVertex(expanders.poly.pts[1]);
	polyHalfQuads[1].pushVertex(expanders.poly.pts[2]);
	polyHalfQuads[1].pushVertex(centerPoints[2]);
	
	polyHalfQuads[2].pushVertex(centerPoints[3]);
	polyHalfQuads[2].pushVertex(centerPoints[1]);
	polyHalfQuads[2].pushVertex(expanders.poly.pts[2]);
	polyHalfQuads[2].pushVertex(expanders.poly.pts[3]);
	
	polyHalfQuads[3].pushVertex(expanders.poly.pts[0]);
	polyHalfQuads[3].pushVertex(centerPoints[0]);
	polyHalfQuads[3].pushVertex(centerPoints[2]);
	polyHalfQuads[3].pushVertex(expanders.poly.pts[3]);
	
	//--- create quarter polygon divisions for better precision
	polyQuarterQuads[0].pushVertex(polyHalfQuads[0].pts[0]);
	polyQuarterQuads[0].pushVertex(polyHalfQuads[0].pts[1]);
	polyQuarterQuads[0].pushVertex( ofPoint(.5*(polyHalfQuads[0].pts[1].x+polyHalfQuads[0].pts[2].x), .5*(polyHalfQuads[0].pts[1].y+polyHalfQuads[0].pts[2].y) ) );
	polyQuarterQuads[0].pushVertex( ofPoint(.5*(polyHalfQuads[0].pts[0].x+polyHalfQuads[0].pts[3].x), .5*(polyHalfQuads[0].pts[0].y+polyHalfQuads[0].pts[3].y) ) );
	
	polyQuarterQuads[1].pushVertex( ofPoint(.5*(polyHalfQuads[1].pts[0].x+polyHalfQuads[1].pts[1].x), .5*(polyHalfQuads[1].pts[0].y+polyHalfQuads[1].pts[1].y) ) );
	polyQuarterQuads[1].pushVertex( polyHalfQuads[1].pts[1] );
	polyQuarterQuads[1].pushVertex( polyHalfQuads[1].pts[2] );
	polyQuarterQuads[1].pushVertex( ofPoint(.5*(polyHalfQuads[1].pts[2].x+polyHalfQuads[1].pts[3].x), .5*(polyHalfQuads[1].pts[2].y+polyHalfQuads[1].pts[3].y) ) );

	polyQuarterQuads[2].pushVertex( ofPoint(.5*(polyHalfQuads[2].pts[0].x+polyHalfQuads[2].pts[3].x), .5*(polyHalfQuads[2].pts[0].y+polyHalfQuads[2].pts[3].y) ) );
	polyQuarterQuads[2].pushVertex( ofPoint(.5*(polyHalfQuads[2].pts[1].x+polyHalfQuads[2].pts[2].x), .5*(polyHalfQuads[2].pts[1].y+polyHalfQuads[2].pts[2].y) ) );
	polyQuarterQuads[2].pushVertex( polyHalfQuads[2].pts[2] );
	polyQuarterQuads[2].pushVertex( polyHalfQuads[2].pts[3] );

	polyQuarterQuads[3].pushVertex( polyHalfQuads[3].pts[0] );
	polyQuarterQuads[3].pushVertex( ofPoint(.5*(polyHalfQuads[3].pts[0].x+polyHalfQuads[3].pts[1].x), .5*(polyHalfQuads[3].pts[0].y+polyHalfQuads[3].pts[1].y) ) );
	polyQuarterQuads[3].pushVertex( ofPoint(.5*(polyHalfQuads[3].pts[2].x+polyHalfQuads[3].pts[3].x), .5*(polyHalfQuads[3].pts[2].y+polyHalfQuads[3].pts[3].y) ) );
	polyQuarterQuads[3].pushVertex( polyHalfQuads[3].pts[3] );
	
	//--- loop through all points of the polygon and assign a side value
	// if it is in only one quarter quadrant, assign to that direction
	// if in none, assign -1
	// if in two assign -2
	for( int i = 0; i < poly.pts.size(); i++)
		sideIds.push_back(-1);
		
	for( int i = 0; i < poly.pts.size(); i++)
	{
		int nxt = ( i==poly.pts.size()-1 ) ? 0 : i+1;
		ofPoint center = ofPoint( .5*(poly.pts[i].x+poly.pts[nxt].x), .5*(poly.pts[i].y+poly.pts[nxt].y) );
		
		for( int j = 0; j < 4; j++)
		{
			if( pointInPolygon(center.x, center.y, polyQuarterQuads[j].pts) )
			{
				if(sideIds[i] == -1 ) sideIds[i] = j;
				else sideIds[i] = -2;
			} 
		}
		
	}
	
	//--- go thru and do before/after test assigning sandwhiched pieces to side that "encloses" them
	checkBeforeAfter();
	 
	// NOTE: not sure on order of next two functions. Perhaps better to use combo of both and choose one over other depending on stronger difference...
	
	//--- go thru all unassigned that are left and assign based on the difference in angle of its perpendicular to the perpendiscular of the enclosing sides
	checkAngleDifference(expanders);
	
	//--- go thru all unassigned and check if it has a longer segment in one quadrant than the other
	checkLengthInQuadrant();
	
	
	
	
	//--- TO DO: check that no more than one continuos line each and none left unassigned
		// (try to figure out which is best and reassign...?))
}

void AnalysisSideAssigner::assignSides(EnsancheBuilding * building)
{
	poly.clear();
	sideIds.clear();
	ppPts.clear();
	
	// copy building points
	poly = building->buildingPoly;
	if( poly.pts.size() > 1 )
	{
		int lst = poly.pts.size()-1;
		if( poly.pts[0].x == poly.pts[lst].x && poly.pts[0].y == poly.pts[lst].y)
		;
		else
			poly.pushVertex(poly.pts[0]);
	}
	
	poly.setWinding(WIND_CLOCKWISE);
	
	// for each point, choose a side id for it
	// 0 = n, 1 = ne, 2 = e, 3 = se, 4 = s, 5 = sw, 6 = w, 7 = nw
	// n(67.5-112.5) ne(22.5-67.5) e(0-22.5)(337.5-360) se(292.5-337.5) s(247.5-292.5) sw(202.5-247.5) w(157.5-202.5) nw(112.5-157.5)

	for( int i = 0; i < poly.pts.size()-1; i++)
	{
		// find a point from the pp
		ofxVec2f diffVc = ofxVec2f(poly.pts[i].x,poly.pts[i].y) - ofxVec2f(poly.pts[i+1].x,poly.pts[i+1].y);
		ofxVec2f pp		= diffVc.perpendicular();
		ofPoint cPt		= ofPoint(.5*(poly.pts[i].x+poly.pts[i+1].x),.5*(poly.pts[i].y+poly.pts[i+1].y) );
		ofPoint ppPt	= ofPoint(cPt.x-pp.x,cPt.y-pp.y);
		
		// get angle
		float angleA = RAD_TO_DEG*atan2(ppPt.y-cPt.y,ppPt.x-cPt.x);
		if( angleA < 0 ) angleA += 360.f;
		
		if( angleA >= 0 && angleA < 45 )		sideIds.push_back(2);	//e
		else if(angleA >= 45 && angleA < 135)	sideIds.push_back(4);	//s
		else if(angleA >= 135 && angleA < 225)	sideIds.push_back(6);	//w
		else if(angleA >= 225 && angleA < 315)	sideIds.push_back(0);	//n ? because reverse coord system?
		else if(angleA >= 315 && angleA <= 360)	sideIds.push_back(2);	//e
		else sideIds.push_back(-1);
		
		ppPts.push_back(ppPt);
	}
	
	
	//--- loop thru ids finding longest for each side
	map<int,int> longestSide;
	map<int,float> longestLength;
	
	//---
	for( int i = 0; i < sideIds.size(); i++)
	{
		ofxVec2f diffVc = ofxVec2f(poly.pts[i].x,poly.pts[i].y) - ofxVec2f(poly.pts[i+1].x,poly.pts[i+1].y);
		float length = diffVc.length();
		int side = sideIds[i];
		if ( longestSide.find(side) == longestSide.end() )
		{
			longestSide.insert( pair<int, int>(side, i) );
			longestLength.insert( pair<int, float>(side, length) );
		}
		
		if( length > longestLength[side] )
		{
			longestSide[side] = i;
			longestLength[side] = length;
		}
		
	}
	
	 
	//--- for those that are not consecutively attached to longest, mark as unknown 
	for( int i = 0; i < longestSide.size(); i++)
	{
		int side = longestSide[i];
		bool bBroken = false;
		for( int j = side+1; j < (side+poly.pts.size()-1); j++)
		{
			int nSide = j % poly.pts.size();
			if( sideIds[nSide] != i) bBroken = true;
			if(sideIds[nSide] == i && bBroken)
			{	
				//cout << "nSide " << sideIds[nSide]  << " side " << side << endl;
				sideIds[nSide] = -1;
			}
		}
	
	}
	
	    
}

void AnalysisSideAssigner::checkBeforeAfter()
{
	for( int i = 0; i < poly.pts.size(); i++)
	{
		if(sideIds[i] < 0 )
		{
			int before = getBefore(i);
			int after = getAfter(i);
									
			// if equal but not neg, set
			if( before >= 0 && before == after )
			{
				//cout << "CHANGE BEFORE AFTER" << endl;
				sideIds[i] = before;
			}

		}
		
	}
}

int AnalysisSideAssigner::getBefore(int i)
{
	int before = -1;
	
	int total = i-poly.pts.size();
	
	// find before
	for( int j = (i-1); j > total; j--)
	{
		int id = (j>=0) ? j % (int)poly.pts.size() : j+poly.pts.size();
		if( sideIds[id] >= 0 )
		{
			before = sideIds[id];
			break;
		}
	}
	
	return before;
}

int AnalysisSideAssigner::getAfter(int i)
{
	int after = -1;
	int total = (i+poly.pts.size())-1;
	for( int j = (i+1); j < total; j++)
	{
		int id = (j>=0) ? j % (int)poly.pts.size() : j+poly.pts.size();
		if( sideIds[id]  >= 0 )
		{
			after = sideIds[id];
			break;
		}
	}
}

void AnalysisSideAssigner::draw()
{
	if(poly.pts.size() <= 0 ) return;
	
	ofEnableAlphaBlending();
	
	for( int i = 0; i < ppPts.size(); i++)
	{
		ofCircle(ppPts[i].x,ppPts[i].y,.2);
	}
	
	ofNoFill();
	for( int i = 0; i < poly.pts.size()-1; i++)
	{
		if(sideIds.size() > i )
		{
		
			switch(sideIds[i])
			{
				case 0: ofSetColor(255,0,0); break;
				case 1: ofSetColor(0,255,0); break;
				case 2: ofSetColor(0,0,255); break;
				case 3: ofSetColor(255,0,255); break;
				case -2: ofSetColor(0,255,255); break;
				default: ofSetColor(0,0,0); break;
					
			}
			
			/*switch(sideIds[i])
			{
				case 0: ofSetColor(255,0,0); break;
				case 2: ofSetColor(0,255,0); break;
				case 4: ofSetColor(0,0,255); break;
				case 6: ofSetColor(255,0,255); break;
				default: ofSetColor(0,0,0); break;
			
			}*/
		}
		
		ofLine(poly.pts[i].x,poly.pts[i].y,poly.pts[i+1].x,poly.pts[i+1].y);
	}
	
	ofSetColor(0,0,0,200);
	for( int i = 0; i < 4; i++)
	{
		//ofCircle(centerPoints[i].x,centerPoints[i].y,.2);
		//centerPoints[i];
		polyHalfQuads[i].draw();
		polyQuarterQuads[i].draw();
	}
	
}

void AnalysisSideAssigner::checkLengthInQuadrant()
{
	// for all negatives
	for( int i = 0; i < poly.pts.size(); i++)
	{
		if(sideIds[i] < 0 )
		{
			int before = getBefore(i);
			int after = getAfter(i);
			
			ofPoint iPtBefore;
			ofPoint iPtAfter;
			float lenB = 0.f;
			float lenA = 0.f;
			
			int nxt = (i==poly.pts.size()-1) ? 0: i+1;
			 
			if( before >= 0 && isIntersectionPoly(poly.pts[i], poly.pts[nxt], polyQuarterQuads[before].pts, iPtBefore) )
			{
				
				// find length between point in poly and new end point
				if( pointInPolygon(poly.pts[i].x, poly.pts[i].y, polyQuarterQuads[before].pts) )
					lenB = (poly.pts[i].x-iPtBefore.x)*(poly.pts[i].x-iPtBefore.x)+(poly.pts[i].y-iPtBefore.y)*(poly.pts[i].y-iPtBefore.y);
				
				else if( pointInPolygon(poly.pts[nxt].x, poly.pts[nxt].y, polyQuarterQuads[before].pts) )
					lenB = (poly.pts[nxt].x-iPtBefore.x)*(poly.pts[nxt].x-iPtBefore.x)+(poly.pts[nxt].y-iPtBefore.y)*(poly.pts[nxt].y-iPtBefore.y);
			
			}else if( before >= 0 && 
					 pointInPolygon(poly.pts[i].x, poly.pts[i].y, polyQuarterQuads[before].pts) &&
					 pointInPolygon(poly.pts[nxt].x, poly.pts[nxt].y, polyQuarterQuads[before].pts) )
			{
				lenB = (poly.pts[nxt].x-poly.pts[i].x)*(poly.pts[nxt].x-poly.pts[i].x)+(poly.pts[nxt].y-poly.pts[i].y)*(poly.pts[nxt].y-poly.pts[i].y);
			}
			
			
			if( after >= 0 && isIntersectionPoly(poly.pts[i], poly.pts[nxt], polyQuarterQuads[after].pts, iPtAfter) )
			{
				// find length between point in poly and new end point
				if( pointInPolygon(poly.pts[i].x, poly.pts[i].y, polyQuarterQuads[after].pts) )
					lenA = (poly.pts[i].x-iPtAfter.x)*(poly.pts[i].x-iPtAfter.x)+(poly.pts[i].y-iPtAfter.y)*(poly.pts[i].y-iPtAfter.y);
				
				else if( pointInPolygon(poly.pts[nxt].x, poly.pts[nxt].y, polyQuarterQuads[after].pts) )
					lenA = (poly.pts[nxt].x-iPtAfter.x)*(poly.pts[nxt].x-iPtAfter.x)+(poly.pts[nxt].y-iPtAfter.y)*(poly.pts[nxt].y-iPtAfter.y);
				
			}else if( after >= 0 && 
					 pointInPolygon(poly.pts[i].x, poly.pts[i].y, polyQuarterQuads[after].pts) &&
					 pointInPolygon(poly.pts[nxt].x, poly.pts[nxt].y, polyQuarterQuads[after].pts) )
			{
				lenA = (poly.pts[nxt].x-poly.pts[i].x)*(poly.pts[nxt].x-poly.pts[i].x)+(poly.pts[nxt].y-poly.pts[i].y)*(poly.pts[nxt].y-poly.pts[i].y);
			}
			
			if( lenA > lenB )
				sideIds[i] = after;
			else if( lenB > lenA )
				sideIds[i]= before;
				
			
		}
	}
}

void AnalysisSideAssigner::checkAngleDifference(enExpandData expanders)
{
	for( int i = 0; i < poly.pts.size(); i++)
	{
		if(sideIds[i] < 0 )
		{
			int before = getBefore(i);
			int after = getAfter(i);
			
			int nxt = (i==poly.pts.size()-1) ? 0: i+1;
			
			ofxVec2f diffVec = ofxVec2f(poly.pts[i].x,poly.pts[i].y) - ofxVec2f(poly.pts[nxt].x,poly.pts[nxt].y);
			ofxVec2f pp = diffVec.perpendicular();
			
			// calc angle of side pp
			float anglePP = RAD_TO_DEG*atan2(pp.y,pp.x);
			if(anglePP < 0 ) anglePP = RAD_TO_DEG*atan2(-pp.y,-pp.x);
			
			// caclulate angle diff of before to side
			float angleB = RAD_TO_DEG*atan2(expanders.pps[before].y,expanders.pps[before].x);
			if(angleB<0) angleB = RAD_TO_DEG*atan2(-expanders.pps[before].y,-expanders.pps[before].x);

			// calc angle diff of after to side
			float angleA = RAD_TO_DEG*atan2(expanders.pps[after].y,expanders.pps[after].x);
			if(angleA<0) angleA = RAD_TO_DEG*atan2(-expanders.pps[after].y,-expanders.pps[after].x);
			
			float diffB = fabs(anglePP-angleB);
			float diffA = fabs(anglePP-angleA);
			
			//cout << "before " << before << " after " << after << " aPP: " << anglePP << " aB " << angleB << " aA " << angleA << " diffB " << diffB << " diffA " << diffA << endl;
			
			// take smaller difference
			if( diffB < diffA)	sideIds[i] = before;
			else				sideIds[i] = after;
		}
	}
	
}

