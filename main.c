/**
*
 *
 * Author: Faris Abufarha
 * SEC:3
 * ID: 1200546
 *
 *
*/
//#include<bits/stdc++.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define null NULL
#define MAX_STRING 100
#define MAX_LINE 4095

typedef char String[MAX_STRING];


typedef struct Passenger {
    int passID;
    long passDate;
    String passengerDepTime;
    String passengerFromDestination;
    String passengerToDestination;

    struct Passenger *passengerNextPassenger;


} Passenger;

typedef struct Bus {

    int busID;
    //could be either string or int
    long date;
    String depTime;
    String fromDestination;
    String toDestination;
    double price;
    int maxCapacity;
    int numberOfPassengers;

    struct Passenger *busNextPassenger;


} Bus;

//node *makeEmpty(node *head);
//
//bool isEmpty(node *l);
//
//bool isLast(node *n, node *head);
//
//node *find(int x, node *head);
//
//node *findPrev(int x, node *l);
//
//void deleteNode(int x, node *head);
//
//void insertAtBeginning(int x, node *head);
//
//void insertAtNode(int x, node *head, node *p);
//
//void printList(node *head);
//
//void deleteList(node *head);
//
//int sizeOfList(node *head);


int countLines(String filename);         //CHECK THIS IF U GET AN ERROR

void memoryMsg();

void loadBusInfo(Bus *busArray);

void loadPassInfo(Bus *busArray, int busArraySize);

int busIndex(Bus *busArray, int sizeOfBusArray, Passenger passenger);

void printAllInfo(Bus *busArray, int busArraySize);

void addNewPassenger(Bus *busArray, int busArraySize);

void deletePassenger(Bus *busArray, int busArraySize);

void deleteBus(Bus *busArray, int busArraySize);

void printUnmatched(Bus *busArray, int busArraySize);

void printBus(Bus *busArray, int busArraySize);

//plan b make the array global


int main() {

    //initializing the bus array

    int sizeOfBusArray = countLines("busses.txt") + 1; // +1 FOR LATE PEOPLE
    Bus busArray[sizeOfBusArray];
    printf("================================================================\n");
    printf("WELCOME\n");
    char selection;

    // MENU

    while (true) {


        //I MADE POINT 2 AND 3 IN ONE OPTION, LOAD AND ASSIGN, 3 FOR PRINTING ONLY              $((ASK!!!))
        printf("================================================================\n");
        printf("PLEASE SELECT AN OPTION:\n"
               "1. Load the bus information file\n"
               "2. Load the tmpPassenger information file and assign each tmpPassenger to his appropriate bus \n"
               "3. print assignment information of all busses\n"
               "4. Print a specific bus information along with its passengers\n"
               "information (names and IDs)\n"
               "5. Print unmatched passengers\n"
               "6. Add new tmpPassenger\n"
               "7. Delete tmpPassenger\n"
               "8. Delete bus number\n"
               "9. Exit"
               "\n");

        scanf(" %c", &selection);

        printf("================================================================\n");

        //Q&A: CHANGE SELECTION TO CHAR
        switch (selection) {

            case '1':
                //load bus
                loadBusInfo(busArray);

                break;

            case '2':
                //load pass
                loadPassInfo(busArray, sizeOfBusArray);
                break;

            case '3':
                //prints all busses info ( not including unmatched passengers )
                printAllInfo(busArray, sizeOfBusArray);
                break;

            case '4':
                printBus(busArray, sizeOfBusArray);

                break;

            case '5':
                printUnmatched(busArray, sizeOfBusArray);
                break;

            case '6':

                addNewPassenger(busArray, sizeOfBusArray);

                break;
            case '7':
                deletePassenger(busArray, sizeOfBusArray);
                break;
            case '8':
                deleteBus(busArray, sizeOfBusArray);
                break;


            case '9':
                printf("THANK YOU, COME AGAIN :)\n");
                return 0;
            default:
                puts("PLEASE ENTER VALID INPUT");
                break;

        }


    }

}


void memoryMsg() {

    printf("OUT OF MEMORY!\n");
}

