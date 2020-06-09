#include <stdio.h>
#include <cs50.h>
#include <string.h>

const int UPPERINSENSITIVE = 65;
const int LOWERINSENSITIVE = 97;
const int TRANSFORMCASE = 32;

bool validate_key(string key);
string encipher(string key, string text);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Invalid arguments\n");
        return 1;
    }
    if (validate_key(argv[1]))
    {
        return 1;
    }

    string plaintext = get_string("plaintext: ");

    printf("ciphertext: %s\n", encipher(argv[1], plaintext));
    return 0;
}

bool validate_key(string key)
{
    int i;
    for (i = 0; key[i] != '\0' ; i++)
    {
        if (!((key[i] >= 'a' && key[i] <= 'z') || (key[i] >= 'A' && key[i] <= 'Z')))
        {
            printf("Key must have only alphabetical characters\n");
            return 1;
        }
        for (int j = i + 1; key[j] != '\0'; j++)
        {
            if (key[i] == key[j] || key[i] == key[j] + TRANSFORMCASE || key[i] == key[j] - TRANSFORMCASE)
            {
                printf("Key must not have repeated characters\n");
                return 1;
            }
        }
    }
    if (i != 26)
    {
        printf("Key length wrong\n");
        return 1;
    }
    return 0;
}

string encipher(string key, string text)
{
    for (int i = 0; text[i] != '\0' ; i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            if (key[text[i] - LOWERINSENSITIVE] >= 'a' && key[text[i] - LOWERINSENSITIVE] <= 'z')
            {
                text[i] = key[text[i] - LOWERINSENSITIVE];
            }
            else
            {
                text[i] = key[text[i] - LOWERINSENSITIVE] + TRANSFORMCASE;
            }
        }
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            if (key[text[i] - UPPERINSENSITIVE] >= 'A' && key[text[i] - UPPERINSENSITIVE] <= 'Z')
            {
                text[i] = key[text[i] - UPPERINSENSITIVE];
            }
            else
            {
                text[i] = key[text[i] - UPPERINSENSITIVE] - TRANSFORMCASE;
            }
        }
        else
        {
            text[i] = text[i];
        }
    }
    return text;
}