/*
 * =====================================================================================
 *
 *       Filename:  player.c
 *
 *    Description:  player code goes here
 *
 *        Version:  1.0
 *        Created:  12/27/2009 03:14:03 PM
 *       Compiler:  gcc
 *
 *         Author:  ajhl
 *
 * =====================================================================================
 */

#include "player.h"

/*-----------------------------------------------------------------------------
 *  static function declarations 
 *-----------------------------------------------------------------------------*/
static int roll(void);
static int * rollsetstandard(int size);
static int * rollsetfixed(int size);
static int compare (const void * a, const void * b);
static couple duplicate_check(int * arr,int size);
static int sum(int * arr,int size);
static bool hasint(int * arr,int size,int target);
static void removenumbers(int ** arr,int size,int target,int targetn);
static void conclusion(struct scorecard_t * sc,int * results,int sizeresult);
static int	numberofint(int * arr,int size,int target);
static int rollweighted(void);


/*-----------------------------------------------------------------------------
 *  static variables
 *-----------------------------------------------------------------------------*/
static int * (*rollset)(int size) = rollsetstandard; 
static int rollfactor = 3;
static int rollpreferednumber = 1;
static int CHANCEGOAL = 21;
static int MAXTHROWS = 3;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setnumberofthrows
 *  Description:  change the number of possible throws 
 * =====================================================================================
 */
