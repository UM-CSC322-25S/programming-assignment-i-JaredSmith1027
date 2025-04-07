#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//defining sizes
#define STRING_LENGTH 250
#define BOAT_MAX 120
#define NAME_LENGTH 127

typedef char String[STRING_LENGTH];



//PlaceType enum 
typedef enum {
    slip,
    land,
    trailor,
    storage,
    no_place
}PlaceType;



//Extra Union for PlaceType
typedef union{
    int SlipNum;
    char LandLetter;
    String TrailorTag;
    int StorageSpace;
}ExtraInfo;



//Boat structure
typedef struct {
    char name[NAME_LENGTH];
    int Length;
    PlaceType type;
    double MoneyOwe;
    ExtraInfo Extra;
}Boat;
typedef Boat* BoatPointer;
typedef BoatPointer listOfBoats[BOAT_MAX];



//This function converts a string to a PlaceType
PlaceType StringToPlaceType(char* PlaceTypeString) {
    if (!strcasecmp(PlaceTypeString,"slip")) {
        return(slip);
    }
    if (!strcasecmp(PlaceTypeString,"land")) {
        return(land);
    }
    if (!strcasecmp(PlaceTypeString,"trailor")) {
        return(trailor);
    }
    if (!strcasecmp(PlaceTypeString,"storage")) {
        return(storage);
    }
    return(no_place);
}



//This function converts a string to a PlaceType
char* PlaceToString(PlaceType Place) {
    switch (Place) {
        case slip:
            return("slip");
        case land:
            return("land");
        case trailor:
            return("trailor");
        case storage:
            return("storage");
        case no_place:
            return("no_place");
        default:
            printf("Error: Invalid Place\n");
            exit(EXIT_FAILURE);
            break;
    }
}



// This function comapares two boats and returns them
int CompareBoats(const void* a, const void* b) {
    BoatPointer boatA = *(BoatPointer*)a;
    BoatPointer boatB = *(BoatPointer*)b;
    return strcmp(boatA->name, boatB->name);
}
// This function sorts the boats alphabtically
void SortBoats(listOfBoats boats, int numBoats) {
    qsort(boats, numBoats, sizeof(BoatPointer), CompareBoats);
}



