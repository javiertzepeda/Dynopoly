/*
Author: Javier T Zepeda

Date: 10/19/2016
*/

#include <stdio.h>
#include <chrono>
#include <ctime>
#include "MonopolyLibrary.h"
#include <string>

/* definition of location codes */
#define GO 0
#define PROPERTY 1
#define JAIL 2
#define GOTOJAIL 3
#define FREEPARKING 4

/* defintion of Number of Locations and Maximum Number of Players, used for board creation */
#define NumLocations 26
#define PlayerMax 10

/* global variables */
int Die1 = 0;
int Die2 = 0;
int NumberPlayers;
int CurrentPlayer;

Player PlayerDB[PlayerMax]; //contains player data

Location LocationDB[NumLocations]; //contains location data

/* Adds the number of players to PlayerDB as specified by the integer input
	Variables changed - PlayerDB */
void SetupPlayers(int NumberPlayers) {
	int NumberPlayersCreated = 1;
	while (NumberPlayers > 0) {
		PlayerDB[NumberPlayersCreated - 1].SetPlayerNumber(NumberPlayersCreated);
		printf("Added player %d to the game. \n", NumberPlayersCreated);
		NumberPlayersCreated++;
		NumberPlayers--;
	}
}

/* Checks to see if atleast two players are still playing
	Variables changed - None */
bool WinnerIfTrue(void) {
	int NumberOfPlayersInGame = 0;
	for (int i = NumberPlayers; i > 0; i--) {
		if (PlayerDB[i - 1].PlayingStatus()) {
			NumberOfPlayersInGame++;
		}
	}
	if (NumberOfPlayersInGame > 1) {
		return false;
	}
    return true;
}

/* Calculates the next player
	Variables changed - CurrentPlayer */
void NextPlayer(void){
	CurrentPlayer++;
	if (CurrentPlayer == NumberPlayers) {
		CurrentPlayer = 0;
	}
}

/* Waits for keyboard input of user for a Yes or No question and returns response
	Variables changed - None */
bool AskUserQuestion() {
	char Answer = 'q';
	while (Answer != 'Y' && Answer != 'y' && Answer != 'N' && Answer != 'n') {
		scanf("%c", &Answer);
		if (Answer != 'Y' && Answer != 'y' && Answer != 'N' && Answer != 'n' && Answer != 'q') {
			printf("Please enter either Y/N.\n");
		}
	}
	if (Answer == 'Y' || Answer == 'y') {
		return true;
	}
	if (Answer == 'N' || Answer == 'n') {
		return false;
	}
    return false;
}

/* Calculates if CurrentPlayer has any properties that can be mortgaged or unmortgaged and asks player
	for if they would like to mortgage or unmortgage any properties.
	Possible variables changed - A properties' mortgage status, or a players' total money*/
void MortgageCheck(void){
	int UnMortgagedPropertiesCount = 0;
	int MortgagedPropertiesCount = 0;
	for (int i = 0; i < NumLocations; i++) {
		if (LocationDB[i].GetPlayerOwner() == CurrentPlayer && LocationDB[i].GetMortgageStatus() == false) {
			UnMortgagedPropertiesCount++;
		}
		if (LocationDB[i].GetPlayerOwner() == CurrentPlayer && LocationDB[i].GetMortgageStatus() == true) {
			MortgagedPropertiesCount++;
		}
	}
	if (UnMortgagedPropertiesCount > 0) {
		printf("You own %d unmortgaged location(s). Would you like to mortgage any of them? Current player has $%d cash.\n", UnMortgagedPropertiesCount, PlayerDB[CurrentPlayer].GetMoney());
		bool answer = AskUserQuestion();
		if (answer == true) {
			for (int i = 0; i < NumLocations; i++) {
				if (LocationDB[i].GetPlayerOwner() == CurrentPlayer && LocationDB[i].GetMortgageStatus() == false) {
					printf("Would you like to mortgage %s? Current player has $%d cash.\n", LocationDB[i].GetLocationName(), PlayerDB[CurrentPlayer].GetMoney());
					bool answer = AskUserQuestion();
					if (answer == true) {
						LocationDB[i].ChangeMortgageProperty();
						PlayerDB[CurrentPlayer].ChangeMoney(LocationDB[i].GetMortgageValue());
					}
				}
			}
		}
	}
	if (MortgagedPropertiesCount > 0) {
		printf("You own %d mortgaged location(s). Would you like to unmortgage any of them? Current player has $%d cash.\n", MortgagedPropertiesCount, PlayerDB[CurrentPlayer].GetMoney());
		bool answer = AskUserQuestion();
		if (answer == true) {
			for (int i = 0; i < NumLocations; i++) {
				if (LocationDB[i].GetPlayerOwner() == CurrentPlayer && LocationDB[i].GetMortgageStatus() == true) {
					int UnMortgageAmount = (LocationDB[i].GetMortgageValue() + (int)(LocationDB[i].GetMortgageValue() / 10));
					printf("Would you like to unmortgage %s for $%d? Current player has $%d cash.\n", LocationDB[i].GetLocationName(), UnMortgageAmount, PlayerDB[CurrentPlayer].GetMoney());
					bool answer = AskUserQuestion();
					if (answer == true) {
						if (PlayerDB[CurrentPlayer].GetMoney() < LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetPurchasePrice()) {
							printf("Player %d does not have enough money to unmortgage this property.\n", CurrentPlayer + 1);
							return;
						}
						LocationDB[i].ChangeMortgageProperty();
						PlayerDB[CurrentPlayer].ChangeMoney(-1 * UnMortgageAmount);
					}
				}
			}
		}
	}
}

