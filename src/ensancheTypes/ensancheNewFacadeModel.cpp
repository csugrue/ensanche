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

void EnsancheNewFacadeModel::insertFacadeLine( vector<ofPoint> fLine, int floorNum, int startPt, int endPt, int sideId )
{
	if( buildingFloors.size() < floorNum ) return;
	
	// create a temporary poly to hold the point data
	polySimple tempPoly = buildingFloors[ floorNum ];
	
	// create temporary struct to hold the sideId data and endPoint data
	
	// create temporary struct for walls of this floor
	
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
	
	
}