void loadBusInfo(Bus *busArray) {
    //taking them as strings then we will parse some of them into to ints or whatever

    String busID;
    String date;
    String depTime;
    String fromDestination;
    String toDestination;
    String price;
    String maxCapacity;

    FILE *pReadBus = null;
    pReadBus = fopen("busses.txt", "r");

    if (pReadBus == null) {
        printf("FILE NOT FOUND\n");
        exit(1);

    }

    char buffer[MAX_LINE];

    int i = 0;
    while (fgets(buffer, MAX_LINE, pReadBus)) {  // for busses in the file ( NOT INCLUDING UNMATCHED LIST)

        //printf("%s", buffer);

        strcpy(busID, strtok(buffer, "#"));
        strcpy(date, strtok(null, "#"));
        strcpy(depTime, strtok(null, "#"));
        strcpy(fromDestination, strtok(null, "#"));
        strcpy(toDestination, strtok(null, "#"));
        strcpy(price, strtok(null, "#"));
        strcpy(maxCapacity, strtok(null, "#"));

        // PARSING

        busArray[i].busID = atoi(busID);
        busArray[i].date = atol(date);

        strcpy(busArray[i].depTime, depTime);
        strcpy(busArray[i].fromDestination, fromDestination);
        strcpy(busArray[i].toDestination, toDestination);

        busArray[i].price = atof(price); // parsing double
        busArray[i].maxCapacity = atoi(maxCapacity);
        busArray[i].numberOfPassengers = 0;
        busArray[i].busNextPassenger = null;


        i++;
    }

    //to make the LATE PEOPLE list (UNMATCHED)
    busArray[i].busNextPassenger = null;

    printf("LOADED SUCCESSFULLY\n");
    fclose(pReadBus);
    //free busArray ??


}


void loadPassInfo(Bus *busArray, int busArraySize) {

    String passID;     //initially int
    String passDate;  //int
    String passengerDepTime;
    String passengerFromDestination;
    String passengerToDestination;

    Passenger *tmpPassenger = null;

    FILE *pReadPass = null;
    pReadPass = fopen("passengers.txt", "r");

    if (pReadPass == null) {

        printf("FILE NOT FOUND\n");
        exit(1);

    }

    char buffer[MAX_LINE];
//    int sizeOfBusArray = countLines("passengers.txt") + 1;
//    Bus *bussArray = malloc(sizeof(Bus) * sizeOfBusArray); // +1 for the late students

    int indx;
    while (fgets(buffer, MAX_LINE, pReadPass)) {

        strcpy(passID, strtok(buffer, "#"));
        strcpy(passDate, strtok(null, "#"));
        strcpy(passengerDepTime, strtok(null, "#"));
        strcpy(passengerFromDestination, strtok(null, "#"));
        strcpy(passengerToDestination, strtok(null, "#"));

        tmpPassenger = malloc(sizeof(Passenger));

        if (tmpPassenger == null) {

            memoryMsg();
            exit(1);
        }

        // assigning

        tmpPassenger->passID = atoi(passID); // converting string into int
        tmpPassenger->passDate = atol(passDate);
        strcpy(tmpPassenger->passengerDepTime, passengerDepTime);
        strcpy(tmpPassenger->passengerFromDestination, passengerFromDestination);
        strcpy(tmpPassenger->passengerToDestination, passengerToDestination);
        tmpPassenger->passengerNextPassenger = null;

//        printf("TESTING BUSARRAY IN LOADPASS: %s\n", busArray[2].fromDestination); PASS

        //  for checking the proper bus for each tmpPassenger
        indx = busIndex(busArray, busArraySize, *tmpPassenger);

        busArray[indx].numberOfPassengers++;  // incrementing the bus current number of passengers

        tmpPassenger->passengerNextPassenger = busArray[indx].busNextPassenger; //
        busArray[indx].busNextPassenger = tmpPassenger;


//        printf("indx: %d\n", indx); PASS


    }

    printf("PASSENGERS LOADED AND ASSIGNED SUCCESSFULLY\n");

    fclose(pReadPass);


}

int countLines(char *filename) {
    // count the number of lines in the file called filename
    FILE *fp = fopen(filename, "r");
    int ch = 0;
    int lines = 0;

    if (fp == NULL) {
        return 0;
    }

    lines++;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }
    fclose(fp);
    return lines;

}

//returns what the bus index should the tmpPassenger be in

int busIndex(Bus *busArray, int busArraySize, Passenger passenger) {

//    printf("%d:::%s\n", busArray[1].busID, busArray[1].fromDestination); PASS
//    printf("$$tmpPassenger %s\n",tmpPassenger.passengerToDestination);PASS

    for (int i = 0; i < busArraySize - 1; i++) { // -1 to not search in the -late passengers section-)

//        printf("!!!bus indx fun TEST: ID: %d from dest : %s\n", busArray[j].busID, (busArray[j].fromDestination));PASS

//        if (strcmp( busArray[j].fromDestination, tmpPassenger.passengerFromDestination) == 0) { PASS
//            return j;
//        }
        String tmpBusTime;
        strcpy(tmpBusTime, busArray[i].depTime);
        tmpBusTime[2] = '0';
        String tmpPassTime;
        strcpy(tmpPassTime, passenger.passengerDepTime);
        tmpPassTime[2] = '0';   // converting : to '0' for comparison reasons

//        printf("%d\n", atoi(tmpBusTime));     PASS
//        printf("%d\n",busArray[j].maxCapacity); PASS

        String flushed;
        strcpy(flushed, passenger.passengerToDestination);
        for (int j = 0; j < sizeof(flushed); ++j) {
            if (flushed[j] == '\n') {
                flushed[j] = '\0';
                break;
            }
        }
        //inorder to remove '\n' from the string

        if (busArray[i].date == passenger.passDate
            && atoi(tmpBusTime) >= atoi(tmpPassTime)
            && strcasecmp(busArray[i].fromDestination, passenger.passengerFromDestination) == 0
            && strcasecmp(busArray[i].toDestination, flushed) == 0
            && busArray[i].numberOfPassengers < busArray[i].maxCapacity
                ) {

            return i;
        }

        // in case none of busses is suitable

    }
    return (busArraySize - 1);//last index
}


