#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SHIP{
    int row, column, length;
    char direction;
} SHIP;

typedef struct ATTACK{
    int row, column;
} ATTACK;

// these are the prototype functions developed for our solution.  You are welcome to use them 
// or modify them if you have a different strategy for solving this project.
void Read_Save_Game_File(char *filename, int *rows, int *columns, int *ship_count, SHIP **ships, int *attack_count, ATTACK **attacks);
void Initialize_Board(int rows, int columns, char **board);
void Print_Board(int rows, int columns, char *board);
void Add_Ships(int rows, int columns, char *board, int ship_count, SHIP *ships);
void Process_Attacks(int rows, int columns, char *board, int attack_count, ATTACK *attacks);
void Game_Over(int rows, int columns, char *board);

int main(int argc, char *argv[] ) {

    // verify command line parameters
    if(argc != 2){
        printf("expected usage: %s <save_game_file>", argv[0]);
        exit(1);
    }

    // declare variables - feel free to add additional variables as needed
    char *board = NULL;
    SHIP *ships = NULL;
    ATTACK *attacks = NULL;
    int rows=0, columns=0, ship_count=0, attack_count=0;
    
    // read the save game file - doing this in main now
    //Read_Save_Game_File(argv[1], &rows, &columns, &ship_count, &ships, &attack_count, &attacks);
    
    printf("Reading Save Game File\n");
    FILE *input = fopen(argv[1], "r");
    if(!input){
        printf("INPUT File open failed\n");   
       exit(1);
    }
   
    // skip the board size line
    char skip_this_text[1000];
    if (!fgets(skip_this_text, 1000, input)) {
        printf("Error reading board size");
        exit(1);}
                                                                                   
    // read the board size
    fscanf(input, "%d", &rows);
    fscanf(input, "%d", &columns);
    printf("Board Size = (%d, %d)\n",rows, columns);
                                                                                   
    /*  Complete the rest of the function below - or move the code above to main */
    
    fgets(skip_this_text, 1000, input);
    fgets(skip_this_text, 1000, input);
    fscanf(input, "%d", &ship_count);
   
    printf("Number of Ships = %d\n", ship_count);
    
    //assign space for each ship struct
    ships = malloc(ship_count * sizeof(SHIP));
    
    //makes sure malloc worked properly
    if(ships == NULL){
    	fprintf(stderr, "Memory allocation failed.\n");
	exit(1);
    }

    fgets(skip_this_text, 1000, input);

    //for each ship, scan in their location/direction and assign the newly formed ship structs to their place in malloc memory
    for(int i = 0; i < ship_count; i++){
	fscanf(input, "%d", &(ships + i)->row);
        fscanf(input, "%d", &(ships + i)->column);
        fscanf(input, "%d", &(ships + i)->length);
	
        fscanf(input, " %c", &(ships + i)->direction);
        printf("Ship %d: (%d, %d), length = %d, direction %c\n", i, (ships + i)->row, (ships + i)->column, (ships + i)->length, (ships + i)->direction);
        //move to next line to get next ship data
	fgets(skip_this_text, 1000, input);
    }
                               
    fgets(skip_this_text, 1000, input);
    
    fscanf(input, "%d", &attack_count);
    printf("Number of Attacks = %d\n", attack_count);
   
    //dynamically allocate space for each attack struct
    attacks = malloc(attack_count * sizeof(ATTACK));
    fgets(skip_this_text, 1000, input);
    //assign attack data to attack struct
    for(int i = 0; i < attack_count; i++){
    	fscanf(input, "%d", &(attacks+ i)->row);
	fscanf(input, "%d", &(attacks + i)->column);
	printf("Attack %d: (%d, %d)\n", i, (attacks + i)->row, (attacks + i)->column);
	fgets(skip_this_text, 1000, input);
    }  
    printf("\n");
    fclose(input);
     
    /* Note: Reading the file and using malloc to reserve space for 
     * the data may be easier to do here, in main, rather than in a 
     * function */
     
    // Uncomment each function as you write and test it
    // Generate the board;
    Initialize_Board(rows, columns, &board);
    
    Print_Board(rows, columns, board);
     
    // Add Ships to the board;
    Add_Ships(rows, columns, board, ship_count, ships);
    Print_Board(rows, columns, board);
    
    // Process Attacks
    Process_Attacks(rows, columns, board, attack_count, attacks);
    Print_Board(rows, columns, board);
    
    Game_Over(rows, columns, board);
     
    //free memory
    free(board);
    free(ships);
    free(attacks);
    
     
    return 0;
}    

/*
 * Function initializes battleship game board with specified number of rows and columns. Each entry is set to . by default
 * @param rows - number of rows for the game board being intialized
 * @param columns - number of columns for the game board being initialized
 * @param **board - pointer to pointer of char battleship game board
 * @return void
 */ 
void Initialize_Board(int rows, int columns, char **board){
    printf("Initializing Board\n");

    //dynamically allocate space for char board
    *board = malloc(rows * columns * sizeof(char));

    //assign each element of board to be . by default
    for(int i = 0; i < rows; i++){
    	for(int j = 0; j < columns; j++){
		*(*board + i*columns + j) = '.';
	}
    }
}

/*
 * Function prints the game board at its current state - doesnt make any changes
 * @param rows - number of rows in game board passed in
 * @param cols - number of columns in game board passed in
 * @param *board - pointer to game board being printed
 * @return void
 */
void Print_Board(int rows, int columns, char *board){
    for(int i = 0; i < rows; i++){
    	for(int j = 0; j < columns; j++){
		printf("%c", *(board + i*columns + j));
	}
	printf("\n");
    }
    printf("\n");
}

