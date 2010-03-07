/*
 *  googleQueryMaker.h
 *  ensanche_2.0
 *
 *  Created by Chris on 1/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ensancheConstants.h"

// http stuff ------------------------------------------
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

// image stuff ---------------------------------------

#include "Poco/URIStreamOpener.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include <memory>
#include <iostream>

using Poco::URIStreamOpener;
using Poco::Net::HTTPStreamFactory;


class googleQueryMaker{
	
	public:
	
		googleQueryMaker();
		~googleQueryMaker(){}
	

		
		// this must be called first so that an initial lat and longitude is found and remaining tiles depend on this
		void	setup( string enteredAddress );
		
		// returns a google query string for tile at tileX,tileY
		string getQueryForTile( int tileX, int tileY);
	
		string status;

	
	private:
	
		// create the query string to ask google for latitude and longitude from the user address
		string	getLatAndLongQueryString( string enteredAddress );
	
		// first must get a longitude and latitude out of the entered street address
		void findLatitudeAndLongitude( string enteredAddress );
		
		// fills a vector of string with geo location data from google. pass in query address from above. [2] and [3] are lat and long respectively
		void findGeoDataFromGoogle( string query );
	
		// returns the query string from google for getting a static map. must already be setup
		string	getMapQueryString( int pX, int pY );
	
		// function to split string on delimeters
		void splitString( string & str, vector<string> & split_strs, string delimeter);
	
		void	calcRadius();
	
		float	xToLongitude( float x);
		float	yToLatitude( float y);
		float	getWorldYFromLat( float y, float zoom );

	
		// lat and long for center of address
		float latitude;
		float longitude;
	
		//	 lat and long for current tile we want to load
		float clatitude;
		float clongitude;
	
		// radius of earth based on google map zoom
		float radius;
	
		// store geo location data from google
		vector<string> geocode_data;
	
	

	
};

