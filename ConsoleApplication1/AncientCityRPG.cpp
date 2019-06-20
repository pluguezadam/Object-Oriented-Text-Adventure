// AncientCityRPG.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <vector>
#include <iostream>
#include <time.h>
#include <string>
#include "objects.h"
#include "monsters.h"
#include "classes.h"
#include "char.h"
#include "shop.h"
#include "spells.h"

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::string;

enum {
	mapHeight = 30,
	mapWidth = 30
};

//Functions
void playerAttack(player* playerOne, Monster* monster);
void monsterAttacks(player* playerOne, Monster* monster);
void characterCreation(player* playerOne);
void shop(player* playerOne);

//Intialize Map
vector<vector<int>> map;

int main() {

	//build random Map? Currently all 0's
	int input = 0;
	for (int i = 0; i < mapHeight; i++) {
		vector<int> inputVec;
		for (int j = 0; j < mapWidth; j++) {
			//cin >> input;
			inputVec.push_back(input);
		}
		map.push_back(inputVec);
	}
	while (true) {

		// Intialize srand seed for random number generation
		srand(unsigned int(time(NULL)));

		cout << "\n";
		cout << "\n";
		bool gameRunning = true;
		while (gameRunning) {
			cout << "Welcome to the Ancient City Text Adventure!?\n";
			cout << "Please enter your character name:\n";
			//Set name/create player object
			string playerName;
			cin >> playerName;
			player playerOne(playerName);

			//Creates char and sets stats
			characterCreation(&playerOne);
			
			//TODO Fix or Add to Intro?
			cout << "Growing up in a small village outside of the great forest you have heard tales of the ancient city and its treasures lost within the forest all your life.\n"
				"Upon reaching adulthood you collected your things and headed into the forest to find adventure,\n fortune and the ancient city!\n"
				"As you leave your village you enter a small clearing with a merchant's caravan off the path.\n This is a good chance to buy supplies before heading out!\n";

			//Shop
			shop(&playerOne);
			
			//Display char stats
			playerOne.display();

			//Set starting position 30 x 30 map
			playerOne.setPositionX(15);
			playerOne.setPositionY(1);

			//TODO:First directional choice, cant go south; reuseable for any path is blocked etc.....
			//TODO:Track each movement?	
			//TODO:Main Play Menu System
			/*cout << "You attempt to take the path going south, "
						"but you quickly realize this path leads back home and the call of adventure forces you to turn around.\n"
						"Choose another path.\n";
			*/
			input = -1;
			//mainmenuFunction--Movement function/also track position?
			//Main Menu --Movement menu --Equipment menu --Inventory/Use item(Potion,Rope,Lantern,etc)
			//mainMenu();
			while (input < 1 || input > 3) {
				cout << "Choose an action: 1-Travel Onward, 2-Check surroundings, 3-Inventory, 4-Equipment\n";
				cin >> input;
				switch (input) {
				case 1:
					//moveMenu();
					cout << "You continue your journey.\n";
					input = -1;
					cout << "Choose a direction: 1-Path to the north, 2-Path to the south, 3-Path to the east, 4-Path to the west\n";
					cin >> input;

					while (input < 1 || input > 3) {
						if (input == 1) {
							cout << "You take the path going north.\n";
						}
						else if (input == 2) {
							cout << "You take the path going south.\n";
						}
						else if (input == 3) {
							cout << "You take the path going east.\n";
						}
						else if (input == 4) {
							cout << "You take the path going west.\n";
						}
					}
					break;
				case 2:
					//TODO:Examine your surroundings
					cout << "You examine your surroundings....\n";
					break;
				case 3:
					playerOne.displayInventory();
					break;
				case 4:
					//TODO:Equipment system / menu
					cout << "Equipment Menu\n";
					break;
				default: cout << "Please select one of the listed options to continue...\n";
					break;
				}
			}

			//TODO:Random monster Generation Function
			//Monster one = randomMonster();
			int random = rand() % 3;//or total in random list for day/night or difficulty?
			Skeleton skel;
			Lizardman liz;
			Goblin gob;
			Monster randomMonster;
			//put in function->
			if (random == 2) {
				randomMonster = skel;
			}
			else if (random == 3) {
				randomMonster = liz;
			}
			else {
				randomMonster = gob;
			}

			//TODO random weapon generation for monsters in function
			Sword mon;
			randomMonster.setWeapon(mon);

			//TODO First Monster fight
			//Single monster encounter (one), change one to be whatever monster, or generate.
			cout << "As you reach the next clearing you come upon a lone " << randomMonster.getSpecies() << ".\n"
				"Upon seeing you the " << randomMonster.getSpecies() << " charges you and attacks with a " << randomMonster.getWeapon().getName() << ".\n";
			bool inFight = true;
			while (inFight) {
				
				int playInit = (rand() % 10) + 1;
				int monsterInit = (rand() % 10) + 1;
				cout << "Initative is rolled, you got a " << playInit << ", the monsters got a " << monsterInit << ".\n";
				char lastAction = 'n';
				if (playInit <= monsterInit && lastAction == 'n') {
					playerAttack(&playerOne, &randomMonster);
					lastAction = 'p';
					//TODO: deathCheckM(&randomMonster);
					if (randomMonster.getHealth() <= 0) {
						inFight = false;
						cout << "The " << randomMonster.getSpecies() << " has died.\n";
						lastAction = 'd';
						break;
					}
				}
				else if(playInit > monsterInit && lastAction == 'n') {
					monsterAttacks(&playerOne, &randomMonster);
					//TODO: deathCheckP(&randomMonster);
					lastAction = 'm';
					if (playerOne.getHealth() <= 0) {
						gameRunning = false;
						inFight = false;
						cout << "You Died! Please try again.\n";
					}
					
				}
				if (lastAction == 'm') {
					playerAttack(&playerOne, &randomMonster);
					//TODO: deathCheckM(&randomMonster);
					cout << "Round Complete###############(One Action each is a round...)\n";
					if (randomMonster.getHealth() <= 0) {
						inFight = false;
						cout << "The " << randomMonster.getSpecies() << " has died.\n";
						lastAction = 'd';
						break;
					}
				}
				else if (lastAction == 'p') {
					monsterAttacks(&playerOne, &randomMonster);
					//TODO: deathCheckP(&randomMonster);
					cout << "Round Complete###############(One Action each is a round...)\n";
					lastAction = 'm';
					if (playerOne.getHealth() <= 0) {
						gameRunning = false;
						cout << "You Died! Please try again.\n";
					}
				}
			}

		}
	}

}// End

