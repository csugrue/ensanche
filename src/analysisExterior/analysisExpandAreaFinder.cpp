/*
 *  analysisSideExpandMethod.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "analysisExpandAreaFinder.h"


AnalysisExpandAreaFinder::AnalysisExpandAreaFinder()
{
bSetup = false;
}


AnalysisExpandAreaFinder::~AnalysisExpandAreaFinder()
{
}

void AnalysisExpandAreaFinder::setup( EnsancheBarrio barrioOriginal )
{
	
	// finds the minimum enclosing rectangle
	findMinRect( barrioOriginal );
	
	// finds perpendicular vectors of each side of enclosing rectangle
	findPPs(expandersOriginal);
	
	// create data type to match the originals, will use these to make an expansion but preserve originals
	expanders.clear();
	expanders.assign( expandersOriginal.begin(),expandersOriginal.end() );
	
	// store buildings
	buildings.clear();
	buildings.assign( barrioOriginal.buildings.begin(), barrioOriginal.buildings.end() );
	
	// we are set to go!
	bSetup = true;
	
	expand(0);
}



void AnalysisExpandAreaFinder::clear()
{
	for( int i = 0; i < expandersOriginal.size(); i++)
		expandersOriginal[i].pps.clear();
	
	expandersOriginal.clear();
	
	for( int i = 0; i < expanders.size(); i++)
		expanders[i].pps.clear();
	
	expanders.clear();
	
	buildings.clear();
	
	bSetup = false;
}

//-------------------------- DRAWING

void AnalysisExpandAreaFinder::drawIntersections( int me )
{
	for( int i = 0; i < expandersOriginal.size(); i++)
	{
		
		if( me >= 0 && i != me ) continue;
		
		ofSetColor( getColor(i) );
		
		//for( int j = 0; j < expandersOriginal[i].ePts.size(); j++)
		//	ofLine(expandersOriginal[i].ePts[j].x, expandersOriginal[i].ePts[j].y, expandersOriginal[i].sPts[j].x, expandersOriginal[i].sPts[j].y);
		
		
		
	}
	
	
	for( int i = 0; i < expanders.size(); i++)
	{
		
		if( me >= 0 && i != me ) continue;
		
		ofSetColor( getColor(i) );
		
		for( int j = 0; j < expanders[i].ePts.size(); j++)
			ofLine(expanders[i].ePts[j].x, expanders[i].ePts[j].y, expanders[i].sPts[j].x, expanders[i].sPts[j].y);
		
		
		
	}
	
}

void AnalysisExpandAreaFinder::drawShortest( int me )
{
	for( int i = 0; i < expandersOriginal.size(); i++)
	{
	
		if( me >= 0 && i != me ) continue;

		ofSetColor( getColor(i) );

		for( int j = 0; j < expanders[i].iSides.size(); j++)
		{
			ofSetColor( getColor(expanders[i].iSides[j].mySide ) );
			//if(expanders[i].iSides[j].bSideStopper)
				//ofSetColor(0xff0000);
				ofLine(expanders[i].iSides[j].sPt.x, expanders[i].iSides[j].sPt.y, expanders[i].iSides[j].ePt.x, expanders[i].iSides[j].ePt.y);
			
		}
		for( int j = 0; j < expanders[i].iSides.size(); j++)
		{
			if(expanders[i].iSides[j].bSideStopper){
			 ofSetColor(0,0,0);
			 ofFill();
			}else{
				ofSetColor( getColor(expanders[i].iSides[j].mySide ) );
				ofNoFill();
			}
			
			ofCircle(expanders[i].iSides[j].sPt.x, expanders[i].iSides[j].sPt.y,.25f);
			ofCircle(expanders[i].iSides[j].ePt.x, expanders[i].iSides[j].ePt.y,.25f);
		}
		
		
	}	
}

void AnalysisExpandAreaFinder::drawExpandAreas( int me )
{
	for( int i = 0; i < expandersAreas.size(); i++)
	{
		ofSetColor( getColor(i) );
		ofNoFill();
		//expandersAreas[i].poly.draw();
		for( int j = 0; j < expandersAreas[i].poly.pts.size()-1; j++)
		{
			ofSetColor( getColor(j) );
			
			if(expandersAreas[i].bSideMaxed[j]) ofSetColor(0xff0000);
			
			ofLine(expandersAreas[i].poly.pts[j].x,expandersAreas[i].poly.pts[j].y,
				   expandersAreas[i].poly.pts[j+1].x,expandersAreas[i].poly.pts[j+1].y);
		}
		
		ofFill();
		ofCircle(expandersAreas[i].poly.pts[0].x, expandersAreas[i].poly.pts[0].y, .1);
	}
}

void AnalysisExpandAreaFinder::drawMinRectangles( int me  )
{
	for( int i = 0; i < expandersOriginal.size(); i++)
	{
		ofSetColor( getColor(i) );
		ofNoFill();
		for( int j = 0; j < expandersOriginal[i].poly.pts.size()-1; j++)
		{
			ofSetColor( getColor(j) );
						
			ofLine(expandersOriginal[i].poly.pts[j].x,expandersOriginal[i].poly.pts[j].y,
				   expandersOriginal[i].poly.pts[j+1].x,expandersOriginal[i].poly.pts[j+1].y);
		}
		
		ofFill();
		ofCircle(expandersOriginal[i].poly.pts[0].x, expandersOriginal[i].poly.pts[0].y, .1);
	}
}


void AnalysisExpandAreaFinder::draw()
{
	
	ofPushStyle();
	//ofEnableAlphaBlending();
	
	for( int i = 0; i < expandersOriginal.size(); i++)
	{
		
		ofSetColor(0xab2869);//200,0,0,255);
		ofSetColor( getColor(i) );
		
		ofPushMatrix();
			ofBeginShape();
				for( int j = 0; j < 4; j++)
				{
					ofSetColor( getColor(j) );
					ofVertex(expandersOriginal[i].pt[j].x, expandersOriginal[i].pt[j].y);
				}
			ofEndShape(true);
		ofPopMatrix();
		
		/*ofSetColor(getColor(i));
		for( int j = 0; j < expanders[i].sPts.size(); j++)
		{
			ofCircle(expanders[i].sPts[j].x, expanders[i].sPts[j].y,.25);
		}*/
		
						
	}
	ofPopStyle();
	
}

