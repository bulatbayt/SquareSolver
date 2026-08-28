#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

#include "solver.h"
#include "color.h"
#include "test.h"
#include "mode.h"


int get_info (struct coeff* square);

int hand_assert (int result_expression, char* string, int line_of_string, char* file_error);

void show_info (double* x1, double* x2, enum decision quantity, struct coeff* square, FILE* note);

void clear_buffer ();

void clear_massive (char* massive);


int main (int argc, char* argv[])
{
    struct coeff square = { };

    double x1 = 0, x2 = 0;

    printf (GREEN "Программа предназначена для решения квадратного уравнения\n" RESET_COLOR);
    printf (GREEN "Квадратное уравнение представлено в виде уравнения: a*x^2+b*x+c\n" RESET_COLOR);

    printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

    char choise_out = '0';

    FILE* note = fopen ("solver_results.txt", "w");

    while (choise_out != 'q')
    {
        char argv_alternative[100];

        if (argc < 2 )
        {
            printf ("Вы не ввели mode при запуске программы\n");
            printf ("Введите mode: ");

            while (scanf ("%99s", argv_alternative) != 1)
            {
                printf ("Не успешно\n");
            }

            clear_buffer ();

            mode_choise (&x1, &x2, &square, argv_alternative, note);

            size_t i = 0;

            while (i < strlen (argv_alternative) && argv_alternative[i] != '\n')
            {
                argv_alternative[i] = '\0';
                i++;
            }
            argv_alternative[i] = '\0';

            printf ("Отправьте q, чтобы выйти из программы (другой символ для продолжения)\n" );
            choise_out = getchar ();

            clear_buffer ();
        }

        if (argc >= 2)
        {
            if ( argv[1][0] == '\0')
            {
                printf ("Введите mode\n");
                scanf ("%s", argv[1]);

                clear_buffer ();
            }

            mode_choise (&x1, &x2, &square, argv[1], note);

            size_t i = 0;

            while (i < strlen (argv[1]) && argv[1][i] != '\n')
            {
                argv[1][i] = '\0';
                i++;
            }
            argv[1][i] = '\0';

            printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

            //clear_buffer ();

            printf ("Отправьте q, чтобы выйти из программы (другой символ для продолжения)\n" );
            choise_out = getchar ();

            clear_buffer ();
        }

    }

    fclose (note);

    printf (GREEN "Принято!\n" RESET_COLOR);

    return 0;
}

int RunTests_txt (int amount, FILE* note)
{
    struct coeff square = { };

    double x1 = 0, x2 = 0;

    FILE* fp = fopen ("unit_test.txt", "r");

    if (fp == NULL)
    {
        printf (RED "Ошибка открытия файла" RESET_COLOR);

        return 1;
    }

    for (int i = 0; i < amount; i++)
    {
        printf ("Test №: %d\n", i+1);
        fscanf (fp, "%lg", &square.a);
        fscanf (fp, "%lg", &square.b);
        fscanf (fp, "%lg", &square.c);

        enum decision quantity = solve_square (&square, &x1, &x2);

        show_info (&x1, &x2, quantity, &square, note);

        double nRootsRef = 0, x1Ref = 0, x2Ref = 0;

        fscanf (fp, "%lg", &nRootsRef);
        fscanf (fp, "%lg", &x1Ref);
        fscanf (fp, "%lg", &x2Ref);

        double support_change = 0;

        if (x1Ref > x2Ref && quantity == TWO_DECISION)
        {
            support_change = x1Ref;
            x1Ref = x2Ref;
            x2Ref = support_change;
        }

        RunOneTest_txt (&square, nRootsRef, x1Ref, x2Ref);
    }

    fclose (fp);

    return 0;
}

void RunOneTest_txt (struct coeff* square, int nRootsRef, double x1Ref, double x2Ref)
{
    double x1 = 0, x2 = 0;

    int nRoots = solve_square (square, &x1, &x2);

    if (nRoots != nRootsRef || x1Ref != x1 || x2Ref != x2)
        printf (RED "Test FAILED: a = %lg, b = %lg, c = %lg\n" RESET_COLOR
                YELLOW"Expected: %d roots, x1ref = %lg, x2ref = %lg\n"
                "got:      %d roots, x1    = %lg, x2    = %lg\n" RESET_COLOR,
                (*square).a, (*square).b, (*square).c, nRootsRef, x1Ref,
                x2Ref, nRoots, x1, x2);

    else
        printf (GREEN "Success\n" RESET_COLOR);
}