//Main Menu Tree
void mainMenu() {


}

void shop(player* playerOne) {
	cout << "Why hello there traveler!\n I have some supplies if you want to browse my wares?\n";
	int input = -1;

	//Shop object instance
	Shop shopOne;

	//Shop decision tree
	while (input != 5) {

		cout << "Do you wish to buy 1-Weapons, 2-Armor, 3-Items, 4-Sell Equipment, or 5-Continue your Journey?\n";
		cin >> input;
		switch (input) {
		case 1:
			input = -1;
			cout << "What weapon would you like to buy? 1-" << shopOne.getDagger().getName() << "(" << shopOne.getDagger().getCost() <<
				" gold pieces) 2-" << shopOne.getMace().getName() << "(" << shopOne.getMace().getCost() <<
				" gold pieces) 3-" << shopOne.getSword().getName() << "(" << shopOne.getSword().getCost() << " gold pieces).\n";
			cin >> input;
			if (input == 1) {
				if (playerOne->getGold() >= shopOne.getDagger().getCost()) {
					cout << "You bought a " << shopOne.getDagger().getName() << " for " << shopOne.getDagger().getCost() << " gold pieces.\n";
					playerOne->setWeapon(shopOne.getDagger());
					playerOne->setGold(playerOne->getGold() - shopOne.getDagger().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
			}
			else if (input == 2) {
				if (playerOne->getGold() >= shopOne.getMace().getCost()) {
					cout << "You bought a " << shopOne.getMace().getName() << " for " << shopOne.getMace().getCost() << " gold pieces.\n";
					playerOne->setWeapon(shopOne.getMace());
					playerOne->setGold(playerOne->getGold() - shopOne.getMace().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}

			}
			else if (input == 3) {
				if (playerOne->getGold() >= shopOne.getSword().getCost()) {
					cout << "You bought a " << shopOne.getSword().getName() << " for " << shopOne.getSword().getCost() << " gold pieces.\n";
					playerOne->setWeapon(shopOne.getSword());
					playerOne->setGold(playerOne->getGold() - shopOne.getSword().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
			}
			input = -1;
			break;
		case 2:
			input = -1;
			cout << "What armor would you like to buy? 1-" << shopOne.getLeatherArmor().getName() << "(" << shopOne.getLeatherArmor().getCost() <<
				" gold pieces) 2-" << shopOne.getChainArmor().getName() << "(" << shopOne.getChainArmor().getCost() <<
				" gold pieces) 3-" << shopOne.getPlateArmor().getName() << "(" << shopOne.getPlateArmor().getCost() << " gold pieces).\n";
			cin >> input;
			if (input == 1) {
				if (playerOne->getGold() >= shopOne.getLeatherArmor().getCost()) {
					cout << "You bought " << shopOne.getLeatherArmor().getName() << " for " << shopOne.getLeatherArmor().getCost() << " gold pieces.\n";
					playerOne->setArmor(shopOne.getLeatherArmor());
					playerOne->setGold(playerOne->getGold() - shopOne.getLeatherArmor().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
			}
			else if (input == 2) {
				if (playerOne->getGold() >= shopOne.getChainArmor().getCost()) {
					cout << "You bought " << shopOne.getChainArmor().getName() << " for " << shopOne.getChainArmor().getCost() << " gold pieces.\n";
					playerOne->setArmor(shopOne.getChainArmor());
					playerOne->setGold(playerOne->getGold() - shopOne.getChainArmor().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
			}
			else if (input == 3) {
				if (playerOne->getGold() >= shopOne.getPlateArmor().getCost()) {
					cout << "You bought " << shopOne.getPlateArmor().getName() << " for " << shopOne.getPlateArmor().getCost() << " gold pieces.\n";
					playerOne->setArmor(shopOne.getPlateArmor());
					playerOne->setGold(playerOne->getGold() - shopOne.getPlateArmor().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}

			}
			input = -1;
			break;
		case 3:
			input = -1;
			cout << "What item do you want to buy? 1-" << shopOne.getLockpicks().getName() << "(" << shopOne.getLockpicks().getPicks()
				<< "picks)(" << shopOne.getLockpicks().getCost() << "gold pieces), 2-Minor Heal Potion("
				<< shopOne.getMinorHeal().getCost() << "gold pieces), 3-" << shopOne.getLantern().getName() << "("
				<< shopOne.getLantern().getCost() << " gold pieces), 4-" << shopOne.getBomb().getName() << "("
				<< shopOne.getBomb().getCost() << "gold pieces), 5-" << shopOne.getRope().getName() << "("
				<< shopOne.getRope().getUses() << " uses)(" << shopOne.getRope().getCost() << " gold pieces)";
			cin >> input;
			if (input == 1) {
				if (playerOne->getGold() >= shopOne.getLockpicks().getCost()) {
					cout << "You bought " << shopOne.getLockpicks().getName() << " for " << shopOne.getLockpicks().getCost() << " gold pieces.\n";
					playerOne->addToInventory(shopOne.getLockpicks());
					playerOne->setGold(playerOne->getGold() - shopOne.getLockpicks().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}

			}
			else if (input == 2) {
				if (playerOne->getGold() >= shopOne.getMinorHeal().getCost()) {
					cout << "You bought " << shopOne.getMinorHeal().getName() << " for " << shopOne.getMinorHeal().getCost() << " gold pieces.\n";
					playerOne->addToInventory(shopOne.getMinorHeal());
					playerOne->setGold(playerOne->getGold() - shopOne.getMinorHeal().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}

			}
			else if (input == 3) {
				if (playerOne->getGold() >= shopOne.getLantern().getCost()) {
					cout << "You bought " << shopOne.getLantern().getName() << " for " << shopOne.getLantern().getCost() << " gold pieces.\n";
					playerOne->addToInventory(shopOne.getLantern());
					playerOne->setGold(playerOne->getGold() - shopOne.getLantern().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}

			}
			else if (input == 4) {
				if (playerOne->getGold() >= shopOne.getBomb().getCost()) {
					cout << "You bought " << shopOne.getBomb().getName() << " for " << shopOne.getBomb().getCost() << " gold pieces.\n";
					playerOne->addToInventory(shopOne.getBomb());
					playerOne->setGold(playerOne->getGold() - shopOne.getBomb().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
			}
			else if (input == 5) {
				if (playerOne->getGold() >= shopOne.getRope().getCost()) {
					cout << "You bought " << shopOne.getRope().getName() << " for " << shopOne.getRope().getCost() << " gold pieces.\n";
					playerOne->addToInventory(shopOne.getRope());
					playerOne->setGold(playerOne->getGold() - shopOne.getRope().getCost());
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}
				else {
					cout << "You do not have enough gold to purchase this item.\n";
					cout << "You have " << playerOne->getGold() << " gold pieces.\n";
				}

			}
			input = -1;
			break;
		case 4:
			cout << "What would you like to sell?";
			//playerOne.setGold(playerOne.getGold() - shopOne.getRope().getCost());
			cin >> input;
			input = -1;
			break;
		default:
			break;
		}//end of switch
	}//end of while

}

//Create char, intial stats/class
void characterCreation(player* playerOne) {
	int input = -1;
	while (input < 1 || input > 3) {
		cout << "Please select a class:\n"
			"1- Fighter: High Health, High Combat, Low Magic, Always needs Keys. \n"
			"2- Mage: Low Health, Low Combat, High Magic, Can learn unlock spell, allowing a medium chance to pick locks.\n"
			"3- Thief: Medium Health, Medium Combat, Medium Magic, Has a chance a high chance to pick locks.\n";
		cin >> input;
		switch (input) {

		case 1:
			playerOne->setClass(input);
			cout << "You selected a fighter.\n";
			break;
		case 2:
			playerOne->setClass(input);
			cout << "You selected a mage.\n";
			break;
		case 3:
			playerOne->setClass(input);
			cout << "You selected a thief.\n";
			break;
		default:cout << "Please select one of the listed options to continue...\n";
			break;
		}
	}



	//Roll intial health + modifier and stats based on class
	int i = playerOne->getClassN();
	//Set player starting stats
	//Fighter
	if (input == 1) {
		playerOne->setHealth((rand() % 20) + 20);
		playerOne->setCombat(18);
		playerOne->setMagic(20);
	}
	//Mage
	else if (input == 2) {
		playerOne->setHealth((rand() % 20) + 5);
		playerOne->setCombat(20);
		playerOne->setMagic(18);
	}
	//Thief
	else {
		playerOne->setHealth((rand() % 20) + 10);
		playerOne->setCombat(19);
		playerOne->setMagic(19);
	}

	//Display current player stats
	cout << "Your current stats are:\n";
	playerOne->display();
}

//Allows monsters to attack the player
void monsterAttacks(player* playerOne, Monster* monster) {
	//Attack roll
	int roll = (rand() % 20) + 1;
	
	if (monster->getCombat() > monster->getMagic() && monster->getHealth() > 0) {
		cout << "The " << monster->getSpecies() << " attempts to attack you!\n";
		if (roll > monster->getCombat() - playerOne->getArmor()) {
			cout << "The " << monster->getSpecies() << " hits you for " << monster->getWeapon().getDmg() << " damage.\n";
			playerOne->setHealth(playerOne->getHealth() - monster->getWeapon().getDmg());
			cout << "You have " << playerOne->getHealth() << " health left.";
		}
		else {
			cout << "The " << monster->getSpecies() << " missed you.\n";
		}
	}
}

//Allows player to attack, cast spell, or run away during monster encounter
void playerAttack(player* playerOne, Monster* monster) {
	int input = 0;
	//Attack roll
	int roll = (rand() % 20) + 1;
	while (input < 1 || input > 3 && monster->getHealth() > 0) {
		cout << "Which action will you take: \n"
			"1- Attack\n"
			"2- Magic\n"
			"3- Run Away\n";
		cin >> input;
		switch (input) {
		case 1:
			cout << "The " << monster->getSpecies() << " has " << monster->getHealth() << " health left.\n";
			cout << "You attempt to attack the " << monster->getSpecies() << " with your "<< playerOne->getWeapon().getName() <<".\n";
			if (roll >= (playerOne->getCombat() - monster->getArmor())) {
				cout << "You hit the " << monster->getSpecies() << " for " << playerOne->getWeapon().getDmg() << " damage!\n";
				if (monster->getHealth() - playerOne->getWeapon().getDmg() <= 0)
					monster->setHealth(0);
				else
					monster->setHealth(monster->getHealth() - playerOne->getWeapon().getDmg());
				cout << "The " << monster->getSpecies() << " has " << monster->getHealth() << " health left.\n";
			}
			else {
				cout << "You missed the " << monster->getSpecies() << "!\n";;
			}
			break;
		case 2:
			cout << "The " << monster->getHealth() << " has " << monster->getHealth() << " health left.\n";
			cout << "You try to cast magic missle...\n";
			if (roll >= playerOne->getMagic() - monster->getArmor()) {
				cout << "Your spell hits the " << monster->getSpecies() << " for 5 damage!\n";
				
				if (monster->getHealth() - 5 <= 0)
					monster->setHealth(0);
				else
					monster->setHealth(monster->getHealth() - 5);
				cout << "The " << monster->getSpecies() << " has " << monster->getHealth() << " health left.\n";
			}
			else {
				cout << "Your spell fizzled and died, having no effect on the " << monster->getSpecies() << ".\n";
			}
			break;
		case 3:
			cout << "You attempt to turn run away from the " << monster->getSpecies() << ".\n";
			//TODO:Percentage check of some kind to run away?
			break;
		default: cout << "Please select one of the listed options to continue...\n";
			break;
			
		}
	}
}