//-------------------------- ANALYSIS

void AnalysisExpandAreaFinder::findMinRect( EnsancheBarrio barrioOriginal )
{
	
	clear();
	
	for( int i = 0; i < barrioOriginal.buildings.size(); i++)
	{
		int npts = barrioOriginal.buildings[i].buildingPoly.pts.size();
		
		if( npts <= 0 ) continue;
		
		CvPoint2D32f* cvpts = new CvPoint2D32f[npts];
		for( int j=0; j < npts ; j++ ) {
			cvpts[j].x = barrioOriginal.buildings[i].buildingPoly.pts[j].x;
			cvpts[j].y = barrioOriginal.buildings[i].buildingPoly.pts[j].y;
		}
		
		CvMat mat = cvMat( 1, npts, CV_32FC2, cvpts );
		
		enExpandData eData;
		eData.box = cvMinAreaRect2( &mat, 0 );
		cvBoxPoints( eData.box, eData.pt);
		
		// copy the points from the cvBox to a polygon object, make closed so that last == first 
		for( int j = 0; j < 4; j++) 
			eData.poly.pushVertex(ofPoint(eData.pt[j].x,eData.pt[j].y));
		eData.poly.pushVertex(ofPoint(eData.pt[0].x,eData.pt[0].y));
		
		eData.uid = barrioOriginal.buildings[i].uid;
		eData.nFloors = barrioOriginal.buildings[i].nFloors;
		
		expandersOriginal.push_back(eData);
		
		delete cvpts;
	}
	

}

void AnalysisExpandAreaFinder::findPPs(vector<enExpandData> & epData)
{
	for( int i = 0; i < epData.size(); i++)
	{
	
		epData[i].pps.clear();
		
		for( int j = 0; j < epData[i].poly.pts.size()-1; j++)
		{
			ofxVec2f diffVec = ofxVec2f(epData[i].poly.pts[j].x,epData[i].poly.pts[j].y) - ofxVec2f(epData[i].poly.pts[j+1].x,epData[i].poly.pts[j+1].y);
			ofxVec2f pp = diffVec.perpendicular();
			epData[i].pps.push_back(pp);
		}
		
	}
	
}


