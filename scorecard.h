/*
 * =====================================================================================
 *
 *       Filename:  scorecard.h
 *
 *    Description:  the header file for scorecard
 *					defines the score card interface
 *
 *        Version:  1.0
 *        Created:  12/25/2009 04:31:13 PM
 *       Compiler:  gcc
 *
 *         Author:  Adriaan Larmuseau
 *
 * =====================================================================================
 */

#ifndef DOBBELEN_SCORECARD_H
#define DOBBELEN_SCORECARD_H

#include <stdbool.h>
#include "util.h"

/*-----------------------------------------------------------------------------
 *  typedef for 8 bit word - this is large enough
 *-----------------------------------------------------------------------------*/
typedef unsigned char u8;

enum {FULLCARD = 0xFF}; /** a full card has all 8 bits set on the done variable */

enum /** score related constants */ 
{
	COMBOCAT = 6,
	FIVETYPES = 50,
	COMBOBONUS = 35,	
};


/*-----------------------------------------------------------------------------
 *  Score Card Struct
 *-----------------------------------------------------------------------------*/
 /** datastructure minimized to save space and boost efficiency */
struct scorecard_t
{
	u8 done;
	u8 comboscores[COMBOCAT];
	u8 fivesame;
	u8 chance;
};

/*-----------------------------------------------------------------------------
 *  function interfaces
 *-----------------------------------------------------------------------------*/
extern bool isdone(struct scorecard_t * sc);
extern void setcombo(struct scorecard_t * sc,int combo,u8 score);
extern bool checkcombo(struct scorecard_t * sc,int combo);
extern void setfivesame(struct scorecard_t * sc,bool score);
extern bool checkfivesame(struct scorecard_t * sc);
extern void setchance(struct scorecard_t *sc,u8 score);
extern bool checkchance(struct scorecard_t * sc);
extern void printcard(struct scorecard_t * sc);
extern int * getfreecombos(struct scorecard_t * sc,int * size);
extern int calculatescore(struct scorecard_t * sc,int * comboresults);
extern int calculatecomboscore(struct scorecard_t * sc);
#endif
