/****************************
*
* This program expects one argument which is the name of a text file long listing (ls -l) of a directory tree.
*
*
*
*
* -rwxrwxrwx@ 1 karelchlebek  staff  823 24 Mar  2019 ./.config/Technical Info.plist
* -rw-r--r--@ 1 karelchlebek  staff  481  3 Jan  2016 ./.config/git/ignore
* -rw-r--r--@ 1 karelchlebek  staff  10244 25 Sep 21:44 ./Music/.DS_Store
* -rw-r--r--  1 karelchlebek  staff  0  5 Jan  2015 ./Music/.localized
* -rw-r--r--@ 1 karelchlebek  staff  6148 18 Jul  2018 ./Music/$RECYCLE.BIN/.DS_Store
* -rw-r--r--@ 1 karelchlebek  staff  129  6 Jan  2015 ./Music/$RECYCLE.BIN/.prl_rec/desktop.ini
* -rw-r--r--@ 1 karelchlebek  staff  16384  5 Sep 17:19 ./Music/iTunes/iTunes Library Extras.itdb
* -rw-r--r--@ 1 karelchlebek  staff  538543  5 Sep 20:45 ./Music/iTunes/iTunes Music Library.xml
* -rw-r--r--@ 1 karelchlebek  staff  6148 18 Jul  2018 ./Music/iTunes/.DS_Store
* 
****************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAXSTR 500 
#define MAXRETRIES 30
int main(int argc, char *argv[])
{

	FILE *ifp ;
	FILE *ftypes ; /* an external file which holds file types to be filtered, e.g. Video.txt. */
	FILE *ofp ;
	char ofname[25] ;

	char instring[MAXSTR] ;
	char outstring[MAXSTR] ;
	char *p, *p2 ;
	char delim = ',' ;

	char findExt[20][10] ;    /* holds extensions which are to be matched. Lines whose filenames have these extensions are included or omitted from the output */
	int n  , m , incl ;
	int x ;
	int n_ext = 0 ; /* number of extensions to be filtered */
	char fileExt[55] ;

	if( (argc != 2 ) && (argc != 3) ) {
		fprintf(stderr, "\nUsage: longListingToCSV inputfilename [delimiter] \n\n") ;
		fprintf(stderr, "The default delimiter ia a comma.\n") ;
		fprintf(stderr, "If using the pipe (|) as the delimiter remember to escape it thus: \\|  \n\n") ;

		return(0) ;
	}

	ifp = fopen(argv[1],"r+") ;

	if(ifp == NULL){
		fprintf(stderr,"\nCan't open %s for read/write.\n",argv[1]) ;
		return(1) ;
	}
	else {
		if( argc == 3)
			delim = argv[2][0] ;
		strcpy(ofname,argv[1]) ;
		p = strrchr(ofname,'.') ;
		p++ ;
		*p = 0 ;
		strcat(ofname,"csv") ;
		ofp = fopen(ofname,"w") ;
		if(!ofp){
			fprintf(stderr,"\nCan't open %s for write.\n",ofname) ;
			return(1) ;
		}
	}

		
/* return(0) ; */

	while(1){
		int n = 0 ;
		while(n < MAXSTR){
			instring[n] = 0 ;
			outstring[n] = 0 ;
			n++ ;
		}

		fgets(instring,MAXSTR-1,ifp);
		if(feof(ifp))
			break ;
		if(strlen(instring) < 10) /* skip blank lines */
			continue ;

	/*	insertDelimiter(instring,p) ; */

/* -rwxrwxrwx@ 1 karelchlebek  staff  823 24 Mar  2019 ./.config/Technical Info.plist */
		p = instring ;
		n = 0 ; 
		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = delim ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* permissions */

		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = delim ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* links */

		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = delim ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* owner */

		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = delim ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* group */

		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = delim ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* size */

/* -rwxrwxrwx@ 1 karelchlebek  staff  823 24 Mar  2019 ./.config/Technical Info.plist */
		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = ' ' ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* day */
		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = ' ' ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* month */
		while(!isspace(*p)){
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = delim ;
		n++ ;
		while(isspace(*p)){
			p++ ;
		}  /* year or time */
		while((*p)){
			if( *p == '\n')    /* don't copy the newline character - it will be put in later */
				break ;
			outstring[n] = *p ;
			p++ ;
			n++ ;
		}
		outstring[n] = 0 ; /* fname and path */

/* -rwxrwxrwx@ 1 karelchlebek  staff  823 24 Mar  2019 ./.config/Technical Info.plist */

/*	The complete output line has now been constructed. Now look for the rightmost path separator 
	and replace it with the delimiter to separate the path from the filename.
*/

		p = strrchr(outstring,'/') ;
		*p = delim ;




/*	Now we get the extension, if any, bearing in mind that if the first character of a filename in *nix files is a period (.)
	then it is a hidden file - do not repace this period!

	Filenames do not necessarily have an extension, and there may be more than one period in a filename.
*/

/*	Assume there is an extension */
		p++ ;  /* at first character of filename */
		p2 = strrchr(outstring,'.') ;
		if(p2 > p){
			*p2 = delim ;	
		}
		else{    /* there was no file extension, so we need to add the field to the output string */
			outstring[n] = delim ;
			n++ ;
			outstring[n] = 0 ;
		}	

		printf("%s\n",outstring) ;
		fprintf(ofp,"%s\n",outstring) ;

	}
	fclose(ifp) ;
	fclose(ofp) ;
}	
	
