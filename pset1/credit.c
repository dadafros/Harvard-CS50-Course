#include <cs50.h>
#include <stdio.h>

int main(void)
{
    while (true)
    {
        long n;
        int digits[20], i;
        int sum = 0;
        n = get_long("Number: ");

        //Split into digits
        for (i = 0; n > 0; i++)
        {
            digits[i] = n % 10;
            n /= 10;
        }

        //Calculate check-sum
        for (int j = 0; j < i; j++)
        {
            if (j % 2 != 0)
            {
                if (2 * digits[j] > 9)
                {
                    sum += (2 * digits[j]) % 10;
                    sum += (2 * digits[j]) / 10;
                }
                else
                {
                    sum += 2 * digits[j];
                }
            }
            else
            {
                sum += digits[j];
            }
        }
        //Conditions to AMEX: number of digits 15, valid check-sum, first two digits 34 or 37
        if (i == 15 && sum % 10 == 0 && digits[i - 1] == 3 && (digits[i - 2] == 4 || digits[i - 2] == 7))
        {
            printf("AMEX\n");
            break;
        }
        //Conditions to MASTERCARD: number of digits 16, valid check-sum, first two digits 51 to 55
        if (i == 16 && sum % 10 == 0 && digits[i - 1] == 5 && (digits[i - 2] >= 1 && digits[i - 2] <= 5))
        {
            printf("MASTERCARD\n");
            break;
        }
        //Conditions to VISA: number of digits 13 or 16, valid check-sum, first digit 4
        if ((i == 13 || i == 16) && sum % 10 == 0 && digits[i - 1] == 4)
        {
            printf("VISA\n");
            break;
        }
        else
        {
            printf("INVALID\n");
            break;
        } 
    } 
}