void RunOneTest_rand (struct coeff* square)
{
    printf ("a =%lg\n", (*square).a);
    printf ("b =%lg\n", (*square).b);
    printf ("c =%lg\n", (*square).c);

    double x1 = 0, x2 = 0;

    int nRoots = solve_square (square, &x1, &x2);

    printf ("x1: %lg\n", x1);
    printf ("x2: %lg\n", x2);
    printf ("nRoots: %d\n", nRoots);

    if (nRoots == 0)
    {
        if ( ( (*square).a * x1 * x1 + (*square).b * x1 + (*square).c ) != 0
            && ( (*square).a * x2 * x2 + (*square).b * x2 + (*square).c ) != 0)
            printf (GREEN "Success\n" RESET_COLOR);

        else
            printf (RED "No decision, but quadratic = 0\n" RESET_COLOR);
    }

    if (nRoots == 1)
    {
        if ( ( (*square).a * x1 * x1 + (*square).b * x1 + (*square).c ) != 0 )
            printf (RED "Test FAILED: a = %lg, b = %lg, c = %lg\n"
                    "got:      %d roots, x    = %lg,\n"
                    "but x != root\n" RESET_COLOR, (*square).a, (*square).b, (*square).c, nRoots, x1);

        else
            printf (GREEN "Success\n" RESET_COLOR);
    }

    if (nRoots == 2)
    {
        if (!is_zero ( (*square).a * x1 * x1 + (*square).b * x1 + (*square).c) ||
            !is_zero ( (*square).a * x2 * x2 + (*square).b * x2 + (*square).c))
        printf (RED "Test FAILED: a = %lg, b = %lg, c = %lg\n" RESET_COLOR
                "got:      %d roots, x1    = %lg, x2    = %lg\n"
                "These roots are not a solution.\n",
                (*square).a, (*square).b, (*square).c, nRoots, x1, x2);

        else
            printf (GREEN "Success\n" RESET_COLOR);
    }

}

void RunTests_rand (int amount)  // сделать, чтобы при ошибке теста указывалось sit (srand) при котором это произошло
{//убрать ---- при тестах - некрасиво
// вывод в файлы
    srand (time (NULL));

    struct coeff square = { };

    for (int i = 0; i < amount; i++)
    {
        printf (BLUE "Test №: %d\n" RESET_COLOR, i+1);
        square.a = (double)( rand () % 20001 - 10000) /100;
        square.b = (double)( rand () % 20001 - 10000) /100;
        square.c = (double)( rand () % 20001 - 10000) /100;

        RunOneTest_rand (&square);

        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);
    }

}

void mode_choise (double* x1, double* x2, struct coeff* square, char* mode, FILE* note)
{
    coefficient_mode (x1, x2, square, mode, note);

    complete_mode (x1, x2, square, mode, note);

    test_mode (mode, note);

    help_mode (mode);

    if (strcmp (mode, "complete") != 0 && strcmp (mode, "coefficient") != 0
        && strcmp (mode, "test") != 0 && strcmp (mode, "help") != 0)
    {

        printf (RED "Вы ввели неправильный mode\n" RESET_COLOR);

        printf ("При запуске программы введите флаг компиляции help / или при выборе moda введите help\n");
    }

}

void test_mode (char* mode, FILE* note)
{
    if (strcmp (mode, "test") == 0)
    {
        printf (GREEN "Режим создан для теста программы square_main.c\n" RESET_COLOR);

        printf (YELLOW "Выберите режим, в котором вы хотите тестить\n"RESET_COLOR);

        printf (GREEN "Вариант 1: Использовать случайные коэффициенты\n"RESET_COLOR);
        printf (GREEN "Вариант 2: Использовать данные из unit_test.txt\n"RESET_COLOR);

        int choise = 0;

        while (scanf ("%d", &choise) == 0 || choise > 2 || choise < 1)
        {
            printf ("Вы ввели некорректное значение\n");
            printf (GREEN "Вариант 1: Использовать случайные коэффициенты\n"RESET_COLOR);
            printf (GREEN "Вариант 2: Использовать данные из unit_test.txt\n"RESET_COLOR);
        }

        int amount = 0;
        printf (BLUE "Введите количество тестов: " RESET_COLOR);
        scanf ("%d", &amount);

        clear_buffer ();

        switch (choise)
        {
            case 1:
            {
                RunTests_rand (amount);
                break;
            }

            case 2:
            {
                RunTests_txt (amount, note);
                break;
            }

            default:
            {
                break;
            }

        }

    }

}

