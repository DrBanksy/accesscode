/*
Program that allows user to enter a 4 digit code (or can be auto generated),
which can then be encrypted and compared against the default access code
(4523). User has the option to decrypt the code. The number of times the encrypted
access code is correct/wrong will be displayed if user wishes.

Author: Cormac Smith
Date: 20/04/20


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 4

//structure templates
struct code_counter
{
    int correct_code;
    int wrong_code;
};


//function signature
void empty_stdin(void);
void userCode(int*, int*, int*);
void encryptCode(int*, int*);
void compareCode(int*, int*, struct code_counter*);
void decryptCode(int*, int*, int*);
void displayRes(struct code_counter*);
void exitProgram();



int main()
{
    int access_code[4] = { 4, 5, 2, 3 };
    int userNums[SIZE];

    //correct and wrong counters stored in this struct
    struct code_counter results;
    struct code_counter* ptr;

    //assign address of the struct variable to ptr
    ptr = &results;

    int userOption;
    int encrypted = 0;
    int decrypted = 0;
    int check = 0;

    do
    {
        //main menu
        printf("\n==================\n");
        printf("Security Authorisation");
        printf("\n==================\n\n");
        printf("1)Enter a code or generate a random code\n");
        printf("2)Encrypt code\n");
        printf("3)Check if encrypted code matches default access code\n");
        printf("4)Decrypt code\n");
        printf("5)Display number of times the code is correct/wrong\n");
        printf("6)Exit program\n");
        printf("\nEnter your choice below:\n");

        //grab users input
        scanf("%d", &userOption); 

        switch (userOption)
        {
            case 1:
            {
                //function that will allow the user to enter or auto gen a 4 digit access code
                userCode(userNums, &encrypted, &check);
                break;
            }
       

            //encrypts code
            case 2:
            {
                //setting value to zero, this varible lets us know if its been decrypted, it also only lets user decypt the code once
                decrypted = 0;

                //this prevents the user from selecting option 2 when they have no access code(i.e they press option 2 before option 1)
                if (check > 0)
                {
                    //if code has not already been encrypted then the value of "encrypted" will be 0
                    if (encrypted == 0)
                    {
                        encryptCode(userNums, &encrypted);
                        break;
                    }
                    else
                    {
                        puts(" **Error: Code already encypted**");
                        break;
                    }
                }
                break;


            }
        
             
            //compares code with the preset access code
            case 3:
            {
                if (encrypted > 0)
                {
                    //store correct and wrong results to struct variable "results"
                    compareCode(userNums, access_code, ptr);
                    break;
                }
                else
                {
                    puts(" **Error: Must encrypt access code before comparing");
                    break;
                }

            }
 

            //decrypts users four digit code
            case 4:
            {
                if (encrypted > 0)
                {
                    decryptCode(userNums, &decrypted, &encrypted);
                    break;
                }
                else
                {
                    puts(" **Error: Code must be encrypted first**");
                    break;
                }

            }
    

            case 5:
            {
                /*
                we are passing the address of the struct variable cotaining the counters
                "correct" and "wrong"
                */
                if (ptr->correct_code > 0 || ptr->wrong_code > 0)
                {
                    displayRes(ptr);
                    break;
                }
                else
                {
                    puts("**Error: Encrypted code must be compared with access code at least once**");
                    break;
                }

                
            }


            case 6:
            {
                exitProgram();
                break;
            }

            //error checking: if a num > 6 or num < 1 or a character/symbol is entered
            default: 
            {
                    puts("**Error: invalid option");
                    //clears standard input to prevent infinte loop if char is entered
                    empty_stdin();
                    break;
            }


       
        }


    } while (1);


    
}

//option 1
//generates the users four digit code
void userCode(int* userNums, int* encrypted, int* check)
{
    /*
    setting value of encrypted to zero so if the user wanted to enter a new four digit code, 
    they will be allowed to encrypt it.
    */
    (*encrypted) = 0;
    int rtn;
    int userChoice;


    puts("---------------------------------");
    puts("1) Enter your own 4 digit number");
    puts("2) Generate random 4 digit number");
    puts("---------------------------------");
    do 
    {
        rtn = scanf("%d", &userChoice);
        if (rtn == 0)
        {
            //this prints an error to the users terminal
            fputs(" **error: must select option 1 or 2**\n", stderr); //satandard error

            //clears the standard input,  repeats until valid option is entered
            empty_stdin();

            
        }
        if (userChoice > 2 || userChoice < 1)
        {
            puts(" **Error: Enter a valid option buddy**");
            
        }


    } while (rtn == 0 || userChoice > 2 || userChoice < 1);

    //if the user wants to specify their own 4 digit code
    if (userChoice == 1)
    {
        for (int i = 0; i < SIZE; i++)
        {
            do
            {
                //error checking for characters/symbols
                rtn = scanf("%d", &*(userNums + i));
                if (rtn == 0)
                {
                    //this prints an error to the users terminal
                    fputs(" **Error: must enter an integer between 0-9 inclusive**\n", stderr); //satandard error

                    //clears the standard input,  repeats until valid option is entered
                    empty_stdin();


                }

                //checking if integer is within the boundaries of 0-9
                else if (*(userNums + i) < 0 || *(userNums + i) > 9)
                {
                    puts(" **Error: integer must be between 0-9 inclusive**");
                }


            } while (*(userNums + i) < 0 || *(userNums + i) > 9);
            
        }//END for loop
        //we dont want the user picking option 2 when they have not been given an access code
        (*check) = (*check) + 1;

    }//END if statement

    //if the user wants an auto generated 4 digit code
    else if (userChoice == 2)
    {
        for (int i = 0; i < SIZE; i++)
        {
            *(userNums + i) = rand() % 10 + 0;
        }//END for
        (*check) = (*check) + 1;
        //displaying the auto generated code to the user
        puts("Your randomly generated code is:");
        for (int i = 0; i < SIZE; i++)
        {
            printf("%d", *(userNums + i));
        }//END for
        
    }//END elseif

}//END function

