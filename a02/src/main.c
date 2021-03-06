#include <stdio.h>
#include <string.h>
#include "HashTableAPI.h"
#include "PasswordVault.h"

int main ()
{
	char intro;
  char buffer[100];

	while(intro != 'E')
	{
		//Initialize hashtable for the vault and user struct
		HTable * passVault = createVault(109, hashFunction, deletePassword, printPassword);
		User theUser;

		//Show menu screen and ask user to create account or sign in
		printf("\n*********************** PASSWORD VAULT ***********************\n");
  	printf("\n(N) Create a new account\n(S) Sign in\n\n(E) Exit\n\n> ");
		fgets(buffer, 100, stdin);
		buffer[strlen(buffer) - 1] = '\0';
		intro= buffer[0];

		//Prompt user again if answer is not one of the optioj
		while(intro != 'N' && intro != 'S' && intro != 'E')
		{
			if(intro == 'N' || intro == 'S' || intro == 'E')
			{
				break;
			}
			printf("> Not one of the options. Try again.\n> ");
			fgets(buffer, 100, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			intro= buffer[0];
		}

		switch(intro)
		{
			//Create new user
			case'N':
			theUser = createUser(passVault);
			break;

			//Sign in user
			case'S':
			printf("> [Username]: ");
			fgets(theUser.username, 100, stdin);
			theUser.username[strlen(theUser.username) - 1] = '\0';

			//Check if input is valid
			FILE * userFile;
			userFile = fopen(createPathname(theUser),"rb+");
			while(userFile == NULL)
			{
				printf("> No such file. Please try again\n");
				printf("> [Username]: ");
				fgets(theUser.username, 100, stdin);
				theUser.username[strlen(theUser.username) - 1] = '\0';
				userFile = fopen(createPathname(theUser),"rb+");
			}
			fclose(userFile);

			//Ask password
			printf("> [Password]: ");
			fgets(theUser.password, 100, stdin);
			theUser.password[strlen(theUser.password) - 1] = '\0';

			signin(theUser, passVault);
			printf("> [Logged in successfully]\n");
			break;

			//Exit program
			case'E':
			printf("\n********************** THANK YOU FOR USING PASSWORD VAULT **********************\n\n");
			exit(-1);
			break;
		}

		char option = 'X';
		while(option != '2')
		{
			//Show menu screen and prompt for answer
			printf("\n*********************** M E N U ***********************\n");
			printVault(theUser, passVault);
			printf("\n(A) Add a system and password \n");
			printf("(B) Change a password \n");
			printf("(C) Get a password \n");
			printf("(D) Remove a password \n\n");
			printf("(1) Change your password vault password \n");
			printf("(2) Logout\n\n> ");
			fgets(buffer, 100, stdin);
			option = buffer[0];

			//Ask until answer is one of the options
			while(option != 'A' && option != 'B' && option != 'C' && option != 'D' && option != '1' && option!= '2')
			{
				if(option == 'A' || option == 'B' || option == 'C' || option == 'D' || option == '1' || option== '2')
				{
					break;
				}
				printf("> Not one of the options. Try again.\n> ");
				fgets(buffer, 100, stdin);
				option = buffer[0];
			}

			switch(option)
			{
				//Add new system and password
				case 'A':
				printf("*********************** ADD NEW SYSTEM AND PASSWORD ***********************\n\n");
				//Ask new system and password
				User * newSystem = malloc(sizeof(User));
				printf("> [Enter new system]: ");
				fgets(newSystem->username, 100, stdin);
				newSystem->username[strlen(newSystem->username) - 1] = '\0';
				printf("> [Enter system password]: ");
				fgets(newSystem->password, 100, stdin);
				newSystem->password[strlen(newSystem->password) - 1] = '\0';

				//Add to the vault
				addPassword(newSystem, passVault);
				loadTable(theUser, passVault);
				printf("> [Password added successfully]\n");
				break;

				//Change a system password
				case 'B':
				printf("\n*********************** CHANGE A SYSTEM PASSWORD ***********************\n\n");
				printVault(theUser, passVault);

				//Ask system to change
				User * changeSystem = malloc(sizeof(User));
				printf("\n> [Enter system]: ");
				fgets(changeSystem->username, 100, stdin);
				changeSystem->username[strlen(changeSystem->username) - 1] = '\0';

				//Ask again if system does not exist
				while(checkSystem(changeSystem, passVault) == 0)
				{
					printf("> No such system in the vault. Try again ");
					printf("\n> [Enter system]: ");
					fgets(changeSystem->username, 100, stdin);
					changeSystem->username[strlen(changeSystem->username) - 1] = '\0';
				}

				//Ask password
				printf("> [Enter new system password]: ");
				fgets(changeSystem->password, 100, stdin);
				changeSystem->password[strlen(changeSystem->password) - 1] = '\0';

				//changePassword
				changePassword(changeSystem, passVault);
				loadTable(theUser, passVault);
				printf("\n> [Password changed successfully]\n");
				break;

				//Get a system password
				case 'C':
				printf("\n*********************** GET A SYSTEM PASSWORD ***********************\n\n");
				printVault(theUser, passVault);

				//Ask system to get
				printf("\n[Enter system]: ");
				User * getSystem = malloc(sizeof(User));
				fgets(getSystem->username, 100, stdin);
				getSystem->username[strlen(getSystem->username) - 1] = '\0';

				//Ask again if system does not exist
				while(checkSystem(getSystem, passVault) == 0)
				{
					printf("> No such system in the vault. Try again ");
					printf("\n> [Enter system]: ");
					fgets(getSystem->username, 100, stdin);
					getSystem->username[strlen(getSystem->username) - 1] = '\0';
				}

				//Show password
				strcpy(getSystem->password, getPassword(getSystem, passVault));
				printf("\n           >>>>[Your Password is]: %s<<<<\n", getSystem->password);
				break;

				//Remove a system password
				case 'D':
				printf("\n*********************** REMOVE A SYSTEM PASSWORD ***********************\n\n");
				printVault(theUser, passVault);

				//Ask system to remove
				printf("\n> [Enter system]: ");
				User * removeSystem = malloc(sizeof(User));
				fgets(removeSystem->username, 100, stdin);
				removeSystem->username[strlen(removeSystem->username) - 1] = '\0';

				//Ask again if system doesnt exist
				while(checkSystem(removeSystem, passVault) == 0)
				{
					printf("> No such system in the vault. Try again ");
					printf("\n> [Enter system]: ");
					fgets(removeSystem->username, 100, stdin);
					removeSystem->username[strlen(removeSystem->username) - 1] = '\0';
				}

				//Remove password
				removePassword(removeSystem, passVault);
				loadTable(theUser, passVault);
				printf("\n> [System and password removed successfully]\n");
				break;

				//Change master password
				case '1':
				printf("\n*********************** CHANGE YOUR VAULT PASSWORD ***********************\n\n");
				printf("> [Your username]: %s\n", theUser.username);
				printf("> [Enter new vault password]: ");

				fgets(theUser.password, 100, stdin);
				theUser.password[strlen(theUser.password) - 1] = '\0';
				changePassword(&theUser, passVault);
				loadTable(theUser, passVault);
				printf("> [Vault Password updated successfully]\n");
				break;

				//Logout
				case '2':
				destroyTable(passVault);
				printf("> [Logout successful]\n");

				break;
			}
		}
	}
  return 0;
}
