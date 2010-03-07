/*
 *  uuid.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/6/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

// from http://www.ietf.org/rfc/rfc4122.txt
//#include "copyrt.h"
#pragma once

#include "sysdep.h"

//#undef uuid_tt

typedef struct {
    unsigned32  time_low;
    unsigned16  time_mid;
    unsigned16  time_hi_and_version;
    unsigned8   clock_seq_hi_and_reserved;
    unsigned8   clock_seq_low;
    byte        node[6];
} uuid_tt;


/* uuid_create -- generate a UUID */
int uuid_create(uuid_tt * uuid);

/* uuid_create_md5_from_name -- create a version 3 (MD5) UUID using a
 "name" from a "name space" */
/*void uuid_create_md5_from_name(
							   uuid_t *uuid,         // resulting UUID 
							   uuid_t nsid,          // UUID of the namespace 
							   void *name,           // the name from which to generate a UUID 
							   int namelen           // the length of the name 
);
*/
/* uuid_create_sha1_from_name -- create a version 5 (SHA-1) UUID
 using a "name" from a "name space" */
void uuid_create_sha1_from_name(
								
								uuid_t *uuid,         /* resulting UUID */
								uuid_t nsid,          /* UUID of the namespace */
								void *name,           /* the name from which to generate a UUID */
								int namelen           /* the length of the name */
);

/* uuid_compare --  Compare two UUID's "lexically" and return
 -1   u1 is lexically before u2
 0   u1 is equal to u2
 1   u1 is lexically after u2
 Note that lexical ordering is not temporal ordering!
 */
int uuid_compare(uuid_tt *u1, uuid_tt *u2);

