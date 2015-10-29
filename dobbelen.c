/*
 * =====================================================================================
 *
 *       Filename:  dobbelen.c
 *
 *    Description:  Main source file
 *
 *        Version:  1.0
 *        Created:  12/23/2009 11:39:57 PM
 *       Compiler:  gcc
 *
 *         Author:  Adriaan Larmuseau
 *
 * =====================================================================================
 */

/** c lib **/
#include <time.h>


#include <unistd.h>
#include <limits.h>
#include <math.h>

/** personal **/


#include "player.h"

/** Global defined Constants*/
enum
{
	TESTN = 1000000,
	TIMESTR_SIZE = 34,
	DICEN = 5,
	NUMBEROFTHROWS = 3,
};


/*-----------------------------------------------------------------------------
 *  Function declarations -> keeps the compiler happy
 *-----------------------------------------------------------------------------*/
static void randomize(unsigned int seed);
static void usage(void);
static char * get_time_str(void);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  randomize
 *  Description:  initialize the random seed
 * =====================================================================================
 */
static void randomize(unsigned int seed)
{ 
	srand(seed); 
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  usage
 *  Description:  print usage
 * =====================================================================================
 */
static void usage(void)
{
	fprintf(stderr,"dobbelen -c [number] -f [file] -d [number of dice]"
				   "-g [fixed goal] -i [seed] -r [fixeddice] -s [strategy 1/2/3] -t [throws]");
	exit(1);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_time_str
 *  Description:  return a string with the current date and time
 * =====================================================================================
 */
static  char * get_time_str(void)
{
	time_t current_time;
	time(&current_time);
	struct tm * time_info = localtime(&current_time);
	char * timestr = (char*) safeMalloc(TIMESTR_SIZE); 
	strftime(timestr,TIMESTR_SIZE,"LOG::%a %b %d %Y - %I:%M:%S",time_info); 
	return timestr;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main function
 * =====================================================================================
 */
int main(int argc , char ** argv)
{

	int ch;
	int numberoftests = TESTN;
	int numberofdice = DICEN;
	FILE * filepointer = NULL;
	void (*player_strategy) (struct scorecard_t * sc,int throw,int ndice,int * a,int b) = collectingplayer;
	int seed = (unsigned) time(NULL);

	/* iterate over commandline parameters 
	 * call usage when parameters are unusable
	 */
	while((ch = getopt(argc,argv,"c:f:d:s:r:g:i:t:")) != -1)
	{
		switch(ch)
		{
			case 'c':
				numberoftests = atoi(optarg);
				break;

			case 'd':
				numberofdice = atoi(optarg);
				setchancegoal((int) ((6*numberofdice) * 0.7)); 
				break;

			case 'f':
				safeFileOpen(filepointer,optarg,"wb");	
				break;

			case 'g':
				setchancegoal(atoi(optarg));
				break;
			
			case 'i':
				seed = atoi(optarg);
				break;

			case 'r':
			{
				int temp = atoi(optarg);
				setrollfunction(temp/10,temp%10);
			}
				break;

			case 's':
				switch(atoi(optarg))
				{
					case 1 :
						player_strategy = collectingplayer;
						break;

					case 2 :
						player_strategy = categorybycategory;
						break;
					case 3 :
						player_strategy = greedyplayer;
						break;

					default :
						printf("*1 is collectingplayer\n*2 is categorybycategory\n*3 is greedyplayer\n");
						break;
				}
				break;

			case 't':
				setnumberofthrows(atoi(optarg));
				break;

			case ':':     
                fprintf(stderr,"Option -%c requires an operand\n", optopt);
				break;

			case '?':
			default :
				usage();
		}
	}

	/** set seed */
	randomize(seed);

	if(filepointer == NULL) 
	{
		char * fname = get_time_str();
		safeFileOpen(filepointer,fname,"wb");
		free(fname);
	}


	/** variables to keep statistics */
	int total = 0;
	int numberofsame = 0;
	int totalchancescore = 0;
	int totalcomboresults = 0;
	int numberofcombobonus = 0;
	int tempscore;
	int maxtotal = 0;
	int mintotal = INT_MAX;
	int tempcalcscore = 0;

	int MAXSCORE = (21 * numberofdice) + (35 +50) + (6 * numberofdice); 

	int * distribution = safeMalloc((MAXSCORE+1) * sizeof(int));
	for(int i = 0; i <= MAXSCORE; i++) distribution[i] = 0;

	/*-----------------------------------------------------------------------------
	 *  play the game
	 *-----------------------------------------------------------------------------*/
	struct scorecard_t * my_card = (struct scorecard_t *) safeMalloc(sizeof(struct scorecard_t));	
	for(int testi = 0; testi < numberoftests ; testi++)
	{		
		clearStruct(my_card,struct scorecard_t);
		while(!isdone(my_card))
		{
			int storage = 0;
			player_strategy(my_card,0,numberofdice,&storage,0);	
		}
		
	   /*-----------------------------------------------------------------------------
		*  parse the results
		*-----------------------------------------------------------------------------*/
		tempcalcscore = calculatescore(my_card,&tempscore);
		distribution[tempcalcscore] += 1;
		total += tempcalcscore;

		/** MIN / MAX */	
		if(tempcalcscore > maxtotal) maxtotal = tempcalcscore;
		if(tempcalcscore < mintotal) mintotal = tempcalcscore;

		totalcomboresults += tempscore;
		if(tempscore >= 63) numberofcombobonus++;
		if(my_card->fivesame == 50) numberofsame++;
		totalchancescore+= my_card->chance;
	}

	/** calculate statistics */
	double average = (double)(total) / (double)(numberoftests);
	double percentagefivesame = ((double)(numberofsame) / (double)(numberoftests)) * 100;
	double averagechancenozero = ((double)(totalchancescore) / (double)(numberoftests));
	double averagecomboscore = ((double)(totalcomboresults)/(double)(numberoftests));
	double percentagecombobonus = ((double)(numberofcombobonus)/(double)(numberoftests)) * 100; 

	long double devtemp = 0;
	
	/** calculate deviation */
	/** write out distribution to file this is slow*/
	for(int i = 1 ; i <= MAXSCORE ; i++)
	{
		fprintf(filepointer,"%d , %d\n",i,distribution[i]);
		devtemp += ((average - i) * (average - i)) * distribution[i];
	}

	double deviation = sqrt((devtemp) / (numberoftests -1));
		

	printf("Average = %lf \n"
			"Deviation = %lf \n"
			"Maximum = %d \n"
			"Minimum = %d \n"
			"Average Combo Result = %lf\n"
			"Percentage of Combo bonusses = %lf%%\n"
			"Percentage five same = %lf%%\n"
			"Average chance score  = %lf\n",average,deviation,maxtotal,mintotal,averagecomboscore,
			percentagecombobonus,percentagefivesame,averagechancenozero);


	free(my_card);
	free(distribution);
	safeFileClose(filepointer);

	return 0;
}

