#include "program.h"
#include "dirparse.h"
#include "utils.h"
#include "parse.h"
#include "ls.h"
#include "cd.h"
#include "rm.h"
#include "creat.h"
#include "size.h"
#include <stdio.h>
#include <string.h>

char* INPUT[5];

void run(char* entry){
	while(1){
		printf("%s> ", entry);
		getInput();

		if (strcmp(INPUT[0], "exit") == 0){
			break;
		}
		// info
		else if(strcmp(INPUT[0], "info") == 0){
			PrintBootSector();
		}
		// ls
		else if(){

		}
		// cd
		else if(){

		}
		// size
		else if(){

		}
		// creat
		else if(){

		}
		// mkdir
		else if(){

		}
		// open
		else if(){

		}
		// close
		else if(){

		}
		// rm
		else if(){

		}
		// rmdir
		else if(){

		}

}


}