/*
 *  uidGenerator.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/7/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "uidGenerator.h"

string getUid()
{
	string myguid;
	
	uuid_tt u;
	int emsg = uuid_create(&u);
	
	if( emsg != 1 )
	{
		cout << "ERROR GENERATING UID" << endl;
		return "0";
	}
	
	char suid[64];
	
	sprintf(suid, "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
			u.time_hi_and_version, u.clock_seq_hi_and_reserved,
			u.clock_seq_low);
	
	for (int i = 0; i < 6; i++)
		sprintf(suid+strlen(suid),"%2.2x", u.node[i]);
	
	sprintf(suid+strlen(suid),"\0");
	
	myguid = suid;
	cout << "generated uid : " << myguid << endl;
	
	return myguid;
}