//option 2
void encryptCode(int* myarr, int* encrypted)
{
    
    int temp;


    //swapping 1st and 3rd element
    temp = *(myarr + 0);
    *(myarr + 0) = *(myarr + 2);
    *(myarr + 2) = temp;

    //swapping 2nd and 4th
    temp = *(myarr + 1);
    *(myarr + 1) = *(myarr + 3);
    *(myarr + 3) = temp;

    //add 1 to each number
    for (int i = 0; i < SIZE; i++)
    {
        *(myarr + i) = *(myarr + i) + 1;
        if (*(myarr + i) == 10)
        {
            *(myarr + i) = 0;
        }

    }

    (*encrypted) = (*encrypted) + 1;

    //printing out the users encrypted code
    puts("Your encrypted code is:");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d", *(myarr + i));

    }


}


//option 3
//comparing encrypted code with access code (4523)
void compareCode(int* usersCode, int* accessCode, struct code_counter* ptr)
{

    

    /*
        we only want to initialise the counters to zero ONCE,
        this static variable will only be ZERO once then it will always be a different value,
        therefore the while loop WILL NOT execute again
        */
    static int stop = 0;
    while (stop == 0)
    {
        ptr->correct_code = 0;
        ptr->wrong_code = 0;
        stop++;
    }

    for (int i = 0; i < SIZE; i++)
    {
        //if one of the elements does not match, then we increment "different"
        if (*(usersCode + i) != *(accessCode + i))
        {
            //different++;
            puts("Wrong code entered");
            ptr->wrong_code++;

            break;
        }
        else
        {
            puts("Correct code entered");
            ptr->correct_code++;
            break;
        }//ENDIF

    }//ENDFOR

}//end function

//option 4
//this function decrypts an encrypted code
void decryptCode(int* usersCode, int* decrypted, int *encrypted )
{
    
    int temp;
    (*decrypted) = (*decrypted) + 1;
    if ((*decrypted) == 1)
    {
        //subtract 1 from each number
        for (int i = 0; i < SIZE; i++)
        {
            //subtracting 1
            *(usersCode + i) = *(usersCode + i) - 1;

            //if a negative value occurs then set value to 9
            if (*(usersCode + i) == -1)
            {
                *(usersCode + i) = 9;
            }

        }//ENDFOR

        //swap the 1st number with the 3rd
        temp = *(usersCode + 0);
        *(usersCode + 0) = *(usersCode + 2);
        *(usersCode + 2) = temp;

        //swap the 2nd number with the 4th number
        temp = *(usersCode + 1);
        *(usersCode + 1) = *(usersCode + 3);
        *(usersCode + 3) = temp;

        puts("Your code was:");
        for (int i = 0; i < SIZE; i++)
        {
            printf("%d", *(usersCode + i));

        }

        (*encrypted) = 0;
        
        
    }//ENDIF

    //if value of var decrypted is greater than one it means the code has been decrypted 
    else if ((*decrypted) > 1)
    {
        puts("**Error: Code already decrypted");
    }

}

//option5
//this function will display a counter for incorrect and correct guesses of the code
void displayRes(struct code_counter* ptr)
{
    printf("correct = %d\n", ptr->correct_code);
    printf("wrong = %d\n", ptr->wrong_code);

}


//option6
//this will end the program
void exitProgram(void)
{
    char userInput[2];
    puts("Are you sure you want to end the program: y/n");
    
    do
    {
        scanf("%s", userInput);
        


        if (strcmp(userInput, "y") == 0)
        {

            exit(0);
        }
        else if (strcmp(userInput, "n") == 0)
        {
            return;
        }

        else if (strcmp(userInput, "y") != 0 && strcmp(userInput, "n") != 0)
        {
            puts(" **Error: Must enter either \"y\" or \"n\" **");
        }

    } while (strcmp(userInput, "y") != 0 && strcmp(userInput, "n") != 0);
}

//this function clears the standard input
void empty_stdin(void)
{
    //getchar reads a single character from std input regardless of what data type it is
    int c = getchar();
    while (c != '\n' && c != EOF)
    c = getchar();
}