/* Checks whether CurrentPlayer is in Jail and hasn't paid and checks if they rolled doubles or want to pay to get out
	Possible variables changed - Jail Property player owner, players' total money, Array JailDB values */
bool InJailCheck(void) {
	if (LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetLocationType() == JAIL && PlayerDB[CurrentPlayer].GetJailStatus()) {
		printf("Player %d is in jail.\n", CurrentPlayer + 1);
		if (Die1 == Die2) {
			printf("Since player %d has rolled doubles, they are now out of jail.\n", CurrentPlayer + 1);
			PlayerDB[CurrentPlayer].ChangeJailStatus();
			return true;
		}else if (PlayerDB[CurrentPlayer].GetMoney() > 50) {
			printf("Would Player %d like to pay $50 and get out of jail? Current player has $%d cash.\n", CurrentPlayer + 1,PlayerDB[CurrentPlayer].GetMoney());
			bool answer = AskUserQuestion();
			if (answer == true) {
				PlayerDB[CurrentPlayer].ChangeMoney(-50);
				printf("Player %d has paid the fine and is now out of jail.\n", CurrentPlayer + 1);
				PlayerDB[CurrentPlayer].ChangeJailStatus();
				return true;
			}
			else {
				printf("Player %d will not pay the fine and will remain in jail.\n", CurrentPlayer + 1);
				return true;
			}
		}
		else {
			printf("Player %d cannot pay the fine will remain in jail.\n", CurrentPlayer + 1);
			return true;
		}
	}
	else {
		return false;
	}
}

/* Checks current location of CurrentPlayer and prompts for actions if appropriate */
void LandingOnCheck() {
	if (LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetLocationType() == PROPERTY) {
		int CurrentLocationOwner = LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetPlayerOwner();
		if (CurrentLocationOwner == -1) {
			printf("No one owns this location. Would player %d like to purchase it for $%d? Current player has $%d cash.\n", CurrentPlayer + 1, LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetPurchasePrice(), PlayerDB[CurrentPlayer].GetMoney());
			MortgageCheck();
			bool answer = AskUserQuestion();
			if (answer == true) {
				if (PlayerDB[CurrentPlayer].GetMoney() < LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetPurchasePrice()) {
					printf("Player %d does not have enough money to purchase this property.", CurrentPlayer + 1);
					return;
				}
				LocationDB[PlayerDB[CurrentPlayer].GetLocation()].SetPlayerOwner(CurrentPlayer);
				PlayerDB[CurrentPlayer].ChangeMoney(-1 * LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetPurchasePrice());
				printf("Player %d now owns %s\n", CurrentPlayer+1, LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetLocationName());
				return;
			}
		}
		else if (CurrentLocationOwner == CurrentPlayer) {
			printf("Player %d currently owns this location, and checks on all the employees to make sure they are happy.\n", CurrentPlayer+1);
			return;
		}
		else if (LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetMortgageStatus()==false){
			int RentValue = LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetRentValue();
			printf("Player %d currently owns this location, and so player %d must pay the rent amount of $%d.\n", CurrentLocationOwner + 1, CurrentPlayer + 1, RentValue);
			PlayerDB[CurrentPlayer].ChangeMoney(-1*RentValue);
			PlayerDB[CurrentLocationOwner].ChangeMoney(RentValue);
			return;
		}
		else {
			printf("Player %d currently owns this location, but it is currently mortgaged so player %d does not have to pay rent.\n", CurrentLocationOwner + 1, CurrentPlayer + 1);
		}
	}
	else if (LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetLocationType() == GOTOJAIL) {
		for (int i = 0; i < NumLocations; i++) {
			if (LocationDB[i].GetLocationType() == JAIL) {
				PlayerDB[CurrentPlayer].ChangeJailStatus();
				PlayerDB[CurrentPlayer].SetLocation(i);
				printf("Player %d has been moved to jail.\n", CurrentPlayer + 1);
				return;
			}
		}
	}
}

