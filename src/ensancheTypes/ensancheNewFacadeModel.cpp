/*
 *  ensancheNewFacadeModel.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 5/19/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheNewFacadeModel.h"

EnsancheNewFacadeModel::EnsancheNewFacadeModel()
{
	
	offSet.set(0,0,0);
	center.set(0,0,0);
	rotation.set(0,0,0);
	lastMouse.set(0,0);
	
	nPts		= 0;
	nFloorPts	= 0;
	nTexPts		= 0;
	scale		= 1;
	nFloors		= 0;

	editMode = MODEL_EDIT_MODE_NORMAL;
	
	bSetWallTexture = false;
	memset(bSetWallTextures,0,MODEL_T_TEXTURES*sizeof(bool));
	
	memset(nptsv,0,MODEL_T_TEXTURES*sizeof(int));
	memset(ntexptsv,0,MODEL_T_TEXTURES*sizeof(int));
	
	bEnabled = true;

}

EnsancheNewFacadeModel::~EnsancheNewFacadeModel()
{
	if( nPts > 0 )		delete [] pts;
	if(nFloorPts > 0)	delete [] floorPts;
	if(nTexPts > 0)		delete [] texPts;
	
	//ofRemoveListener(ofEvents.mousePressed, this, &EnsancheNewFacadeModel::mousePressed);
	//ofRemoveListener(ofEvents.mouseDragged, this, &EnsancheNewFacadeModel::mouseDragged);
	
	buildingFloors.clear();
	
	//!! NOTE check if this is ok for delete other arrays!!
	for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
		if(nptsv[i] > 0) delete [] ptsv[i];
		if(ntexptsv[i] > 0) delete [] texptsv[i];
	}
}

EnsancheNewFacadeModel::EnsancheNewFacadeModel(const EnsancheNewFacadeModel & mom )
{
	bEnabled	= mom.bEnabled;
	offSet		= mom.offSet;
	center		= mom.center;
	rotation	= mom.rotation;
	scale		= mom.scale;
	editMode	= mom.editMode;
	
	buildingFloors.assign(mom.buildingFloors.begin(),mom.buildingFloors.end());
	
	lastMouse	= mom.lastMouse;
	nFloors		= mom.nFloors;
	
	nPts		= mom.nPts;
	nFloorPts	= mom.nFloorPts;
	nTexPts		= mom.nTexPts;
	
	if(nPts > 0 ){
		pts = new float[nPts];
		memcpy(pts,mom.pts,mom.nPts*sizeof(float));
	}
	
	if(nFloorPts > 0 ){
		floorPts = new float[nFloorPts];
		memcpy(floorPts,mom.floorPts,mom.nFloorPts*sizeof(float));
	}
	
	if(nTexPts > 0 ){
		texPts = new float[nTexPts];
		memcpy(texPts,mom.texPts,mom.nTexPts*sizeof(float) );
	}
	
	// copy all data
	wallTexIds.assign(mom.wallTexIds.begin(),mom.wallTexIds.end());
	
	
	// NOTE: does not keep texture
	bSetWallTexture = false;
	
	momBuildingFloors.clear();
	momBuildingFloors.assign(mom.momBuildingFloors.begin(), mom.momBuildingFloors.end() );
	
	// endpoints + side ids for each floor
	floorData.clear();
	floorData.assign( mom.floorData.begin(),mom.floorData.end() );
	
	
	//!!! NOTE: texture data
	//	memset(nptsv,0,MODEL_T_TEXTURES*sizeof(int));
	// memset(ntexptsv,0,MODEL_T_TEXTURES*sizeof(int));
}

void EnsancheNewFacadeModel::setupFromBuilding(EnsancheBuilding buildingOriginal )
{
	EnsancheModelBuildingAdv::setupFromBuilding( buildingOriginal );
	
	// add data
	for( int i = 0; i < buildingFloors.size(); i++)
	{
		newFacadeFloorData fdata;
		floorData.push_back(fdata);
		for( int j = 0; j < 4; j++)
		{
			floorData[i].endPtIndex[j].set(0,0);
			floorData[i].sideAltered[j] = false;
		}
	}
	
}

void EnsancheNewFacadeModel::setFloor(EnsancheBuilding buildingFloor, int floorNum, vector<int> sideIds,  map<int,ofPoint> sideToEndPt)
{
	EnsancheModelBuildingAdv::setFloor( buildingFloor,  floorNum );
	
	if( floorData.size() > floorNum )
	{
		floorData[floorNum].sideIds.clear();
		
		// copy side ids
		for( int i = 0; i < sideIds.size(); i++)
		{
			floorData[floorNum].sideIds.push_back(sideIds[i]);
		}
		
		//if( epts.size() >= 4)
		//{
			for( int i = 0; i < 4; i++)
			{
				//map<int,ofPoint> sideToEndPt
				cout << "set ednpts " <<  sideToEndPt.find(i)->second.x << " " << sideToEndPt.find(i)->second.y << endl;
				floorData[floorNum].endPtIndex[i]= sideToEndPt.find(i)->second ;//epts[i].x,epts[i].y);
			}
		//}
		
	}
}

void EnsancheNewFacadeModel::insertFacadeLine( vector<ofPoint> fLine, int floorNum, int startPt, int endPt, int sideId )
{
	if( buildingFloors.size() < floorNum ) return;
	if( floorData.size() > floorNum && floorData[ floorNum ].sideAltered[ sideId ]==true ) return;
	
	cout << "INSERT FACADE LINE " << floorNum << endl;
	// create a temporary poly to hold the point data
	polySimple tempPoly = buildingFloors[ floorNum ];
		
	// ?? create temporary struct for walls of this floor
	
	// clear original data
	buildingFloors[ floorNum ].clear();
	
	// ? check winding of line ?? or assume it is ok...
	
	// start looping though the original, when the start point is reached, begin inserting the new line
	for( int i = 0; i < tempPoly.pts.size(); i++)
	{
		buildingFloors[ floorNum ].pushVertex( tempPoly.pts[i] );
		
		if( i == startPt){
			for( int j = 0; j < fLine.size(); j++)
				buildingFloors[ floorNum ].pushVertex( fLine[j] );
		}
		
	}
	
	if( floorData.size() > floorNum)
	{
		floorData[ floorNum ].sideAltered[ sideId ] = true; 
		
		// insert sides
		vector<int> tempSides;
		tempSides.assign( floorData[ floorNum ].sideIds.begin(),floorData[ floorNum ].sideIds.end() );
		floorData[ floorNum ].sideIds.clear();
		 
		for( int i = 0; i < tempSides.size(); i++)
		{
			floorData[ floorNum ].sideIds.push_back( tempSides[i] );
			
			if( i == startPt){
				for( int j = 0; j < fLine.size(); j++)
					floorData[ floorNum ].sideIds.push_back(sideId);
			}
			
		}
		
		
		// insert new wall texture ids for inserted points
		//vector<wallTextureIds> wallTexIds;
		if( wallTexIds.size() > floorNum )
		{
			wallTextureIds tempWallIds;
			tempWallIds.tId.assign( wallTexIds[ floorNum ].tId.begin(),wallTexIds[ floorNum ].tId.end() );
			wallTexIds[ floorNum ].tId.clear();
			
			for( int i = 0; i < tempSides.size(); i++)
			{
				wallTexIds[ floorNum ].tId.push_back( wallTexIds[ floorNum ].tId[i] );
				
				// NOTE: this needs types to be applied to make sense!
				if( i == startPt){
					for( int j = 0; j < fLine.size(); j++)
						wallTexIds[ floorNum ].tId.push_back( j%MODEL_T_TEXTURES );
				}
				
			}
		}

		// insert ends
		for( int i = 0; i < 4; i++)
		{
			if( floorData[ floorNum ].endPtIndex[i].x > startPt ) floorData[ floorNum ].endPtIndex[i].x += fLine.size();
			if( floorData[ floorNum ].endPtIndex[i].y > startPt ) floorData[ floorNum ].endPtIndex[i].y += fLine.size();
		}
	}
	
	
	//generateModel(EN_FLOOR_HEIGHT);
}



void EnsancheNewFacadeModel::draw2D(bool bDrawSideColorCoded, bool bDrawWOffset)
{
	if(!bDrawSideColorCoded) EnsancheModelBuildingAdv::draw2D(bDrawWOffset);
	
	else{
		glPushMatrix();
		
		if(bDrawWOffset) glTranslatef(offSet.x,offSet.y,0);
		
		for(int i = 0; i < buildingFloors.size(); i++)
		{
			
			ofNoFill();
			for( int j = 0; j < buildingFloors[i].pts.size(); j++)
			{
					int nxt = (j==buildingFloors[i].pts.size()-1) ? 0 : j+1;
					int sideId = -1;
					
					if( floorData.size() > i && floorData[i].sideIds.size() > j )
					{
						switch( floorData[i].sideIds[j] )
						{
							case 0: ofSetColor(255,0,0); break;
							case 1: ofSetColor(0,255,0); break;
							case 2: ofSetColor(0,0,255); break;
							case 3: ofSetColor(255,0,255); break;
							case -2: ofSetColor(0,255,255); break;
							default: ofSetColor(0,0,0); break;
						
						}
						sideId = floorData[i].sideIds[j];
					}
					
				ofLine(buildingFloors[i].pts[j].x, buildingFloors[i].pts[j].y,buildingFloors[i].pts[nxt].x, buildingFloors[i].pts[nxt].y);
				
				ofNoFill();
				for( int k = 0 ; k < 4; k++)
				{
					if( floorData.size() > i && ((int)floorData[i].endPtIndex[k].x == j || (int)floorData[i].endPtIndex[k].y == j) ) 
					{
						//cout << "sideId " << sideId << "stpt  " << floorData[i].endPtIndex[k].x << " ept " << floorData[i].endPtIndex[k].y << endl;
						ofFill();
					}
				}
				ofCircle(buildingFloors[i].pts[j].x, buildingFloors[i].pts[j].y, .75);	
			}
			
			//buildingFloors[i].bDrawnWithPoints = false;
			//buildingFloors[i].draw();
		}
		
		glPopMatrix();
	
	}
	
}


void EnsancheNewFacadeModel::drawAllFloors(bool bDrawSideColorCoded)
{
	// find largest w and height
	float bbW = 0;
	float bbH = 0;
	
	for( int i = 0; i < buildingFloors.size(); i++)
	{
		ofRectangle bb = buildingFloors[i].getBoundingBox();
		if( i ==0 || bb.width > bbW ) bbW = bb.width;
		if( i ==0 || bb.height > bbH ) bbH = bb.height;
	}
	
	
	glPushMatrix();
	
		glTranslatef(0, -bbH, 0);
		
		for(int i = 0; i < buildingFloors.size(); i++)
		{
		
			//glPushMatrix();
				
				if(i>0) glTranslatef(bbW+(.1*bbW),0,0);
				
				ofNoFill();
				for( int j = 0; j < buildingFloors[i].pts.size(); j++)
				{
					int nxt = (j==buildingFloors[i].pts.size()-1) ? 0 : j+1;
					int sideId = -1;
				
					if( bDrawSideColorCoded && floorData.size() > i && floorData[i].sideIds.size() > j )
					{
						switch( floorData[i].sideIds[j] )
						{
							case 0: ofSetColor(255,0,0); break;
							case 1: ofSetColor(0,255,0); break;
							case 2: ofSetColor(0,0,255); break;
							case 3: ofSetColor(255,0,255); break;
							case -2: ofSetColor(0,255,255); break;
							default: ofSetColor(0,0,0); break;
							
						}
						sideId = floorData[i].sideIds[j];
					}
				
					ofLine(buildingFloors[i].pts[j].x, buildingFloors[i].pts[j].y,buildingFloors[i].pts[nxt].x, buildingFloors[i].pts[nxt].y);
				}
			
			//glPopMatrix();
		
		}
	
	glPopMatrix();
	


	
}