void AnalysisExpandAreaFinder::expand( float step )
{
	if(!bSetup) return;
	

	//----- check if we have expand structs
	if(expandersAreas.size() != expanders.size() )
	{
		expandersAreas.clear();
		for( int i = 0; i < expanders.size(); i++)
		{
			enExpandAreas tempArea;
			memset(tempArea.bSideMaxed,false,4*sizeof(bool));
			tempArea.poly = expanders[i].poly;			
			tempArea.poly.bDrawnWithPoints = false;
			tempArea.uid = expanders[i].uid;
			expandersAreas.push_back(tempArea);
		}
		
	}
	
	findPPs(expanders);
	//if(step==0)
	findIntersectingLines(expanders);
	findShortestDistances(expanders);
	
	
	//----- for all buildings, expand out by step
	for( int i = 0; i < expanders.size(); i++)
	{
		//cout << "num pts" << expandersAreas[i].poly.pts.size() << endl;
		
		// expand side
		for( int j = 0; j < expandersAreas[i].poly.pts.size()-1; j++)
		{

			if(expandersAreas[i].bSideMaxed[j]) continue;
			
			expandersAreas[i].poly.pts[j].x += step * expanders[i].pps[j].x;
			expandersAreas[i].poly.pts[j].y += step * expanders[i].pps[j].y;
			
			if(j < expandersAreas[i].poly.pts.size()-1)
			{
			expandersAreas[i].poly.pts[j+1].x += step * expanders[i].pps[j].x;
			expandersAreas[i].poly.pts[j+1].y += step * expanders[i].pps[j].y;
			}
			
			if( j == 0 )
			{
				int lst = expandersAreas[i].poly.pts.size()-1;
				expandersAreas[i].poly.pts[lst].x += step * expanders[i].pps[j].x;
				expandersAreas[i].poly.pts[lst].y += step * expanders[i].pps[j].y;
			}
			
		}
		
		if( expandersAreas[i].poly.pts.size() > 1 )
		{
			int lst = expandersAreas[i].poly.pts.size()-1;
			expandersAreas[i].poly.pts[0].x = expandersAreas[i].poly.pts[lst].x;
			expandersAreas[i].poly.pts[0].y = expandersAreas[i].poly.pts[lst].y;
		}
		
		expanders[i].poly = expandersAreas[i].poly;
		cout << " expanders[i].poly.pts.size()  " << expanders[i].poly.pts.size() << endl;
	}
	
	
	limitExpansion(expanders);
	
}