/*
 * Function adds the ships to the game board IF they satisfy the game board conditions (fit on board, no overlaps,..)
 * @param rows - rows in game board
 * @param cols - cols in game board
 * @param *board - pointer to game board where ship will be places
 * @param ship_count - number of ships to be potentially added to game board if they satisfy the board conditions
 * @param *ships - pointer to ship structs that are potentially going to be added to the game board
 * @return void
 */ 
void Add_Ships(int rows, int columns, char *board, int ship_count, SHIP *ships){
    printf("Adding Ships\n");
    //checking if ships are valid
    for(int i = 0; i < ship_count; i++){
	//var check verifies if ship conditions are satisfied i.e ship stays on board
	int check = 0;
	
	if(check != 1){
	for(int k = 0; k < (ships+i)->length; k++){
	    //if ships go vertical direction, need to increment rows to see if ship stays inbounds	
	    if((ships + i)->direction == 'V'){
		//int tempCol = (ships + i)->column;
		//int tempRow = (ships + i)->row;
	        if(((ships + i)->row + k) >= rows || ((ships + i)->row + k) < 0){
	            printf("Ship %d is out of bounds - skipping\n", i);
		    check = 1;
		    break;    
		}

		if((ships + i)->column >= columns || (ships + i)-> column < 0){
		    printf("Ship %d is out of bounds - skipping\n", i);
		    check = 1;
		    break;
		}
		/*
		if(*(board + tempRow * columns + tempCol) == 'S'){
		    printf("Ship %d overlaps another ship - skipping\n", i);
		    check = 1;
		    break;
		}
		*/	
	    }
	   //if ships go horizontal in direction, increment columns to see if ship stays inbounds
	    else{
		//int tempCol = (ships + i)->column;
		//int tempRow = (ships + i)->row;
		if(((ships + i)->column + k) >= columns || ((ships + i)->column + k) < 0){
		    printf("Ship %d is out of bounds - skipping\n", i);
		    check = 1;
		    break;
		}
		if((ships + i)->row >= rows || (ships + i)->row < 0){
		    printf("Ship %d is out of bounds - skipping\n", i);
		    check = 1;
		    break;
		}
		/*
		if(*(board + tempRow * columns + tempCol) == 'S'){
		    printf("Ship %d is out of bounds - skipping\n", i);
		    check = 1;
		    break;
		}
		*/
            }
        }
	}
	
	//if there is an issue with the ship aka check = 1, don't places a ship on the board
	
	if(check != 1){
	    for(int j = 0; j < (ships+i)->length;j++){
	        int tempCol = (ships + i)->column;
	        int tempRow = (ships + i)->row;
	        if((ships + i)->direction == 'V'){
	            tempRow += j;
	        }
	        else{
	            tempCol += j;
	        }
	        //if there is already a ship there/overlap	
	        
		if(*(board + tempRow * columns + tempCol) == 'S'){
	            printf("Ship %d overlaps another ship - skipping\n", i);
		    check = 1;
		    break;
	        } 
		
		/*
	        if(check != 1){
	            *(board + tempRow*columns + tempCol) = 'S';
	        }
		*/
		
	    }
	}


	if(check != 1){
	    for(int m = 0; m < (ships + i)->length;m++){
	        int tempCol = (ships + i)->column;
		int tempRow = (ships + i)->row;
		if((ships + i)->direction == 'V'){
		    tempRow+=m;
		}
		else{
		    tempCol +=m;
		}
		*(board + tempRow*columns + tempCol) = 'S';
            }
	}
    }
}

/*
 * Function goes through game attacks and determines if they are in bound. If they are, it will update the game board to reflect the attack
 * @param rows - rows in game board
 * @param columns - columns in game board
 * @Param *board - pointer to game board
 * @param attack_count - number of attacks to be processed
 * @param *attacks - pointer to attack structs that will be processed
 * @return void
 */
void Process_Attacks(int rows, int columns, char *board, int attack_count, ATTACK *attacks){
    printf("Processing Attacks\n");
    //checking if attacks are in bounds
    for(int i = 0; i < attack_count; i++){
        if((attacks + i)->row >= rows || (attacks + i)->column >= columns || (attacks + i)->row < 0 || (attacks + i)->column < 0){
	    printf("Attack %d is out of bounds - skipping\n", i);
	    continue;
	}
	
	//tempRow and tempCol represent the current attacks corresponding row and column
	int tempRow = (attacks + i)->row;
	int tempCol = (attacks + i)->column;
        if(*(board + tempRow*columns + tempCol) == 'S'){
	    *(board + tempRow*columns + tempCol) = 'H';
	}
	else if(*(board + tempRow*columns + tempCol) == '.'){
	    *(board + tempRow*columns + tempCol) = 'M';
	}
	//if its already a hit, ignore it with no error message
	else{
	    //just skip it
	}
    }
}

/*
 * Function determined whether the game is over - aka all ships have been sunk
 * @param rows - rows in game board
 * @param cols - cols in game board
 * @param board - current version of game board
 * @return void
 */
void Game_Over(int rows, int columns, char *board){
    int gameOver = 1;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
	    if(*(board + i * columns + j) == 'S'){
	        gameOver = 0;
	    }
	}
    }
    if(gameOver == 1){
        printf("All ships have been sunk - game over\n");
    }
    else{
        printf("Ships remain afloat - game on\n");
    }
}
//// ////////////////////////////////////////
// a dd comments here
//// ////////////////////////////////////////
//