void printAllInfo(Bus *busArray, int busArraySize) {

    //visiting all busses

    // last block is for the unmatched psngrs so '-1'

    for (int i = 0; i < busArraySize - 1; ++i) {

        if (busArray[i].busID == null){
            continue;
        }

        printf("BUS %d date: %ld departure time: %s from %s to %s ticket price %.2f capacity %d\n",
               busArray[i].busID, busArray[i].date, busArray[i].depTime, busArray[i].fromDestination,
               busArray[i].toDestination, busArray[i].price, busArray[i].maxCapacity
        );

        if (busArray[i].busNextPassenger == null) {

            printf("EMPTY BUS \n");
            continue;
        }

        // each tmpPassenger in each bus
        for (Passenger *iter = busArray[i].busNextPassenger; iter != NULL; iter = iter->passengerNextPassenger) {
            printf("Passenger: ID: %d date: %ld  time: %s from: %s to: %s\n", iter->passID, iter->passDate,
                   iter->passengerDepTime, iter->passengerFromDestination, iter->passengerToDestination
            );

        }

    }


}

//still, spaced string not fixed
void addNewPassenger(Bus *busArray, int busArraySize) {
    printf("INPUT NEW PASSENGER'S:"
           "ID, DEPARTURE DATE, TIME, AND FROM -TO- DESTINATION, RESPECTIVELY  "
           "\n");
    int passID;
    long passDate;
    String passengerDepTime;
    String passengerFromDestination;
    String passengerToDestination;
    //pointer

    Passenger *tmpPassenger = null;
    tmpPassenger = malloc(sizeof(Passenger));
    if (tmpPassenger == null) {

        memoryMsg();
        exit(1);
    }

    // string do not need '&'

    scanf("%d%ld%s%s%s", &passID, &passDate, passengerDepTime, passengerFromDestination, passengerToDestination);

    tmpPassenger->passID = (passID); // converting string into int
    tmpPassenger->passDate = (passDate);
    strcpy(tmpPassenger->passengerDepTime, passengerDepTime);
    strcpy(tmpPassenger->passengerFromDestination, passengerFromDestination);
    strcpy(tmpPassenger->passengerToDestination, passengerToDestination);
    tmpPassenger->passengerNextPassenger = null;

    int indx = busIndex(busArray, busArraySize, *tmpPassenger);

    busArray[indx].numberOfPassengers++;  // incrementing the bus current number of passengers
    tmpPassenger->passengerNextPassenger = busArray[indx].busNextPassenger; //
    busArray[indx].busNextPassenger = tmpPassenger;


}

void deletePassenger(Bus *busArray, int busArraySize) {

    printf("PLEASE INPUT PASSENGER ID YOU WANT TO DELETE \n");

    int passID;
    scanf("%d", &passID);

    bool isLast;
    for (int i = 0; i < busArraySize; ++i) {


        // each tmpPassenger in each bus
        for (Passenger *iter = busArray[i].busNextPassenger; iter != null; iter = iter->passengerNextPassenger) {

            isLast = iter->passengerNextPassenger == null;
            // in case the first pass was the one


            if (iter->passID == passID) {

                Passenger *tmpPass = null;
                tmpPass = iter;
                busArray[i].busNextPassenger = tmpPass->passengerNextPassenger;
                free(tmpPass);
                printf("PASSENGER DELETED SUCCESSFULLY\n");
                busArray[i].numberOfPassengers--;
                return;

            }


            if (iter->passengerNextPassenger != null && iter->passengerNextPassenger->passID == passID) {
                //like findPrevious
                Passenger *tmpPass = null;
                tmpPass = iter->passengerNextPassenger;
                iter->passengerNextPassenger = tmpPass->passengerNextPassenger;
                free(tmpPass);
                printf("PASSENGER DELETED SUCCESSFULLY\n");
                busArray[i].numberOfPassengers--;
                return;
            }

        }

    }

    printf("PASSENGER DOESN'T EXIST ! \n");


}

