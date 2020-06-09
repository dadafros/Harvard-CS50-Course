#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n = 0;
    while (n < 1 || n > 8)
    {
        n = get_int("Height: ");
    } 

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            printf(" ");
        }
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}