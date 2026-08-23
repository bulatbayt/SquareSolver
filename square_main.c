#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "solver.h"
#include "color.h"

int get_info (double* a, double*b, double*c);

void show_info (double* x1, double* x2, enum desion quantity);

void clean_buffer ();

int main ()
{
    struct coeff square = { };

    double x1 = 0, x2 = 0;

    char chose = '0';

    printf (GREEN "Программа предназначена для решения квадратного уравнения\n" RESET_COLOR);
    printf (GREEN "Квадратное уравнение представлено в виде уравнения: a*x^2+b*x+c\n" RESET_COLOR);

    while (chose != 'q')
    {
        if (get_info (&square.a, &square.b, &square.c) == 0)
        {
            printf (RED "Вы ввели некорректное значение" RESET_COLOR);
            break;
        }

        clean_buffer ();
        
        enum desion quantity = info_processing (square, &x1, &x2);

        show_info (&x1, &x2, quantity);

        printf (YELLOW "Введите q для выхода из программы (другую букву для продолжения): " RESET_COLOR);
        chose = getchar ();

        clean_buffer ();
    }

    return 0;
}

int get_info (double* a, double* b, double* c)
{ 
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    printf (BLUE "Введите значения для коэффициента a: " RESET_COLOR);
    if (scanf ("%lg", a) == 0)
    {
        printf (GREEN "You are Poltoraskha meow!" RESET_COLOR);

        return 0;
    }

    printf (BLUE "Введите значения для коэффициента b: " RESET_COLOR); // vsnprintf
    if (scanf ("%lg", b) == 0)
    {
        printf (GREEN "You are Poltoraskha meow!" RESET_COLOR);

        return 0;
    }

    printf (BLUE "Введите значения для коэффициента c: " RESET_COLOR);
    if (scanf ("%lg", c) == 0)
    {
        printf (GREEN "You are Poltoraskha meow!" RESET_COLOR);

        return 0;
    }

    return 1;
}
int info_processing (struct coeff values, double* x1, double* x2)
{
    double d = values.b * values.b - 4 * values.a * values.c;

    if (d < 0)
    {
        return NO_DECISION;
    }

    if (is_zero (d)) 
    {
        if (is_zero (values.a) != 1)
        {
            *x1 = -values.b / (2 * values.a);

            return ONE_DECISION;
        }

        if (is_zero (values.a) == 1 && is_zero (values.b) != 1)
        {                
            *x1 = (-values.c) / (values.b);

            return ONE_DECISION;
        }

        if (is_zero (values.a) == 1 && is_zero (values.b) == 1)
        {
            if (is_zero (values.c) == 1)
                return ENDLESSLY_DECISION;
            else
                return NO_DECISION;
        }
    }
    
    if (d > 0) 
    {
        if (is_zero (values.a) != 1)
        {
            *x1 = (-values.b - sqrt(d)) / (2 * values.a);
            *x2 = (-values.b + sqrt(d)) / (2 * values.a);

            return TWO_DECISION;
        }
        else
        {
            *x1 = -values.c / values.b;

            return ONE_DECISION;
        }
    }
    
    return NO_DECISION;
}

int is_zero (double value)
{
    if (value > -pow (10, -10) && value < pow (10, -10)) // 1e-10
        return 1;
    else
        return 0;
}

void show_info (double* x1, double* x2, enum desion quantity)
{
    if (quantity == NO_DECISION)
    {
        printf (RED "No dision\n" RESET_COLOR);
    }

    if (quantity == ENDLESSLY_DECISION)
    {
        printf (BLUE "Endlessly dision\n" RESET_COLOR);
    }

    if (quantity == ONE_DECISION)
    {
        printf (GREEN "X = %lg \n" RESET_COLOR, *x1);
    }

    if (quantity == TWO_DECISION)
    {
        printf (GREEN "X1 = %lg\n" RESET_COLOR, *x1);
        printf (GREEN "X2 = %lg\n" RESET_COLOR, *x2);
    }
}

void clean_buffer()
{
    while (getchar () != '\n')
    {
        continue;
    }
}