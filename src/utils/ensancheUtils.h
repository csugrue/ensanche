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

//--------------------------------------------------
// text utils
//--------------------------------------------------

typedef struct{
	string character;
	string code;
}charSubstitution;

string subsChars(string origString);