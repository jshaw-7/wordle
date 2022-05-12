#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// stuff

// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize, string options[]);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);
bool stringin(string word, string options[]);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }
    int wordsize = 0;
    // ensure argv[1] is either 5, 6, 7, or 8 and store that value in wordsize instead
    int x = atoi(argv[1]);
    if ((x != 5) & (x != 6) & (x != 7) & (x != 8))
    {
        printf("Error: wordsize must be either 5, 6, 7, or 8\n");
        return 1;
    }
    wordsize = x;

    // open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }


    // load word file into an array of size LISTSIZE
    string options[LISTSIZE];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("%s", choice);
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);
    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess,
        string guess = get_guess(wordsize, options);
        // array to hold guess status, initially set to zero
        int status[wordsize];
        // set all elements of status array initially to 0, aka WRONG

        for (int j = 0; j < wordsize; j++)
        {
            status[x] = 0;
        }

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        printf(RESET"Guess %i: ", i + 1);
        // Print the guess
        print_word(guess, wordsize, status);
        // if they guessed it exactly right, set terminate loop
        if (score >= (2 * wordsize))
        {
            won = true;
            break;
        }
    }
    // Print the game's result
    if (won)
    {
        printf(GREEN"you won! the correct word was \"%s\"", choice);
        printf(RESET"\n");
    }
    else
    {
        printf(RED"you lost. the correct word was \"%s\"", choice);
        printf(RESET"\n");
    }
    // that's all folks!
    return 0;
}

string get_guess(int wordsize, string options[])
{
    string guess = "";
    // ensure users actually provide a guess that is the correct length and is a real word
    while ((strlen(guess) != wordsize) && (stringin(guess, options)))
    {
        guess = get_string(RESET"Input a %i-letter word: "RESET, wordsize);
    }
    return guess;
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;
    // compare guess to choice and score points as appropriate, storing points in status
    for (int i = 0; i < wordsize; i++)
    {
        for (int x = 0; x < wordsize; x++)
        {
            if ((guess[i] == choice[x]) & (x == i))
            {
                score += 2;
                status[i] = 1;
            }
            else if (guess[i] == choice[x])
            {
                score += 1;
                status[i] = 2;
            }
        }
    }
    return score;
}

void print_word(string guess, int wordsize, int status[])
{
    // print word character-for-character with correct color coding, then reset terminal font to normal
    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == 1)
        {
            printf(GREEN"%c", guess[i]);
        }
        else if (status[i] == 2)
        {
            printf(YELLOW"%c", guess[i]);
        }
        else
        {
            printf(RED"%c", guess[i]);
        }
    }
    printf("\n"RESET);
    return;
}

bool stringin(string word, string options[])
{
    for(int i = 0; i < LISTSIZE; i++)
    {
        if(strcmp(options[i], word) == 0)
        {
                return true;
        }
    }
    return false;
}