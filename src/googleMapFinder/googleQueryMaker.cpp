/*
 *  googleQueryMaker.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 1/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "googleQueryMaker.h"


	
googleQueryMaker::googleQueryMaker()
{
	latitude  = 0;
	longitude = 0;
		
	clatitude   = 0;
	clongitude  = 0;
	
	status = "ok";
	
	calcRadius();
}

void googleQueryMaker::setup( string enteredAddress )
{
	status = "ok";
	
	// create latitude and longitude query string from adderss to send to google
	string query = getLatAndLongQueryString( enteredAddress );
	cout << "Google query for lat/long: " << query << endl;
	
	// ask google for latitude and longitude
	findGeoDataFromGoogle(query);
	
	// store returned values as our map center point
	if( geocode_data.size() > 2 )
	{
		clatitude	= atof(geocode_data[2].c_str());
		clongitude	= atof(geocode_data[3].c_str());
		cout << "set lat long for center tile " << clatitude << " " << clongitude << endl;
		if( clatitude == 0 && clongitude == 0 )
		{
			status = "error";
			cout << "lat long error" << endl;
			return;
		}
		
	}else{
		status = "error";
		cout << "error in lat long " << endl;
		return;
	}
	
	
	// create query string for getting map
	query.clear();
	query = getMapQueryString(0, 0);
}

string googleQueryMaker::getQueryForTile( int tileX, int tileY)
{
	if( status == "error") return "error";
	else return getMapQueryString(tileX, tileY);
}


string	googleQueryMaker::getLatAndLongQueryString( string enteredAddress )
{
    vector<string> address_parts;
    splitString(enteredAddress,address_parts," ");
	
    enteredAddress.clear();
    for( int i = 0; i < address_parts.size(); i++)
    {
        enteredAddress.append(address_parts[i]);
        if(i < address_parts.size()-1) enteredAddress.append("+");
    }
	
    string geoCodeQuery;
    geoCodeQuery.append(GOOGLE_MAP_GEO_START);
    geoCodeQuery.append(enteredAddress);
    geoCodeQuery.append(GOOGLE_MAP_GEO_END);
    geoCodeQuery.append(GOOGLE_MAP_KEY);
	
	return geoCodeQuery;
}

void googleQueryMaker::findGeoDataFromGoogle( string query )
{
	
	try{
		URI uri(query.c_str());
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";
		
		HTTPClientSession session(uri.getHost(), uri.getPort());
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		session.sendRequest(req);
		
		session.setTimeout(Poco::Timespan(10,0));
		
		HTTPResponse res;
		istream& rs = session.receiveResponse(res);
		
		string responseBody;
		StreamCopier::copyToString(rs, responseBody);
		cout << responseBody << endl;
		
		geocode_data.clear();
		splitString(responseBody,geocode_data,",");
		
		//ofxHttpResponse response=ofxHttpResponse(res,rs,url);
		//ofxHttpEvents.notifyNewResponse(response);
		
	}catch (Exception& exc){
		//if(verbose) std::cerr << exc.displayText() << std::endl;
		//ofxHttpEvents.notifyNewError("time out ");
	}

}

//	-------------------------------------------
string googleQueryMaker::getMapQueryString( int pX, int pY )
{
	
	string query;
	
	float miny = getWorldYFromLat( clatitude, GOOGLE_MAP_ZOOM );
	float newY = miny-( (GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET)*pY);
	
	float sLat  = yToLatitude( newY );
	float sLong = clongitude + xToLongitude(GOOGLE_MAP_TILE_SIZE*pX);
	
	query += GOOGLE_MAP_START;
	query += ofToString(sLat);
	query += ",";
	query += ofToString(sLong);	
	query += GOOGLE_MAP_MID;
	query += GOOGLE_MAP_KEY;
	query += GOOGLE_MAP_END;
	query += "/staticmap.jpeg";
	
	cout << "map url: " << query << endl;
	
	return query;
}

void googleQueryMaker::splitString( string & str, vector<string> & tokens, string delimiters)
{
	
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
	
    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
	
}

//	-------------------------------------------
void  googleQueryMaker::calcRadius()
{
	float tiles = pow(2,GOOGLE_MAP_ZOOM);
	float circumference = GOOGLE_MAP_TILE_SIZE * tiles;
	radius = circumference/ (2 * PI);
	
	//printf(pow(2,GoogleMapParams.GOOGLE_ZOOM) );
	//printf( "radius: "+radius);
}

float googleQueryMaker::getWorldYFromLat( float y, float zoom )
{
	
	return round( 0-radius*log( 
							   ( 1+sin(y*PI/180.f) ) / (1-sin(y*PI/180.f) ) 
							) /2.f );
	
}

float  googleQueryMaker::xToLongitude( float x)
{
	float longRadians = x/radius;
	float longDegrees = RAD_TO_DEG*(longRadians);
	
	//The user could have panned around the world a lot of times.
	// Lat long goes from -180 to 180.  So every time a user gets 
	// to 181 we want to subtract 360 degrees.  Every time a user
	// gets to -181 we want to add 360 degrees. 
	
	int rotations = floor(  (longDegrees + 180.f)/360.f );
	float longi = longDegrees - (rotations * 360.f);
	return longi;
	
}

//	-------------------------------------------
float  googleQueryMaker::yToLatitude( float y)
{
	
	return ( PI/2.f-2*atan(exp((round(y))/radius)))*180.f/PI;
	
}