void coefficient_mode (double* x1, double* x2, struct coeff* square, char* mode, FILE* note)
{
    char choise = '0';

    if (strcmp (mode, "coefficient") == 0) //сделать без strncmp - флаги с опечатками плохо
    {

        while (choise != 'q')
        {
            if (get_info (square) == 0)
            {
                printf (RED "Вы ввели некорректное значение\n" RESET_COLOR);
                break;
            }

            clear_buffer ();

            enum decision quantity = solve_square (square, x1, x2);

            show_info (x1, x2, quantity, square, note);

            printf (YELLOW "Введите q для выхода из moda (другую букву для продолжения):\n" RESET_COLOR);
            choise = getchar ();

            clear_buffer ();
        }

    }

}

void complete_mode (double* x1, double* x2, struct coeff* square, char* mode, FILE* note)
{
    char choise = '0';

    if (strcmp (mode, "complete") == 0)
    {
        char string_squarte[100]; // забить нулями --completed

        clear_massive (string_squarte);

        while (choise != 'q')
        {
            printf ("Введите уравнение в каноническом виде  \"ax*x+bx+c\" \n");

            (*square).a = 0, (*square).b = 0, (*square).c = 0;

            fgets (string_squarte, sizeof (string_squarte), stdin);

            char* right_string = corrected_string (string_squarte);

            while (sscanf (right_string, "%lg%lg%lg", &(*square).a, &(*square).b, &(*square).c) != 3 ) // УЯЗВИМО -- completed
            {
                printf (RED "Введите уравнение в правильно виде!\n" RESET_COLOR);

                printf ("Введите уравнение в каноническом ввиде  \"ax*x+bx+c\" \n");

                clear_massive (string_squarte);

                fgets (string_squarte, sizeof (string_squarte), stdin);

                char* right_string = corrected_string (string_squarte);
            }

            printf (GREEN "Успешно прочитано\n" RESET_COLOR);

            enum decision quantity = solve_square (square, x1, x2); // переименовать функцию -- completed

            show_info (x1, x2, quantity, square, note);

            printf (YELLOW "Введите q для выхода из moda (другую букву для продолжения):\n" RESET_COLOR);

            clear_massive (string_squarte);

            choise = getchar ();

            clear_buffer ();
        }

    }

}

void help_mode (char* mode)
{

    if (strcmp (mode, "help") == 0)
    {
    printf ("В этом разделе вы можете ознакомиться со списком режимов программы\n");

    printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

    printf ("Введите coefficient: ввести коэффициенты квадратного уравнения\n");
    printf ("Введите complete: ввести уравнение в каноническом виде\n");
    printf ("Введите test: запустить режим тестов программы\n");
    }

}

int get_info (struct coeff* square)
{
    hand_assert ( square != NULL, "! adress structure == NULL !", __LINE__, __FILE__);

    printf (BLUE "Введите значения для коэффициента a: " RESET_COLOR);
    if (scanf ("%lg", &(*square).a) == 0)
    {
        printf (GREEN "You are Poltoraskha meow!" RESET_COLOR);

        return 0;
    }

    printf (BLUE "Введите значения для коэффициента b: " RESET_COLOR); // vsnprintf
    if (scanf ("%lg", &(*square).b) == 0)
    {
        printf (GREEN "You are Poltoraskha meow!" RESET_COLOR);

        return 0;
    }

    printf (BLUE "Введите значения для коэффициента c: " RESET_COLOR);
    if (scanf ("%lg", &(*square).c) == 0)
    {
        printf (GREEN "You are Poltoraskha meow!" RESET_COLOR);

        return 0;
    }

    return 1;
}

int solve_square (struct coeff* square, double* x1, double* x2)
{
   // double d = rate_discriminant (square);

    double d = (*square).b * (*square).b - 4 * (*square).a * (*square).c;

    hand_assert (x1 != NULL, "! x1 == NULL !", __LINE__, __FILE__);
    hand_assert (x2 != NULL, "! x2 == NULL !", __LINE__, __FILE__);

    if (is_zero (d))
    {
        if (is_zero ( (*square).a ) != 1)
        {
            *x1 = - (*square).b / (2 * (*square).a);

            return ONE_DECISION;
        }

        if (is_zero ( (*square).a ) == 1 && is_zero ( (*square).b ) != 1)
        {
            *x1 = ( -(*square).c ) / ( (*square).b );

            return ONE_DECISION;
        }

        if (is_zero ( (*square).a ) == 1 && is_zero ( (*square).b ) == 1)
        {
            if (is_zero ( (*square).c ) == 1)
                return ENDLESSLY_DECISION;

            else
                return NO_DECISION;
        }

    }

    if (d < 0)
    {
        return NO_DECISION;
    }

    if (d > 0)
    {
        if (is_zero ( (*square).a) != 1)
        {
            *x1 = (- (*square).b - sqrt(d)) / (2 * (*square).a);
            *x2 = (- (*square).b + sqrt(d)) / (2 * (*square).a);

            return TWO_DECISION;
        }

        else
        {
            *x1 = - (*square).c / (*square).b;

            return ONE_DECISION;
        }

    }

    return NO_DECISION;
}

