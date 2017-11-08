/*
Author: Javier T Zepeda

Date: 10/19/2016
*/

#ifndef _MonopolyLibrary_h
#define _MonopolyLibrary_h

class Player {
public:
	Player(void); /* sets up player with INITIALMONEYAMOUNT amount of initial money (default of 1500) */
	void SetPlayerNumber(int); /* sets the player number to a specific integer */
	int GetPlayerNumber(void); /* returns the player of number */
	void MoveLocation(int); /* moves player by a integer value and gives $200 to player if they pass go */
	int GetLocation(void); /* returns player location */
	void SetLocation(int); /* sets the player's location */
	int GetMoney(void);	/* returns the player's money amount */
	void ChangeMoney(int); /* changes the player's money amount. Use negative integer values for subtraction. Can change PlayerStatus if the player runs out of money */
	bool PlayingStatus(void); /* returns true if the player is still playing in the game */
	bool GetJailStatus(void); /* returns true if the player is in jail and hasn't paid */
	void ChangeJailStatus(void); /* flips status of Jail boolean from true to false or false to true */

private:
	int PlayerNumber; /* holds the players unique number for identification */
	int money; /* holds the players total money amount */
	int location; /* holds the players current location */
	bool isInJail; /* flag that specifies whether the player is in jail and hasn't paid the fine yet */
	bool isPlaying; /* if a flag that is true if the player is still playing */
};



class Location {
public:
	 Location(); /* sets location data to null */
	 Location(char* LocationName, int LocationType, int PurchasePrice, int MortgageValue, int RentValue); /* sets location data with specific input */

	char* GetLocationName(void); /* returns a string pointer to the name of the location */
	int GetLocationType(void); /* returns a number representing the type of location */
	int GetPurchasePrice(void); /* returns the price of purchase for the location */
	int GetMortgageValue(void); /* returns the amount of cash given the player if the location is mortgaged */
	int GetRentValue(void); /* returns the rent value when another player other than the owner lands on it */
	int GetPlayerOwner(void); /* returns the owner of the location using their unique PlayerNumber */
	void SetPlayerOwner(int); /* sets the owner of the location */
	bool GetMortgageStatus(void); /* returns true if the the location has been mortgaged */
	void ChangeMortgageProperty(void); /* flips status of Mortgage boolean from true to false or false to true */


private:
	char* LocationName; /* holds a pointer to the strings name */
	int LocationType; /* holds a number representing the type of location */
	int PurchasePrice; /* holds an amount of the price of purchase for the location */
	int MortgageValue; /* holds an amount of cash given to the player when mortgaged */
	bool isMortgaged; /* is true if the the location has been mortgaged */
	int RentValue; /* holds the amount of rent */
	int PlayerOwner; /* holds the unique PlayerNumber of the owner of the location */
};



#endif /* _Player_h */