int main(void) {
	/* prompts user for number of players in the game */
	printf("How many players? ");
	scanf("%d", &NumberPlayers);

	while (NumberPlayers <= 1 || NumberPlayers > PlayerMax) {
		printf("Please choose a number from 2 through 10. How many players? ");
		scanf("%d", &NumberPlayers);
	}
	/* populates PlayerDB with the number of players specified above */
	SetupPlayers(NumberPlayers);

	/* Sets up board */
	char Loc0[] = "Go";
	LocationDB[0] = Location(Loc0, GO, 0, 0, 0);
	char Loc1[] = "Mediterranean Ave.";
	LocationDB[1] = Location(Loc1, PROPERTY, 60, 50, 2);
	char Loc2[] = "Baltic Ave.";
	LocationDB[2] = Location(Loc2, PROPERTY, 60, 50, 4);
	char Loc3[] = "Oriental Ave.";
	LocationDB[3] = Location(Loc3, PROPERTY, 100, 50, 6);
	char Loc4[] = "Vermont Ave.";
	LocationDB[4] = Location(Loc4, PROPERTY, 100, 50, 6);
	char Loc5[] = "Connecticut Ave.";
	LocationDB[5] = Location(Loc5, PROPERTY, 120, 50, 8);
	char Loc6[] = "Jail";
	LocationDB[6] = Location(Loc6, JAIL, 0, 0, 0);
	char Loc7[] = "St. Charles Place";
	LocationDB[7] = Location(Loc7, PROPERTY, 140, 50, 10);
	char Loc8[] = "States Ave.";
	LocationDB[8] = Location(Loc8, PROPERTY, 140, 0, 10);
	char Loc9[] = "Virginia Ave.";
	LocationDB[9] = Location(Loc9, PROPERTY, 160, 50, 12);
	char Loc10[] = "St. James Place";
	LocationDB[10] = Location(Loc10, PROPERTY, 180, 50, 14);
	char Loc11[] = "Tennessee Ave.";
	LocationDB[11] = Location(Loc11, PROPERTY, 180, 50, 14);
	char Loc12[] = "New York Ave.";
	LocationDB[12] = Location(Loc12, PROPERTY, 200, 50, 16);
	char Loc13[] = "Free Parking";
	LocationDB[13] = Location(Loc13, FREEPARKING, 0, 0, 0);
	char Loc14[] = "Kentucky Ave.";
	LocationDB[14] = Location(Loc14, PROPERTY, 220, 50, 18);
	char Loc15[] = "Indiana Ave.";
	LocationDB[15] = Location(Loc15, PROPERTY, 220, 50, 18);
	char Loc16[] = "Illinois Ave.";
	LocationDB[16] = Location(Loc16, PROPERTY, 240, 50, 20);
	char Loc17[] = "Atlantic Ave.";
	LocationDB[17] = Location(Loc17, PROPERTY, 260, 50, 22);
	char Loc18[] = "Ventnor Ave.";
	LocationDB[18] = Location(Loc18, PROPERTY, 260, 50, 22);
	char Loc19[] = "Marvin Gardens";
	LocationDB[19] = Location(Loc19, PROPERTY, 280, 50, 24);
	char Loc20[] = "Go To Jail";
	LocationDB[20] = Location(Loc20, GOTOJAIL, 0, 0, 0);
	char Loc21[] = "Pacific Ave.";
	LocationDB[21] = Location(Loc21, PROPERTY, 300, 50, 26);
	char Loc22[] = "North Carolina Ave.";
	LocationDB[22] = Location(Loc22, PROPERTY, 300, 50, 26);
	char Loc23[] = "Pennsylvania Ave.";
	LocationDB[23] = Location(Loc23, PROPERTY, 320, 50, 28);
	char Loc24[] = "Park Place";
	LocationDB[24] = Location(Loc24, PROPERTY, 350, 50, 35);
	char Loc25[] = "Boardwalk";
	LocationDB[25] = Location(Loc25, PROPERTY, 400, 50, 50);

	/* Randomly choose a player to go first */
	srand(time(0));
	CurrentPlayer = rand() % NumberPlayers; 
	printf("Player %d has been randomly selected to go first.\n", CurrentPlayer+1);

	/* Infinitely loops game until only one player is left */
	while (WinnerIfTrue() == false) {
		Die1 = rand() % 6 + 1;
		Die2 = rand() % 6 + 1;
	//	_sleep(1000); /* Slight delay between each player's turn */
		printf("It is player %d's turn.\n", CurrentPlayer + 1);
		//MortgageCheck(); /* Uncomment to ask player if they would like to mortgage/unmortgage before they roll */
		printf("Player %d has rolled a %d and a %d.\n", CurrentPlayer + 1, Die1, Die2);
		if (InJailCheck() == false) { // Before moving player, checks to see if a Player is in jail and needs to pay a fine
			PlayerDB[CurrentPlayer].MoveLocation(Die1 + Die2); // Moves Player to new location, checks if they passed go (+$200)
			char* CurrentPlayerLocation = LocationDB[PlayerDB[CurrentPlayer].GetLocation()].GetLocationName(); // gets string data for the location that CurrentPlayer moved to 
			printf("Player %d moves %d spaces and lands on %s.\n", CurrentPlayer + 1, Die1 + Die2, CurrentPlayerLocation);
			LandingOnCheck(); // Processes location to see if user landed on jail, on another player's property, an unpurchased property, or a property that doesn't require action
		}
		
		MortgageCheck(); // If applicable, asks the user if they would like to mortgage or unmortgage properties

		NextPlayer(); // Calculates next player and restarts loop
	}
	
}


