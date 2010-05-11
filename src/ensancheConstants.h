/*
 *  ensancheConstants.h
 *  ensanche_2.0
 *
 *  Created by Chris on 1/17/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

//-------------------------------------------
// Google Maps
//-------------------------------------------
// Note: find source on this data!
#define GOOGLE_MAP_METERS_TO_PIXEL	7.2f
#define GOOGLE_MAP_TILE_SIZE		256
#define GOOGLE_MAP_N_TILES_X		5
#define GOOGLE_MAP_N_TILES_Y		5
#define GOOGLE_MAP_ZOOM				19
#define GOOGLE_MAP_OFFSET			30
#define GOOGLE_MAP_T_TILES			25
#define GOOGLE_MAP_GEO_START		"http://maps.google.com/maps/geo?q="
#define GOOGLE_MAP_KEY				"ABQIAAAAE398XH88SJIy1LyxQlBFdRSkaQ3LALNvib05dEKVmODMy_o9KBTHmRevznyNWKXyi6d6-NYGctRm0A"
#define GOOGLE_MAP_GEO_END			"&output=csv&oe=utf8&sensor=true_or_false&key="
#define GOOGLE_MAP_START			"http://maps.google.com/staticmap?center="
#define GOOGLE_MAP_MID				"&zoom=19&size=256x256&maptype=satellite&format=jpg&key="
#define GOOGLE_MAP_END				"&sensor=false"

//-------------------------------------------
// Directories
//-------------------------------------------
#define XML_FILE_MAP				"_mapData.xml"
#define XML_FILE_USER				"_userData.xml"
#define XML_FILE_INT				"_interiorData.xml"
#define FLOOR_PLAN_DIRECTORY		"floorPlans/"
#define USER_DIRECTORY				"userData/"

//-------------------------------------------
// Structural 
//-------------------------------------------
#define WALL_ORIENT_H				0
#define WALL_ORIENT_V				1
#define EN_FLOOR_HEIGHT				4	//meters

//-------------------------------------------
// MISC
//-------------------------------------------
#define AUTO_SAVE_TIME				10


//-------------------------------------------
// TEXTURE TYPES
//-------------------------------------------
#define MODEL_TEX_WINDOW	0
#define MODEL_TEX_WALL		1
#define MODEL_TEX_SOLAR		2