void deleteBus(Bus *busArray, int busArraySize) {
    printf("INPUT BUS ID YOU WANT TO DELETE \n");

    int ID;
    scanf("%d", &ID);


    int indx; //bus indx for re-assigning
    for (int i = 0; i < busArraySize - 1; ++i) {

        if (busArray[i].busID == ID) {

            Passenger *tmpPassenger = null;

            //frees all passengers in the bus
            for (Passenger *iter = busArray[i].busNextPassenger; iter != NULL;) {


                //indx = busIndex(busArray, busArraySize, *tmpPassenger);

                for (int j = 0; i < busArraySize - 1; j++) { // -1 to not search in the -late passengers section-)

                    String tmpBusTime;
                    strcpy(tmpBusTime, busArray[j].depTime);
                    tmpBusTime[2] = '0';
                    String tmpPassTime;
                    strcpy(tmpPassTime, tmpPassenger->passengerNextPassenger);
                    tmpPassTime[2] = '0';   // converting : to '0' for comparison reasons


                    String flushed;
                    strcpy(flushed, tmpPassenger->passengerToDestination);
                    for (int k = 0; j < sizeof(flushed); ++k) {
                        if (flushed[k] == '\n') {
                            flushed[k] = '\0';
                            break;
                        }
                    }
//inorder to remove '\n' from the string

                    if (busArray[j].date == tmpPassenger->passDate
                        && atoi(tmpBusTime) >= atoi(tmpPassTime)
                        && strcasecmp(busArray[j].fromDestination, tmpPassenger->passengerFromDestination) == 0
                        && strcasecmp(busArray[j].toDestination, flushed) == 0
                        && busArray[j].numberOfPassengers < busArray[j].maxCapacity
                        && j != i    ) {

                        indx =  j;
                        break;//breaks the reassigning loop
                    }
                    // in case none of busses is suitable

                }
                indx = (busArraySize - 1);//last index


                //
                busArray[indx].numberOfPassengers++;  // incrementing the bus current number of passengers
                tmpPassenger->passengerNextPassenger = busArray[indx].busNextPassenger; //
                busArray[indx].busNextPassenger = tmpPassenger;


                //freeing
                tmpPassenger = iter;
                iter = iter->passengerNextPassenger;
                tmpPassenger->passengerNextPassenger = null;


                free(tmpPassenger);

            }
            busArray[i].busNextPassenger = null;

            //HOW TO DELETE FROM ARRAY !?
            free(busArray[i].busID);
            free(busArray[i].date);
            free(busArray[i].depTime);
            free(busArray[i].fromDestination);
            free(busArray[i].toDestination);
            busArray[i].price = 0; // can't be  freed
            free(busArray[i].maxCapacity);
            free(busArray[i].numberOfPassengers);


            // or just keep it
            for (int j = i; j < busArraySize - 1; ++j) {
                busArray[j] = busArray[j + 1];
            }


        }


    }

    printf("BUS DOESN'T EXIST\n");


}


void printUnmatched(Bus *busArray, int busArraySize) {

    printf("PASSENGERS WITH NO BUSSES: \n");
    for (Passenger *iter = busArray[busArraySize - 1].busNextPassenger;
         iter != NULL; iter = iter->passengerNextPassenger) {
        printf("Passenger: ID: %d date: %ld time: %s from: %s to: %s\n", iter->passID, iter->passDate,
               iter->passengerDepTime, iter->passengerFromDestination, iter->passengerToDestination
        );

    }


}

void printBus(Bus *busArray, int busArraySize) {
    printf("INPUT BUS ID YOU'RE LOOKING FOR\n");
    int id;
    scanf("%d", &id);


    for (int i = 0; i < busArraySize - 1; ++i) {

        if (busArray[i].busID == id) {

            printf("BUS %d date: %ld departure time: %s from %s to %s ticket price %.2f capacity %d\n",
                   busArray[i].busID, busArray[i].date, busArray[i].depTime, busArray[i].fromDestination,
                   busArray[i].toDestination, busArray[i].price, busArray[i].maxCapacity
            );

            if (busArray[i].busNextPassenger == null) {

                printf("EMPTY BUS \n");
                return;

            }

            // each tmpPassenger in each bus
            for (Passenger *iter = busArray[i].busNextPassenger; iter != NULL; iter = iter->passengerNextPassenger) {
                printf("Passenger: ID: %d   time: %s \n", iter->passID,
                       iter->passengerDepTime
                );

            }
            return;

        }


    }
    printf("BUS DOESN'T EXIST\n");

}
