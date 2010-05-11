/*
 *  ensancheUtils.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 1/19/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheUtils.h"

string getTimeString()
{
	return ofToString( ofGetDay() ) + "_" + ofToString( ofGetMonth() )  + "_" + ofToString( ofGetYear() )  + "_" + ofToString( ofGetHours() )  + "_" + ofToString( ofGetMinutes() )  + "_" + ofToString( ofGetSeconds() ); 
}

void enDrawGrid( int xSpace, int ySpace )
{
	for( int x = 0; x < ofGetWidth()+xSpace; x+=xSpace)
	{
		ofLine(x,0,x,ofGetHeight());
	}
	
	for( int y = 0; y < ofGetHeight()+ySpace; y+=ySpace)
	{
		ofLine(0,y,ofGetWidth(),y);
	}
}

string subsChars(string origString){
	
	charSubstitution chars[]={
		{"à","\xE0"}, {"á","\xE1"}, {"â","\xE2"}, {"ã","\xE3"}, {"ä","\xE4"}, 
		{"æ","\xE6"}, {"ò","\xF2"}, {"ó","\xF3"} ,{"ô","\xF4"}, {"õ","\xF5"}, 
		{"ö","\xF6"}, {"ù","\xF9"}, {"ú","\xFA"}, {"û","\xFB"}, {"ü","\xFC"},
		{"è","\xE8"}, {"é","\xE9"}, {"ê","\xEA"}, {"ë","\xEB"}, {"ì","\xEC"}, 
		{"í","\xED"}, {"î","\xEE"}, {"ï","\xEF"}, {"ç","\xE7"}, {"Ç","\xC7"}, 
		{"ñ","\xF1"}
	};
	
	for(int i=0; i<27; i++){
		
		while(origString.find(chars[i].character)!=string::npos){
			origString = origString.substr(0,origString.find(chars[i].character)) + chars[i].code + origString.substr(origString.find(chars[i].character)+2);
			
		}
	};
	
	return origString;
}


void drawPolyIntoMe( ofxCvGrayscaleImage & image, vector<ofPoint> npts, int color )
{
	if( npts.size() > 0 ) {
		CvPoint* cvpts = new CvPoint[npts.size()];
		for( int i=0; i < npts.size() ; i++ ) {
			cvpts[i].x = (int)npts[i].x;
			cvpts[i].y = (int)npts[i].y;
		}
		int nPts = npts.size();
		cvFillConvexPoly ( image.getCvImage(), cvpts, nPts,
						  CV_RGB(color,color,color) );
		delete cvpts;
	}
}


int getColor(int n)
{
	int tcolors = 11;
	
	int colors[] = {
		0x4D1225,
		0x5D6B22,
		0x026B66,
		0xBB9732,
		0x7AB041,
		0x1AD3C4,
		0xB02100,
		0x7F8C1B,
		0x8CB1B1,
		0xDC660F,
		0x192542
		
		/*0xff0000,
		0x00bdb2,
		0x3b2303,
		0x00ff00,
		0x0000ff,
		0x3b23dc,
		0x032506*/
	};
	
	return colors[ n%tcolors ];
	
}

