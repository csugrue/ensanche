/*
 *  polyUtils.h
 *  ensanche_2.0
 *
 *  Created by Chris on 1/17/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"

bool pointInPolygon(float x, float y, vector<ofPoint>pts);

bool isInsideRect(float x, float y, ofRectangle rect);

bool intersectionTwoLines( ofPoint pt1, ofPoint pt2, ofPoint pt3, ofPoint pt4, ofPoint * iPt );
