/*
Author: Javier T Zepeda

Date: 10/19/2016
*/
#include "MonopolyLibrary.h"
#include <stdio.h>


#define GoLocation 0
#define NumLocations 26 /* Number of locations on the board */
#define INITIALMONEYAMOUNT 1500 /* Initial amount for each player at the start of the game */

Player::Player() {
	this->PlayerNumber = 0;
	this->money = INITIALMONEYAMOUNT;
	this->location = GoLocation;
	this->isInJail = false;
	this->isPlaying = true;
}

void Player::SetPlayerNumber(int Number) {
	this->PlayerNumber = Number;
}

int Player::GetPlayerNumber(void) {
	return this->PlayerNumber;
}

void Player::MoveLocation(int LocationsToMove) {
	if ((this->location + LocationsToMove) > (NumLocations - 1)) {
		ChangeMoney(200);
		this->location = (this->location + LocationsToMove) % (NumLocations);
	}
	else {
		this->location = this->location + LocationsToMove;
	}
}

int Player::GetLocation(void) {
	return this->location;
}

void Player::SetLocation(int Position) {
	this->location = Position;
}

int Player::GetMoney(void) {
	return this->money;
}

void Player::ChangeMoney(int Amount) {
	this->money = this->money + Amount;
	if (this->money < 0) {
		printf("Player %d is out of the game!", this->GetPlayerNumber());
		this->isPlaying = false;
	}
}
bool Player::PlayingStatus(void) {
	return this->isPlaying;
}
bool Player::GetJailStatus(void) {
	return this->isInJail;
}
void Player::ChangeJailStatus(void) {
	if (this->isInJail == true) {
		this->isInJail = false;
	}
	else {
		this->isInJail = true;
	}
}

Location::Location(void) {
	this->LocationName = 0;
	this->LocationType = 0;
	this->PurchasePrice = 0;
	this->MortgageValue = 0;
	isMortgaged = false;
	this->RentValue = 0;
	this->PlayerOwner = -1;
}

Location::Location(char* LocationName, int LocationType, int PurchasePrice, int MortgageValue, int RentValue) {
	this->LocationName = LocationName;
	this->LocationType = LocationType;
	this->PurchasePrice = PurchasePrice;
	this->MortgageValue = MortgageValue;
	isMortgaged = false;
	this->RentValue = RentValue;
	this->PlayerOwner = -1;
}


char* Location::GetLocationName(void) {
	return this->LocationName;
}

int Location::GetLocationType(void) {
	return this->LocationType;
}

int Location::GetPurchasePrice(void) {
	return this->PurchasePrice;
}

int Location::GetMortgageValue(void) {
	return this->MortgageValue;
}

int Location::GetRentValue(void) {
	return this->RentValue;
}

int Location::GetPlayerOwner(void) {
	return this->PlayerOwner;
}

void Location::SetPlayerOwner(int Player) {
	this->PlayerOwner = Player;
}

bool Location::GetMortgageStatus(void) {
	return this->isMortgaged;
}

void Location::ChangeMortgageProperty(void) {
	if (this->isMortgaged) {
		this->isMortgaged = false;
	}
	else {
		this->isMortgaged = true;
	}
}