void AnalysisExpandAreaFinder::findIntersectingLines(vector<enExpandData> & epData)
{
	
	float step = 1.f;
	float dLen = 5000.f; // NOTE: this is rather arbitrary, and should base on group bounding box??
	
	// for each side, find spaced out intersections with other buildings but only keeping the closest
	for( int i = 0; i < epData.size(); i++)
	{
		
		epData[i].sPts.clear();
		epData[i].ePts.clear();
		epData[i].iSides.clear();
		
		for( int j = 0; j < epData[i].poly.pts.size()-1; j++)
		{
			ofxVec2f diffVec	= ofxVec2f(epData[i].poly.pts[j].x,epData[i].poly.pts[j].y) - ofxVec2f(epData[i].poly.pts[j+1].x,epData[i].poly.pts[j+1].y);
			float len			= diffVec.length();
			ofxVec2f normVec	= diffVec.normalized();
			
			int npts = (int)(len/step);
			
			
			// go through inserted points on the side and find the intersection with the other building
			// start at 1 so we dont have corner w strange pp?
			for( int p = 1; p < npts; p++)
			{
				ofPoint endPts[2];
				endPts[0] = ofPoint(epData[i].poly.pts[j].x - (step*p)*normVec.x,epData[i].poly.pts[j].y - (step*p)*normVec.y);
				
				ofPoint pp = epData[i].pps[j];
				
				// ? not in use... make corner an angle ray
				if( p == 0 )
				{
					int lst = (j+1)%4;
					//pp.x = .5*pp.x-.5*epData[i].pps[lst].x;
					//pp.y = .5*pp.y-.5*epData[i].pps[lst].y;
				}
				
				endPts[1] = ofPoint(endPts[0].x + dLen * pp.x, endPts[0].y + dLen * pp.y);
				epData[i].sPts.push_back(endPts[0]);
				epData[i].ePts.push_back(endPts[1]);
				
				ofPoint closestPoint = ofPoint(0,0);
				int closestSide = 0;
				
				bool bRecordedSide = false;
				
				int checks = 0;
				
				// check against all buildings
				for( int k = 0; k <  epData.size(); k++)
				{
					//if(epData[k].uid == epData[i].uid ) continue;
					if( k == i ) continue;
					
					// only want those that intersect with the other building
					if( findClosestIntersectionLineAndPoly(endPts[0], endPts[1], epData[k].poly.pts, closestPoint,closestSide) )
					{
						int lpt = epData[i].ePts.size()-1;
						float distNow = (endPts[0].x-epData[i].ePts[ lpt ].x)*(endPts[0].x-epData[i].ePts[ lpt ].x)+(endPts[0].y-epData[i].ePts[ lpt ].y)*(endPts[0].y-epData[i].ePts[ lpt ].y);
						float distNew = (endPts[0].x-closestPoint.x)*(endPts[0].x-closestPoint.x)+(endPts[0].y-closestPoint.y)*(endPts[0].y-closestPoint.y);
						
						// replace or add only if this is the first or the distance is shorter than the last
						// presumably if the line is going through to opposite side, there is a shorter option that will replace it
						if( checks == 0 || ( distNow == 0 || distNew < distNow) ) 
						{
							expanders[i].ePts[ lpt ] = closestPoint;
							
							// insert a new pairing if it does not exist
							if ( epData[i].sisMeSides[j].sister.find(k) == epData[i].sisMeSides[j].sister.end( ) ){
								bRecordedSide = true;
								epData[i].sisMeSides[j].sister.insert(pair<int, int>(k, closestSide));
								// cout << "insert me: " << i << " side: " << j << " sister: " << k << " side: " << closestSide << endl;
							}
							
							// replace the data with new closest
							epData[i].sisMeSides[j].sister[k] = closestSide;
						
							
						}
						
					}
					
					checks++;
				}
				
			} // end each pt along side
			
		} // end each expander side
		
		
		
	}// end each expander
	
	
	// debug printout
	/*
	for( int i = 0; i < epData.size(); i++)
	{ 
		for( int j = 0; j < 4; j++)
			for( int k = 0; k < epData[i].sisMeSides[j].sister.size(); k++)
				if(epData[i].sisMeSides[j].sister.find(k) != epData[i].sisMeSides[j].sister.end( ) ) 
					cout << "me: " << i << " side: " << j << " sister: " << k << " side: " << epData[i].sisMeSides[j].sister[k] << endl;
	}
	*/
	
}