extern void setnumberofthrows(int newthrows)
{
	MAXTHROWS = newthrows;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setchancegoal
 *  Description:  chance the boundary for going for chance -> if current result is larger
 *  than the goal we will file for a chance
 * =====================================================================================
 */
extern void setchancegoal(int newgoal)
{
	CHANCEGOAL = newgoal;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setrollfunction
 *  Description:  set the dice rolling function if the factor is larger than zero
 *	the rolling function will always through one weighted dice, weighted in size of factor
 * =====================================================================================
 */
extern void setrollfunction(int factor,int number)
{
	if(factor == 0)
	{
		rollset = rollsetstandard;
	}
	else
	{
		rollpreferednumber = number;
		rollfactor = factor;
		rollset = rollsetfixed;
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  roll
 *  Description:  generate number between 1 and 6
 * =====================================================================================
 */
static int roll(void)
{
	return 1 + (rand() / (RAND_MAX / 6 + 1)); /** http://c-faq.com/lib/randrange.html */
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rollweighted
 *  Description:  roll a weighted dice
 * =====================================================================================
 */
static int rollweighted()
{
	int rn = 1 + (rand() / (RAND_MAX / (6+rollfactor) + 1));
	if(rn > 6) 
	{
		rn = rollpreferednumber;
	}
	return rn;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rollset
 *  Description:  return a set of random dice rolls of the passed size
 * =====================================================================================
 */
static int * rollsetstandard(int size)
{
	int * ret = (int*) safeMalloc(sizeof(int) * size);

	for(int i = 0 ; i < size ; i++)
	{
		ret[i] = roll();
	}

	return ret;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rollweighted
 *  Description:  roll a set of random numbers + 1 weighed
 * =====================================================================================
 */
static int * rollsetfixed(int size)
{
	int * ret = (int*) safeMalloc(sizeof(int) * size);

	for(int i = 0 ; i < (size -1) ; i++)
	{
		ret[i] = roll();
	}

	if(size > 0) ret[(size-1)] = rollweighted();

	return ret; 
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  duplicate_check
 *  Description:  return the highest number with most duplicates
 * =====================================================================================
 */
static couple duplicate_check(int * arr,int size)
{
	int count[] = {0,0,0,0,0,0};

	for(int i = 0; i < size; i++)
	{
		count[(arr[i])-1]++;
	}
	
	couple max = {0,0};
	
	for(int i = 0; i < 6 ; i++)
	{
		if(count[i] > 0 && count[i] >= max.y)
		{
			max.x = (i+1);
			max.y = count[i];
		}
	}

	return max; 
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sum
 *  Description:  utility function calculate the sum of an pointer of ints
 * =====================================================================================
 */
static int sum(int * arr,int size)
{
	int result = 0;
	for(int i = 0; i < size ; i++)
		result += arr[i];
	return result;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hasint
 *  Description:  find a target in a array of ints
 * =====================================================================================
 */
static bool hasint(int * arr,int size,int target)
{
	for(int i = 0 ; i < size ; i++)
	{
		if(arr[i] == target) return true;
	}
	return false;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  removenumbers
 *  Description:  utility function :: remove target and duplicates form array
 * =====================================================================================
 */
static void removenumbers(int ** arr,int size,int target,int targetn)
{
	int * result = (int *)safeMalloc((size-targetn) * sizeof(int));
	for(int i = 0,j = 0; i < size ; i++)
	{
		if((*arr)[i] != target)
		{
			result[j++] = (*arr)[i];
		}
	}

	free((*arr));
	*arr = result;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  numberofint
 *  Description:  return the number of targets you can find in the list
 * =====================================================================================
 */
static int numberofint(int * arr,int size,int target)
{
	int result = 0;
	for(int i = 0; i < size; i++)
	{
		if(arr[i] == target) result++;
	}
	return result;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  conclusion
 *  Description:  help function for collectingplayer
 * =====================================================================================
 */
static void conclusion(struct scorecard_t * sc,int * results,int sizeresult)
{
	int size;
	int * combos = getfreecombos(sc,&size);
	if(combos != NULL)
	{
		if(hasint(results,sizeresult,combos[0]))
			setcombo(sc,combos[0],combos[0]);
		else
			setcombo(sc,combos[0],0);

		free(combos);
	}
	else if(checkchance(sc) == true)
	{
		setchance(sc,sum(results,sizeresult));
	}
	else
	{
		setfivesame(sc,false);
	}
	free(results);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  collectingplayer
 *  Description:  see the report for a detailed explenation of this tactic - it basicly
 *				  tries to make the largest possible combinations
 * =====================================================================================
 */
extern void collectingplayer(struct scorecard_t * sc,int throw,int numberofdice,int * storage,int storagesize)
{
	int * diceresults = rollset(numberofdice);
	++throw;

	/** if we think this is a high throw */
	if((*storage < 4) && checkchance(sc) == true &&  
	(storagesize * *storage + (sum(diceresults,numberofdice))) > CHANCEGOAL)
	{
		setchance(sc,(storagesize * *storage) + sum(diceresults,numberofdice));	
		free(diceresults);
		return;
	}

	/** get largest set of duplicates */
	couple max = duplicate_check(diceresults,numberofdice);

	if((*storage == 0 || (*storage) == max.x) && max.y == numberofdice)
	{
		if(checkfivesame(sc) == true)
		{
			setfivesame(sc,true);
			free(diceresults);
			return;
		}	
		else if(checkcombo(sc,max.x) == true)
		{
			setcombo(sc,max.x,(max.x * (numberofdice+storagesize)));
			free(diceresults);
			return;
		}
	}

	/** if we're not storing anything*/
	if(storagesize == 0)
	{
		if(throw < MAXTHROWS)
		{
			int newsize = numberofdice;
			while(max.y >= 2 && newsize > 0)
			{
				if(checkcombo(sc,max.x) == true)
				{
					*storage = max.x;
					free(diceresults);
					return collectingplayer(sc,throw,(numberofdice-max.y),storage,max.y);
				}
				removenumbers(&diceresults,newsize,max.x,max.y);
				newsize -= max.y;
				max = duplicate_check(diceresults,newsize);
			}
			free(diceresults);
			return collectingplayer(sc,throw,numberofdice,storage,storagesize);	
		}
		else /** last throw still no storage */
		{
			int newsize = numberofdice;
			int * copydiceresults = safeMalloc(sizeof(int) * numberofdice);

			for(int i = 0 ; i < numberofdice ; i++)
			{
				copydiceresults[i] = diceresults[i];
			}

			while(max.y >= 2 && newsize > 0)
			{		
				if(checkcombo(sc,max.x) == true)
				{
					setcombo(sc,max.x,(max.x * max.y));
					free(diceresults);
					free(copydiceresults);
					return;
				}
				removenumbers(&diceresults,newsize,max.x,max.y);
				newsize -= max.y;
				max = duplicate_check(diceresults,newsize);
			}
			free(diceresults);
			conclusion(sc,copydiceresults,numberofdice); 
			return;
		}
	}
	else /** if there is storage */
	{
		int add =numberofint(diceresults,numberofdice,(*storage));
		storagesize += add;
		numberofdice -= add;

		free(diceresults);

		if(throw < MAXTHROWS)
		{
			return collectingplayer(sc,throw,numberofdice,storage,storagesize);
		}
		else
		{
			setcombo(sc,(*storage),storagesize * (*storage));
			return;
		}	
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  categorybycategory
 *  Description:  see the report for a detailed explenation  - it basicly tries to fill
 *				  in the categories one by one
 * =====================================================================================
 */
extern void categorybycategory(struct scorecard_t * sc,int throw,int numberofdice,int * storage,int storagesize)
{
	int * diceresults = rollset(numberofdice);
	++throw;

	if(*storage == 0)
	{
		int combosize;
		int * freecombos = getfreecombos(sc,&combosize);
		if(freecombos != NULL)
		{
			*storage = freecombos[0];
			free(freecombos);
		}
		else
		{
			couple max = duplicate_check(diceresults,numberofdice);
			if(max.y == (numberofdice) && checkfivesame(sc) == true)
			{
				setfivesame(sc,true);	
				free(diceresults);
				return;
			}
			else if(checkchance(sc) == true)
			{
				setchance(sc,sum(diceresults,numberofdice));
				free(diceresults);
				return;
			}
			else
			{
				setfivesame(sc,false);
				free(diceresults);
				return;
			}
		}
	}

	int add = numberofint(diceresults,numberofdice,*storage);
	storagesize+= add;
	numberofdice -= add;

	if(numberofdice == 0)
	{
		if(checkfivesame(sc) == true)
		{
			setfivesame(sc,true);
		}
		else
		{
			setcombo(sc,*storage,*storage * storagesize);
		}
	}
	else 
	{
		/** set chance when rolling high */
		if((sum(diceresults,numberofdice) + (*storage * storagesize)) > CHANCEGOAL && checkchance(sc) == true)
		{
			setchance(sc,(sum(diceresults,numberofdice) + (*storage * storagesize))); 
		}
		else if(throw == MAXTHROWS)
		{
			setcombo(sc,*storage,storagesize * *storage);
		}
		else
		{
			free(diceresults);
			return categorybycategory(sc,throw,numberofdice,storage,storagesize);
		}
	}

	free(diceresults);
	return;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  greedyplayer
 *  Description:  allways tries to get the highest score for a thrown combination
 *				  see report for the full details
 * =====================================================================================
 */
extern void greedyplayer(struct scorecard_t * sc,int throw,int numberofdice,int * storage,int storagesize)
{
	int * diceresults = rollset(numberofdice);
	++throw;

	if(checkchance(sc) == true && (duplicate_check(storage,storagesize)).x < 5) 
	{
		int temp = sum(diceresults,numberofdice) + sum(storage,storagesize);
		if(temp > CHANCEGOAL) 
		{
			free(diceresults);
			setchance(sc,temp);
			if(storagesize > 0) free(storage);
			return;
		}
	}

	if(checkfivesame(sc) == true)
	{
		couple maxnew = duplicate_check(diceresults,numberofdice);
		couple maxold = duplicate_check(storage,storagesize);

		if(maxnew.x > 0 && maxnew.x == maxold.x && maxold.y == storagesize) 
		{
			if(maxnew.y == numberofdice)
			{
				free(diceresults);
				setfivesame(sc,true);
				if(storagesize > 0) free(storage);
				return;
			}
			else if(maxnew.y == numberofdice - 1)
			{
				storage = safeRealloc(storage,sizeof(int) *(storagesize+maxnew.y));
				for(int i = storagesize ; i < (storagesize +maxnew.y) ; i++)
				{
					storage[i] = maxnew.x;
				}
				storagesize +=  maxnew.y;
				free(diceresults);
				return greedyplayer(sc,throw,numberofdice-maxnew.y,storage,storagesize);
			}
		}
	}

	int combosize;
	int * freecombos = getfreecombos(sc,&combosize);
	int max = 0;
	int bestcat = 0;
	int counter = 0;

	for(int i = 0; i < combosize ; i++)
	{
		int tempresult = 0;
		int tempcounter = 0;
		for(int j = 0; j < numberofdice; j++)
		{
			if(diceresults[j] == freecombos[i]) 
			{
				tempresult+= freecombos[i];
				tempcounter++;
			}
		}

		for(int j = 0; j < storagesize; j++)
		{
			if(storage[j] == freecombos[i]) tempresult += freecombos[i];
		}

		if(tempresult > max) 
		{
			max = tempresult;
			bestcat = freecombos[i];
			counter = tempcounter;
		}
	}

	if(freecombos != NULL) free(freecombos);

	if(throw < MAXTHROWS)
	{
		free(diceresults);
		if(bestcat == 0)
		{
			return greedyplayer(sc,throw,numberofdice,storage,storagesize);
		}

		numberofdice -= counter;
		if(storagesize == 0)
		{
			storage = safeMalloc(sizeof(int) * counter);
			for(int i = 0; i < counter; i++)
			{
				storage[i] = bestcat;
			}
		}
		else
		{
			storage = safeRealloc(storage,sizeof(int) * (storagesize + counter));
			for(int i = storagesize ; i < (storagesize +counter) ; i++)
			{
				storage[i] = bestcat;
			}
		}
		storagesize+=counter;
		return greedyplayer(sc,throw,numberofdice,storage,storagesize);
	}
	else
	{
		if(bestcat != 0)
		{
			setcombo(sc,bestcat,max);
		}
		else
		{
			freecombos = getfreecombos(sc,&combosize);
			if(freecombos != NULL)
			{
				int score = numberofint(diceresults,numberofdice,freecombos[0]) * freecombos[0];
				score += numberofint(storage,storagesize,freecombos[0]) * freecombos[0];
				setcombo(sc,freecombos[0],score);
				free(freecombos);
			}
			else if(checkfivesame(sc) == true)
				setfivesame(sc,false);
			else if(checkchance(sc) == true)
				setchance(sc,sum(diceresults,numberofdice)+sum(storage,storagesize));
		}
		free(diceresults);
		if(storagesize >0) free(storage);
		return;
	}
}