int is_zero (double value)
{
    if (value > -1e-5 && value < 1e-5)
        return 1;

    else
        return 0;
}

void show_info (double* x1, double* x2, enum decision quantity, struct coeff* square, FILE* note) //зачем передавать указатель корней если мы не меняем их
{

    switch (quantity)
    {
        case NO_DECISION:
        {
            printf (RED "No decision\n" RESET_COLOR);

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c);
            fprintf (note, "NO_DECISION\n");
            fprintf (note, "-------------------------------------------------------------------\n");

            break;
        }

        case ENDLESSLY_DECISION:
        {
            printf (BLUE "Endlessly decision\n" RESET_COLOR);

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c);
            fprintf (note, "ENDENSLY_DECISION\n");
            fprintf (note, "-------------------------------------------------------------------\n");

            break;
        }

        case ONE_DECISION:
        {
            printf (GREEN "X = %lg \n" RESET_COLOR, *x1);

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c);
            fprintf (note, "ONE_DECISION: x: %lg\n", *x1);
            fprintf (note, "-------------------------------------------------------------------\n");

            break;
        }

        case TWO_DECISION:
        {
            printf (GREEN "X1 = %lg\n" RESET_COLOR, *x1);
            printf (GREEN "X2 = %lg\n" RESET_COLOR, *x2);

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c);
            fprintf (note, "TWO_DECISION: x1: %lg x2: %lg\n", *x1, *x2);
            fprintf (note, "-------------------------------------------------------------------\n");

            theorem_Vieta (*x1, *x2, square);

            break;
        }

        default:
        {
            printf (RED "Error" RESET_COLOR);

            break;
        }

    }

}

void clear_buffer ()
{
    while (getchar () != '\n')
    {
        continue;
    }

}

void theorem_Vieta (double x1, double x2, struct coeff* square)
{
    if ( (*square).a != 1)
    {
        printf ("Theorem Vieta: %lg + %lg = %lg  / %lg\n", x1, x2, (*square).b * (-1), (*square).a); //если -- - плохо
        printf ("               %lg * %lg =  %lg / %lg\n", x1, x2, (*square).c, (*square).a);
    }

    else
    {
        printf ("Theorem Vieta: %lg + %lg = %lg \n", x1, x2, (*square).b * (-1)); //если -- - плохо
        printf ("               %lg * %lg = %lg \n", x1, x2, (*square).c);
    }

}

int hand_assert (int result_expression, char* string, int line_of_string, char* file_error)
{
    if (result_expression == 0)
    {
        printf (RED "ERROR %s in %d line, %s" RESET_COLOR, string, line_of_string, file_error);
        return 1;
    }

    return 0;
}

double my_pow (double square, int degree)
{
    double results = square;

    for (int i =1; i < degree; i++)
    {
        results *= square;
    }

    return results;
}

/*double rate_discriminant (struct coeff* square)
{
    double discriminant = my_pow ( (*square).b, 2) - 4 * (*square).a * (*square).c;

    return discriminant;
}*/
//пусть итоговые корни записываются в файл
void clear_massive (char* massive)
{
    size_t i;

    for ( i = 0; i < sizeof (massive) && massive[i] != '\n'; i++)
    {
        massive [i] = '\0';
    }

    if (massive[i] == '\n')
    {
        massive[i] = '\0';
    }

}

char* corrected_string (char* string_squarte)
{
    char buffer[100];

    int j = 0;

    for (size_t i = 0; i < strlen (string_squarte); i ++)
    {
        if (string_squarte[i] == ' ' || string_squarte[i] == '*' || string_squarte[i] == 'x' || string_squarte[i] == '\n')
            continue;

        buffer[j] = string_squarte[i] ;

        j++;
    }

    clear_massive (string_squarte);

    for (int i = 0; i < j; i++)
    {
        string_squarte[i] = buffer[i];
    }

    return string_squarte;
}
