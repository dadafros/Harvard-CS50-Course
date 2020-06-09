#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = (float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            image[i][j].rgbtBlue = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtRed = round(avg);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumBlue = 0, sumGreen = 0, sumRed = 0, count = 0;
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (k >= 0 && l >= 0 && k < height && l < width)
                    {
                        sumBlue += image[k][l].rgbtBlue;
                        sumGreen += image[k][l].rgbtGreen;
                        sumRed += image[k][l].rgbtRed;
                        count++;
                    }
                }
            }
            tmp[i][j].rgbtBlue = round(sumBlue / count);
            tmp[i][j].rgbtGreen = round(sumGreen / count);
            tmp[i][j].rgbtRed = round(sumRed / count);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blueX = 0, blueY = 0, greenX = 0, greenY = 0, redX = 0, redY = 0;
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (k >= 0 && l >= 0 && k < height && l < width)
                    {
                        int m = k - i + 1, n = l - j + 1;
                        blueX += image[k][l].rgbtBlue * Gx[m][n];
                        blueY += image[k][l].rgbtBlue * Gy[m][n];
                        greenX += image[k][l].rgbtGreen * Gx[m][n];
                        greenY += image[k][l].rgbtGreen * Gy[m][n];
                        redX += image[k][l].rgbtRed * Gx[m][n];
                        redY += image[k][l].rgbtRed * Gy[m][n];
                    }
                }
            }
            int sobelBlue = round(sqrt(blueX * blueX + blueY * blueY));
            int sobelGreen = round(sqrt(greenX * greenX + greenY * greenY));
            int sobelRed = round(sqrt(redX * redX + redY * redY));
            if (sobelBlue > 255)
            {
                sobelBlue = 255;
            }
            if (sobelGreen > 255)
            {
                sobelGreen = 255;
            }
            if (sobelRed > 255)
            {
                sobelRed = 255;
            }
            tmp[i][j].rgbtBlue = sobelBlue;
            tmp[i][j].rgbtGreen = sobelGreen;
            tmp[i][j].rgbtRed = sobelRed;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}