// This function reads a csv file and gets the number of boats
int ReadFromFile(FILE* file, listOfBoats boats) {
    int numBoats = 0;
    String line;
    while (fgets(line, STRING_LENGTH, file) != NULL && numBoats <= BOAT_MAX) {


        //allocate memory
        boats[numBoats] = malloc(sizeof(Boat));


        // Check if malloc failed
        if (boats[numBoats] == NULL) {
            printf("Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        //Using strtok() to split and tokenize the csv lines from input file
        char* token;
        char* tokens[5];
        int tokenIndex = 0;
        token = strtok(line, ",");
        while (token != NULL && tokenIndex < 5) {
            tokens[tokenIndex++] = token;
            token = strtok(NULL, ",");
        }

        // Populate boat data from tokens
        strcpy(boats[numBoats]->name, tokens[0]);
        boats[numBoats]->Length = atoi(tokens[1]);
        boats[numBoats]->type = StringToPlaceType(tokens[2]);
        switch (boats[numBoats]->type) {
            case slip:
                boats[numBoats]->Extra.SlipNum = atoi(tokens[3]);
                break;
            case land:
                boats[numBoats]->Extra.LandLetter = tokens[3][0];
                break;
            case trailor:
                strcpy(boats[numBoats]->Extra.TrailorTag, tokens[3]);
                break;
            case storage:
                boats[numBoats]->Extra.StorageSpace = atoi(tokens[3]);
                break;
            default:
                printf("Error: Invalid Place Type\n");
                exit(EXIT_FAILURE);
        }
        boats[numBoats]->MoneyOwe = atof(tokens[4]);
        
        //increase the number of boats
        numBoats++;
    }
    SortBoats(boats, numBoats); // Sort boats alphabetically
    return numBoats;
}



// This funciton prints the boat inventory
void PrintInventory(listOfBoats boats, int numBoats) {
    for (int i = 0; i < numBoats; i++) {
        printf("%-20s %3d' %7s ", boats[i]->name, boats[i]->Length, PlaceToString(boats[i]->type));
        switch (boats[i]->type) {
            case slip:
                printf("  #%3d", boats[i]->Extra.SlipNum);
                break;
            case land:
                printf("%6c", boats[i]->Extra.LandLetter);
                break;
            case trailor:
                printf("%6s", boats[i]->Extra.TrailorTag);
                break;
            case storage:
                printf("  #%3d", boats[i]->Extra.StorageSpace);
                break;
            default:
                printf("Error: Invalid Place Type\n");
                break;
        }
        printf("   Owes $%7.2lf\n", boats[i]->MoneyOwe);
    }
}



// This function adds a new boat
void AddBoat(listOfBoats boats, int* numBoats) {


    //check if the number of boats exceeded the limit
    if (*numBoats >= BOAT_MAX) {
        printf("Error: Maximum number of boats reached.\n");
        return;
    }

    boats[*numBoats] = malloc(sizeof(Boat));
    

    // Check if malloc failed
    if (boats[*numBoats] == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    //getting user input for new boat data
    printf("Please enter the boat data in CSV format                 : ");
    String input;
    scanf(" %[^\n]s", input);;

    // Parse input string and populate boat data
    char* token;
    char* tokens[5]; 
    int tokenIndex = 0;
    token = strtok(input, ",");
    while (token != NULL && tokenIndex < 5) {
        tokens[tokenIndex++] = token;
        token = strtok(NULL, ",");
    }

    // Populate boat data from tokens
    strcpy(boats[*numBoats]->name, tokens[0]);
    boats[*numBoats]->Length = atoi(tokens[1]);
    boats[*numBoats]->type = StringToPlaceType(tokens[2]);
    switch (boats[*numBoats]->type) {
        case slip:
            boats[*numBoats]->Extra.SlipNum = atoi(tokens[3]);
            break;
        case land:
            boats[*numBoats]->Extra.LandLetter = tokens[3][0];
            break;
        case trailor:
            strcpy(boats[*numBoats]->Extra.TrailorTag, tokens[3]);
            break;
        case storage:
            boats[*numBoats]->Extra.StorageSpace = atoi(tokens[3]);
            break;
        default:
            printf("Error: Invalid Place Type\n");
            break;
    }
    boats[*numBoats]->MoneyOwe = atof(tokens[4]);

    (*numBoats)++; // adds a new boat pointer to the array
    SortBoats(boats, *numBoats); // Sort boats alphabetically
}



// This function removes boats
void RemoveBoat(listOfBoats boats, int* numBoats) {
    //Getting user boat input
    printf("Please enter the boat name                               : ");
    String input;
    scanf(" %[^\n]s", input);

    // Convert input to lowercase
    for (int i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }

    // Search for boat with given name
    int found = 0;
    for (int i = 0; i < *numBoats; i++) {
        char boatNameLower[NAME_LENGTH];
        strcpy(boatNameLower, boats[i]->name);
        for (int j = 0; boatNameLower[j]; j++) {
            boatNameLower[j] = tolower(boatNameLower[j]);
        }
        if (strcmp(boatNameLower, input) == 0) {
            // Free memory for the boat being removed
            free(boats[i]);
            // Shift remaining boats to fill the gap
            for (int k = i; k < *numBoats - 1; k++) {
                boats[k] = boats[k + 1];
            }
            (*numBoats)--; //decrease boat pointer
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("No boat with that name\n");
    }
}



// This function is for the boat payment
void AcceptPayment(listOfBoats boats, int numBoats) {
    printf("Please enter the boat name                               : ");
    String input;
    scanf(" %[^\n]s", input);
    // Convert input to lowercase
    for (int i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }
    // Search for boat with given name
    int found = 0;
    for (int i = 0; i < numBoats; i++) {
        char boatNameLower[NAME_LENGTH];
        strcpy(boatNameLower, boats[i]->name);
        for (int j = 0; boatNameLower[j]; j++) {
            boatNameLower[j] = tolower(boatNameLower[j]);
        }
        if (strcmp(boatNameLower, input) == 0) {
            //getting user input for boat payment
            printf("Please enter the amount to be paid                       : ");
            double payment;
            scanf("%lf", &payment);
            //check if paymet is more than owed
            if (payment <= boats[i]->MoneyOwe) {
                boats[i]->MoneyOwe -= payment;
            } else {
                printf("That is more than the amount owed, $%.2lf\n", boats[i]->MoneyOwe);
            }
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("No boat with that name\n");
    }
}



// This function updates the amount of money owed
void UpdateOwedMoney(listOfBoats boats, int numBoats) {
    for (int i = 0; i < numBoats; i++) {
        switch (boats[i]->type) {
            case slip:
                boats[i]->MoneyOwe += 12.50 * boats[i]->Length;
                break;
            case land:
                boats[i]->MoneyOwe += 14.00 * boats[i]->Length;
                break;
            case trailor:
                boats[i]->MoneyOwe += 25.00 * boats[i]->Length;
                break;
            case storage:
                boats[i]->MoneyOwe += 11.20 * boats[i]->Length;
                break;
            default:
                printf("Error: Invalid Place Type\n");
                break;
        }
    }
}



// This function saves the csv file
void SaveToFile(char* filename, listOfBoats boats, int numBoats) {
    FILE* file = fopen(filename, "w");
    //check if file is openable
    if (file == NULL) {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }
    //printing to file
    for (int i = 0; i < numBoats; i++) {
        fprintf(file, "%s,%d,%s,", boats[i]->name, boats[i]->Length, PlaceToString(boats[i]->type));
        switch (boats[i]->type) {
            case slip:
                fprintf(file, "%d", boats[i]->Extra.SlipNum);
                break;
            case land:
                fprintf(file, "%c", boats[i]->Extra.LandLetter);
                break;
            case trailor:
                fprintf(file, "%s", boats[i]->Extra.TrailorTag);
                break;
            case storage:
                fprintf(file, "%d", boats[i]->Extra.StorageSpace);
                break;
            default:
                printf("Error: Invalid Place Type\n");
                break;
        }
        fprintf(file, ",%.2lf\n", boats[i]->MoneyOwe);
        free(boats[i]);
    }
    //checking if file can close
    if(fclose(file) == EOF){
        printf("Error Closing File");
    }
}



int main(int argc, char** argv){
    //checking number of arguments in command line
    if(argc != 2){
        printf("Error: Not 1 input file entered\n");
        printf("Usage: <executabe file> <csv file>\n");
        return(1);
    }
    //check if file is openable
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to the Boat Management System\n-------------------------------------\n");


    //reading the file
    listOfBoats boats;
    int numBoats = ReadFromFile(file, boats);

    
    //displaying Input Menu
    while(1){        
        printf("\n(I)nventory, (A)dd, (R)emove, (P)ayment, (M)onth, e(X)it : ");
        String input;
        scanf("%s", input);
        
        if(strcasecmp(input, "I") == 0){
            PrintInventory(boats, numBoats);
        }
        else if (strcasecmp(input, "A") == 0){
            AddBoat(boats, &numBoats);
        }
        else if (strcasecmp(input, "R") == 0){
            RemoveBoat(boats, &numBoats);
        }
        else if (strcasecmp(input, "P") == 0){
            AcceptPayment(boats, numBoats);
        }
        else if (strcasecmp(input, "M") == 0){
            UpdateOwedMoney(boats, numBoats);
        }
        else if (strcasecmp(input, "X") == 0){
            //saving to csv file and exit message
            SaveToFile(argv[1], boats, numBoats);
            printf("\nExiting the Boat Management System\n");
            break;
        }
        else{
            printf("Invalid option %s\n", input);
        }
    }
    return(0);
}