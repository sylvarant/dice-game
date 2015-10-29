/*
 * =====================================================================================
 *
 *       Filename:  player.h
 *
 *    Description:  defines interfaces to different player strategies 
 *
 *        Version:  1.0
 *        Created:  12/27/2009 03:27:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ajhl
 *
 * =====================================================================================
 */

#ifndef DOBBELEN_PLAYER_H
#define DOBBELEN_PLAYER_H



#include "scorecard.h"


/*-----------------------------------------------------------------------------
 *  Couple Struct
 *-----------------------------------------------------------------------------*/
typedef struct 
{
	int x;
	int y;
}couple;


/*-----------------------------------------------------------------------------
 *  function interfaces
 *-----------------------------------------------------------------------------*/
extern void setnumberofthrows(int newthrows);
extern void setchancegoal(int newgoal);
extern void setrollfunction(int factor,int number);
extern void collectingplayer(struct scorecard_t * sc,int throw,int numberofdice,int * storage,int storagesize);
extern void categorybycategory(struct scorecard_t * sc,int throw,int numberofdice,int * storage,int storagesize);
extern void greedyplayer(struct scorecard_t * sc,int throw,int numberofdice,int * storage,int storagesize);
#endif
