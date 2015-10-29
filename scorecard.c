/*
 * =====================================================================================
 *
 *       Filename:  scorecard.c
 *
 *    Description:  all scorecard related utilities go here
 *
 *        Version:  1.0
 *        Created:  12/25/2009 04:25:00 PM
 *       Compiler:  gcc
 *
 *         Author:  Adriaan Larmuseau
 *
 * =====================================================================================
 */

#include "scorecard.h"


/*-----------------------------------------------------------------------------
 * static Function declarations -> keeps the compiler happy
 *-----------------------------------------------------------------------------*/
static u8 count_bits_set(u8 word);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isdone
 *  Description:  returns true when the card is done
 * =====================================================================================
 */
extern bool isdone(struct scorecard_t * sc)
{
	return ((*sc).done == FULLCARD); /** all flags have been set*/
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setcombo
 *  Description:  update card :: #combo = score
 * =====================================================================================
 */
extern void setcombo(struct scorecard_t * sc,int combo,u8 score)
{
	(*sc).comboscores[(combo-1)] = score;
	(*sc).done |= (1<<(combo-1));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  checkcombo
 *  Description:  check wether a combo not has been set
 * =====================================================================================
 */
extern bool checkcombo(struct scorecard_t * sc,int combo)
{
	return !(((*sc).done >> (combo-1)) & 0x1);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setfivesame
 *  Description:  a five different rolls have been achieved or is being zero'd
 * =====================================================================================
 */
extern void setfivesame(struct scorecard_t * sc,bool score)
{
	(*sc).fivesame = (score == true) ? FIVETYPES:0;
	(*sc).done |= (1<<6);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  checkfivesame
 *  Description:  check if the fivesame score has not been set
 * =====================================================================================
 */
extern bool checkfivesame(struct scorecard_t * sc)
{
	return !(((*sc).done>>6) & 0x1);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setchance
 *  Description:  set the chance score to the passed variable update done bitstring
 * =====================================================================================
 */
extern void setchance(struct scorecard_t * sc,u8 score)
{
	(*sc).chance = score;
	(*sc).done |= (1<<7);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  checkchance
 *  Description:  check if the chance score has not been set
 * =====================================================================================
 */
extern bool checkchance(struct scorecard_t * sc)
{
	return !(((*sc).done>>7) & 0x1);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printcard
 *  Description:  print the card information
 * =====================================================================================
 */
extern void printcard(struct scorecard_t *sc)
{
	printf("\n[1] [2] [3] [4] [5] [6] [5-1] [Chance] [Completion]\n");
	printf("%03d %03d %03d %03d %03d %03d  %03d     %03d      %#x\n",(*sc).comboscores[0],
		(*sc).comboscores[1],(*sc).comboscores[2],(*sc).comboscores[3],(*sc).comboscores[4],
		(*sc).comboscores[5],(*sc).fivesame,(*sc).chance,(*sc).done);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  count_bits_set
 *  Description:  return the number of bits set to 1 
 * =====================================================================================
 */
static u8 count_bits_set(u8 v)
{
	u8 c;
	for (c = 0; v; c++)
	{
		v &= v - 1; // clear the least significant bit set
	}
	return c;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getfreecombos
 *  Description:  return the numbers of possible comboscores on the score card
 *				  that have not been set
 * =====================================================================================
 */
extern int * getfreecombos(struct scorecard_t * sc,int * size)
{
	u8 firstsix = (*sc).done & 0x3F;
	*size = (6 - count_bits_set(firstsix));

	if((*size) == 0) return NULL;
	
	int * freenumbers = (int *) safeMalloc((sizeof(int)) * (*size));	
	for(int i = 0,j = 1; i < (*size) ; firstsix>>=1 , j++)
	{
		if(!(firstsix & 0x1)) freenumbers[i++] = j;
	}

	return freenumbers; 
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calculatescore
 *  Description:  return the total score of the given score card
 * =====================================================================================
 */
extern int calculatescore(struct scorecard_t * sc,int * comboresults)
{
	int result = 0;
	for(int i = 0; i < COMBOCAT ; i++)
	{
		result+=(*sc).comboscores[i];
	}
	
	(*comboresults) = result;
	if(result >= 63) result += COMBOBONUS;

	result += (*sc).fivesame;
	result += (*sc).chance;

	return result;	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calculatecomboscore
 *  Description:  calculate the score of the simple categories
 * =====================================================================================
 */
extern int calculatecomboscore(struct scorecard_t * sc)
{
	int result = 0;
	for(int i = 0; i < COMBOCAT ; i++)
	{
		result+=(*sc).comboscores[i];
	}
	return result;
}
