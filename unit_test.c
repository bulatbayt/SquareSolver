#include <stdio.h>
#include <math.h>

#include "solver.h"
#include "color.h"

void RunOneTest (struct coeff values, int nRootsRef, double x1Ref, double x2Ref);
void show_info (double* x1, double* x2, enum desion quantity);
int RunTests (int quantity, struct coeff square);

int main ()
{
    struct coeff square = { };

    printf (GREEN "Программа создана для теста программы square.main\n" RESET_COLOR);

    printf (BLUE "Введите количество тестов: " RESET_COLOR);
    int quantity;
    scanf ("%d", &quantity);

    if (RunTests (quantity, square) == 0)
        return 0;
    else 
        return 1;
}

int RunTests (int quantity, struct coeff square)
{
    double x1 = 0, x2 = 0;

    FILE* fp = fopen ("unit_test.txt", "r");

    if (fp == NULL)
    {
       printf ("Ошибка открытия файла");

       return 1;
    }

    for (int i = 0; i < quantity; i++)
    {
        fscanf (fp, "%lg", &square.a);
        fscanf (fp, "%lg", &square.b);
        fscanf (fp, "%lg", &square.c);
        
        enum desion quantity = info_processing (square, &x1, &x2);

        show_info (&x1, &x2, quantity);

        double nRootsRef = 0, x1Ref = 0, x2Ref = 0;

        fscanf (fp, "%lg", &nRootsRef);
        fscanf (fp, "%lg", &x1Ref);
        fscanf (fp, "%lg", &x2Ref);

        double support_change;

        if (x1Ref > x2Ref && quantity == TWO_DISION)
        {
            support_change = x1Ref;
            x1Ref = x2Ref;
            x2Ref = support_change;
        }

        RunOneTest (square, nRootsRef, x1Ref, x2Ref);
    }

    fclose (fp);

    return 0;
}

void RunOneTest (struct coeff values, int nRootsRef, double x1Ref, double x2Ref)
{
    double x1 = 0, x2 = 0;

    int nRoots = info_processing (values, &x1, &x2);

    if (nRoots != nRootsRef || x1Ref != x1 || x2Ref != x2)
        printf ("Test FAILED: a = %lg, b = %lg, c = %lg\n"
                "Expected: %d roots, x1ref = %lg, x2ref = %lg\n"
                "got:      %d roots, x1    = %lg, x2    = %lg\n",
                values.a, values.b, values.c, nRootsRef, x1Ref, 
                x2Ref, nRoots, x1, x2);
    
    else 
        printf ("Succes\n");
}

void show_info (double* x1, double* x2, enum desion quantity)
{
    if (quantity == NO_DISION)
    {
        printf (RED "No dision\n" RESET_COLOR);
    }

    if (quantity == ENDLESSLY_DISION)
    {
        printf (BLUE "Endlessly dision\n" RESET_COLOR);
    }

    if (quantity == ONE_DISION)
    {
        printf (GREEN "X = %lg \n" RESET_COLOR, *x1);
    }

    if (quantity == TWO_DISION)
    {
        printf (GREEN "X1 = %lg\n" RESET_COLOR, *x1);
        printf (GREEN "X2 = %lg\n" RESET_COLOR, *x2);
    }
}

int info_processing (struct coeff values, double* x1, double* x2)
{
    double d = values.b * values.b - 4 * values.a * values.c;

    if (d < 0)
    {
        return NO_DISION;
    }

    if (is_zero (d)) 
    {
        if (is_zero (values.a) != 1)
        {
            *x1 = -values.b / (2 * values.a);

            return ONE_DISION;
        }

        if (is_zero (values.a) == 1 && is_zero (values.b) != 1)
        {                
            *x1 = (-values.c) / (values.b);

            return ONE_DISION;
        }

        if (is_zero (values.a) == 1 && is_zero (values.b) == 1)
        {
            if (is_zero (values.c) == 1)
                return ENDLESSLY_DISION;
            else
                return NO_DISION;
        }
    }
    
    if (d > 0) 
    {
        if (is_zero (values.a) != 1)
        {
            *x1 = (-values.b - sqrt(d)) / (2 * values.a);
            *x2 = (-values.b + sqrt(d)) / (2 * values.a);

            return TWO_DISION;
        }
        else
        {
            *x1 = -values.c / values.b;

            return ONE_DISION;
        }
    }
    
    return NO_DISION;
}

int is_zero (double value)
{
    if (value > -pow (10, -10) && value < pow (10, -10)) // 1e-10
        return 1;
    else
        return 0;
}