void AnalysisExpandAreaFinder::findShortestDistances(vector<enExpandData> & epData)
{
	
	//---- loop thru all expand rects
	for( int i = 0; i < epData.size(); i++)
	{
		epData[i].iSides.clear();
		
		//--- for all sides of the enclosing rect
		for(int j = 0; j < epData[i].poly.pts.size()-1; j++)
		{
			
			//--- check against each other building that is not me
			for( int k = 0; k < epData.size(); k++)
			{
				if( k == i ) continue;
				
				ofPoint pt1 = epData[i].poly.pts[j];
				ofPoint pt2 = epData[i].poly.pts[j+1];
				
				//--- check if either of my pts are in them, if so stop this side from moving now
				if( pointInPolygon(pt1.x, pt1.y, epData[k].poly.pts) )	
					expandersAreas[i].bSideMaxed[j] = true;
				
				if(pointInPolygon(pt2.x, pt2.y, epData[k].poly.pts))
					expandersAreas[i].bSideMaxed[j] = true;
				
				//--- check if my side intersects with them
				if( isIntersectionPoly(pt1,pt2,epData[k].poly.pts) )
					expandersAreas[i].bSideMaxed[j] = true;
					
				
				 
				//--- check against all sides of this one
				for( int p = 0; p < epData[k].poly.pts.size()-1; p++)
				{
					
					
					//if we have a pairing
					if( 
					epData[i].sisMeSides[j].sister.find(k) != epData[i].sisMeSides[j].sister.end( )  &&
					epData[i].sisMeSides[j].sister[k] == p) 
						;
					else 
						continue;
					
					//cout << "Match side j-->k : p - " << p << " " << epData[i].sisMeSides[j].sister[k] << endl;
					//--- find shortest distance
					
					ofPoint pt3 = epData[k].poly.pts[p];
					ofPoint pt4 = epData[k].poly.pts[p+1];
					
					ofPoint eA = ofPoint(0,0,0);
					ofPoint eB = ofPoint(0,0,0);
					
					if( !minLineBetweenLineSegments(  pt1,  pt2,  pt3,  pt4, &eA, &eB) )
					{
						//--- check if it intersects any other building
						bool bIntersection = false;
						
						for( int h = 0; h < epData.size(); h++)
						{
							
							// if the the side is intersecting another, stop moving
							if( isIntersectionPoly(eA, eB, epData[h].poly.pts) )
								bIntersection = true;
								
							if( isIntersectionPoly(eA, eB, buildings[h].buildingPoly.pts) )
								bIntersection = true;
								
							// if extended point is in another building, stop moving								
							if( h != i && pointInPolygon(eB.x, eB.y, epData[h].poly.pts) )
								bIntersection = true;	
							
						}
						
						// record this data
						if( !bIntersection )
						{
								enShortestDistances iSide;
								epData[i].iSides.push_back(iSide);
								int lst = epData[i].iSides.size()-1;
								epData[i].iSides[lst].sPt = eA;
								epData[i].iSides[lst].ePt = eB;
								epData[i].iSides[lst].mySide = j;
								epData[i].iSides[lst].sisterSide = p;
								epData[i].iSides[lst].sisterIndex = k;
								epData[i].iSides[lst].bSideStopper = false;
						
						}	
					
						
					} // end if minLine
					
					
				} // end p
				
			} // end k for epData others
				
		} // end j epData[i].poly.pts
		
		
	} // end i for all epData
	
	
	
	//-------- for all short distances, check if there is a shorter for that pairing
	for( int i = 0; i < epData.size(); i++)
	{
		// create a vector that will temporarily hold the shortest of the shortest ;)
		vector<enShortestDistances> tSides;
		
		// check pairings for the other buildings
		for( int k = 0; k < buildings.size(); k++)
		{
			// skip ourselves obviously
			if( i == k ) continue;
			
			tSides.push_back( enShortestDistances() );
			float cDistSq = 0;
			int nChecks = 0;
			
			// check all the iSides to find the shortest
			for( int j = 0; j < epData[i].iSides.size(); j++)
			{
				// only look at match between same building
				if( epData[i].iSides[j].sisterIndex != k ) continue;
				
				ofxVec2f eA = ofxVec2f(epData[i].iSides[j].sPt.x,epData[i].iSides[j].sPt.y);
				ofxVec2f eB = ofxVec2f(epData[i].iSides[j].ePt.x,epData[i].iSides[j].ePt.y);
				float distsq = (eA-eB).lengthSquared();
				
				if( nChecks == 0 || distsq < cDistSq )
				{
					tSides.pop_back();
					tSides.push_back(epData[i].iSides[j]);
					cDistSq = distsq;
					nChecks++;
				}
				
			}
			
			// make sure we are not left with any empty ones
			if(nChecks == 0 ) tSides.pop_back();
		}
		
		// replace with the temp
		epData[i].iSides.clear();
		epData[i].iSides.assign(tSides.begin(), tSides.end() );
		
	}
	
	
}

void AnalysisExpandAreaFinder::limitExpansion(vector<enExpandData> & epData)
{
	// for each set of sides, test distance between and if it is close to min, stop expanding both
	for( int i = 0; i < epData.size(); i++)
	{
		// for each iSides
		for(int j = 0; j < epData[i].iSides.size(); j++)
		{
			int side = epData[i].iSides[j].mySide;
			int sisSide = epData[i].iSides[j].sisterSide;
			
			ofxVec2f eA = epData[i].iSides[j].sPt;
			ofxVec2f eB = epData[i].iSides[j].ePt;
			float dist = (eA-eB).length();
						
			// get min based on height of shorter of two buildings
			int sis = epData[i].iSides[j].sisterIndex;
			int nLessFloors = MIN(expanders[i].nFloors,expanders[sis].nFloors);					 
			float minDist = (1/3.f) * (nLessFloors*EN_FLOOR_HEIGHT);
			 
			if(dist <= minDist){
				expandersAreas[i].bSideMaxed[side] = true;
				expandersAreas[sis].bSideMaxed[sisSide] = true;
				epData[i].iSides[j].bSideStopper = true;
				//cout << "limit me: " << i << " myside: " << side << " sis " << sis << " siside " << sisSide << endl;
			}
			
		}
		
	}
	
}

