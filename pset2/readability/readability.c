#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");
    int numLetters = 0;
    int numWords = 0;
    int numSentences = 0;

    for (int i = 0; text[i] != '\0' ; i++)
    {
        //Count if is an alfabethic character
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            numLetters++;
        }
        //Count if is an space, or if it's the end of the text from input
        if (text[i] == ' ' || ((text[i] == '.' || text[i] == '!' || text[i] == '?') && text[i + 1] == '\0'))
        {
            numWords++;
        }
        //Count number of .!? characters in text
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            numSentences++;
        }
    }

    float index = 5.88 * (float) numLetters / numWords - 29.6 * (float) numSentences / numWords - 15.8;

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int)round(index));
    }
}