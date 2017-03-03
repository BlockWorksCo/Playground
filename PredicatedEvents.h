//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//



#ifndef __PREDICATEDEVENTS_H__
#define __PREDICATEDEVENTS_H__


void CallWhenPredicateIsTrue( Handler handler, bool (*predicate)() );
void CheckPredicatedEventHandlers();

#endif


