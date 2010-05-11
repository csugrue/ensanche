/*
 *  ensancheUtils.h
 *  ensanche_2.0
 *
 *  Created by Chris on 1/19/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxCvMain.h"

//--------------------------------------------------
// data utils
//--------------------------------------------------

// returns a string with day_month_year_hour_mins_secs
string getTimeString();

//--------------------------------------------------
// drawing utils
//--------------------------------------------------

// draws a grid over entire screen with submitted spacing
void enDrawGrid( int xSpace, int ySpace );

// gets a unique color from a lookup table
int getColor(int n);

//--------------------------------------------------
// text utils
//--------------------------------------------------

typedef struct{
	string character;
	string code;
}charSubstitution;

string subsChars(string origString);

//--------------------------------------------------
// image utils
//--------------------------------------------------

void drawPolyIntoMe( ofxCvGrayscaleImage & image, vector<ofPoint> npts, int color );