/**
 * @file square_main_v2.c
 * @brief Solve Square 
 * @author Baitimerov Bulat
 * @version 1.0
 * @date 2026
 */


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

/**
 * @brief Program's main function
 * @param argc - number of command-line arguments
 * @param argv -  command-line argument array
 * @return 0 при успешном завершении
 * @return 1 - if program execution error, 0 - if program execution success
 * @details program processes command-line arguments to select the operating mode.
 * @details no mode is specified, keyboard input is requested
 * @todo При выводе я не успеваю все прочитать, нужно добавить задержку при выводе
 */
int main (int argc, char* argv[])
{
    struct coeff square = { }; // initialization of parameters to zero

    double x1 = 0, x2 = 0; // @brief x1, x2 - roots of the equations

    printf (GREEN "Программа предназначена для решения квадратного уравнения\n" RESET_COLOR);
    printf (GREEN "Квадратное уравнение представлено в виде уравнения: a*x^2+b*x+c\n" RESET_COLOR);

    printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

    char choise_out = '0';// symbol for exiting the program

    FILE* note = fopen ("solver_results.txt", "w"); // open file for recording calculations

    while (choise_out != 'q')//cycle for exit check
    {
        char argv_alternative[100];//create array for write mode, if user dont enter mode

        if (argc < 2 )
        {
            printf ("Вы не ввели mode при запуске программы\n");
            printf ("Введите mode: ");

            while (scanf ("%99s", argv_alternative) != 1)
            {
                printf ("Не успешно\n");
            }

            hand_assert (argv_alternative[0] != '\0', "ERROR Input", __LINE__ -5, __FILE__);// verify the correctness of the input.

            clear_buffer ();// clear buffer after scanf

            mode_choise (&x1, &x2, &square, argv_alternative, note);// 

            size_t i = 0;

            while (i < strlen (argv_alternative) && argv_alternative[i] != '\n') // cycle for clear array for the next entry
            {
                argv_alternative[i] = '\0';
                i++;
            }
            argv_alternative[i] = '\0';

            printf ("Отправьте q, чтобы выйти из программы (другой символ для продолжения)\n" );
            choise_out = getchar ();//input for exit check

            clear_buffer ();// clear buffer after getchar ()
        }

        if (argc >= 2)
        {
            if ( argv[1][0] == '\0')// check corretly input
            {
                printf ("Введите mode\n");
                scanf ("%s", argv[1]);
                hand_assert (argv[1][0] != '\0', "Error input", __LINE__-1, __FILE__);
                
                clear_buffer ();// clear buffer after scanf
            }

            mode_choise (&x1, &x2, &square, argv[1], note);

            size_t i = 0;

            while (i < strlen (argv[1]) && argv[1][i] != '\n')// cycle for clear array for the next entry
            {
                argv[1][i] = '\0';
                i++;
            }
            argv[1][i] = '\0';

            printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

            printf ("Отправьте q, чтобы выйти из программы (другой символ для продолжения)\n" );
            choise_out = getchar ();//input for exit check

            clear_buffer ();// clear buffer after getchar()
        }

    }

    fclose (note);// open file for recording calculations

    printf (GREEN "Принято!\n" RESET_COLOR);

    return 0;// uccessful completion of the program
}
/**
 * @brief a function to run the programs tests, taking the coefficients from a file
 * @param amount - number of read cycles
 * @param note - file write results
 * @return 1 - if program execution error, 0 - if program execution success
 */
int RunTests_txt (int amount, FILE* note)
{
    struct coeff square = { }; // initialization of parameters to zero

    double x1 = 0, x2 = 0; // @brief x1, x2 - roots of the equations

    FILE* fp = fopen ("unit_test.txt", "r");// open file for read

    if (fp == NULL)//Error open file
    {
        printf (RED "Ошибка открытия файла" RESET_COLOR);

        return 1;
    }

    for (int i = 0; i < amount; i++)
    {
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

        printf ("Test №: %d\n", i+1);
        fscanf (fp, "%lg", &square.a);//read from a file coefficient a
        fscanf (fp, "%lg", &square.b);//read from a file coefficient b
        fscanf (fp, "%lg", &square.c);//read from a file coefficient c

        enum decision quantity = solve_square (&square, &x1, &x2); // obtaining the number of solutions

        show_info (&x1, &x2, quantity, &square, note);

        double nRootsRef = 0, x1Ref = 0, x2Ref = 0;// nRootsRef - expected number of solutions, x1Ref - expected first root,x2 - expected second root

        fscanf (fp, "%lg", &nRootsRef);//read from a file nRoorsRef
        fscanf (fp, "%lg", &x1Ref);//read from a file x1Ref
        fscanf (fp, "%lg", &x2Ref);//read from a file x2Ref

        double support_change = 0;//creation of an supporting variable

        if (x1Ref > x2Ref && quantity == TWO_DECISION)// cycle for change roots in ascending order
        {
            support_change = x1Ref;
            x1Ref = x2Ref;
            x2Ref = support_change;
        }

        RunOneTest_txt (&square, nRootsRef, x1Ref, x2Ref);
    }

    fclose (fp);//close file 

    return 0;
}
/**
 * @brief function for comparing expected roots with the obtained ones
 * @param struct coeff* square - pointer to the equation coefficients structure
 * @param  nRootsRef - expected number of solutions, x1Ref - expected first root,x2 - expected second root
 */
void RunOneTest_txt (struct coeff* square, int nRootsRef, double x1Ref, double x2Ref)
{
    double x1 = 0, x2 = 0;

    int nRoots = solve_square (square, &x1, &x2); // obtain the number of solutions

    if (nRoots != nRootsRef || x1Ref != x1 || x2Ref != x2) // condition for check for coincidence roots
        printf (RED "Test FAILED: a = %lg, b = %lg, c = %lg\n" RESET_COLOR
                YELLOW"Expected: %d roots, x1ref = %lg, x2ref = %lg\n"
                "got:      %d roots, x1    = %lg, x2    = %lg\n" RESET_COLOR,
                (*square).a, (*square).b, (*square).c, nRootsRef, x1Ref,
                x2Ref, nRoots, x1, x2);

    else
        printf (GREEN "Success\n" RESET_COLOR);
}
/**
 * @brief function for comparing expected roots with the obtained ones
 * @param struct coeff* square - pointer to the equation coefficients structure
 */
void RunOneTest_rand (struct coeff* square)
{
    printf ("a =%lg\n", (*square).a);
    printf ("b =%lg\n", (*square).b);
    printf ("c =%lg\n", (*square).c);

    double x1 = 0, x2 = 0;

    int nRoots = solve_square (square, &x1, &x2);// obtain the number of solutions

    printf ("x1: %lg\n", x1);
    printf ("x2: %lg\n", x2);
    printf ("nRoots: %d\n", nRoots);

    if (nRoots == 0) // check for zero solutions
    {
        if ( ( (*square).a * x1 * x1 + (*square).b * x1 + (*square).c ) != 0
            && ( (*square).a * x2 * x2 + (*square).b * x2 + (*square).c ) != 0)
            printf (GREEN "Success\n" RESET_COLOR); // check for non-coinciding roots

        else // error case
            printf (RED "No decision, but quadratic = 0\n" RESET_COLOR);
    }

    if (nRoots == 1) // check for one solutions
    {
        if ( ( (*square).a * x1 * x1 + (*square).b * x1 + (*square).c ) != 0 )
            printf (RED "Test FAILED: a = %lg, b = %lg, c = %lg\n"
                    "got:      %d roots, x    = %lg,\n"
                    "but x != root\n" RESET_COLOR, (*square).a, (*square).b, (*square).c, nRoots, x1);// check for non-coinciding roots

        else// Success case
            printf (GREEN "Success\n" RESET_COLOR);
    }

    if (nRoots == 2)// check for two solutions
    {
        if (!is_zero ( (*square).a * x1 * x1 + (*square).b * x1 + (*square).c) ||
            !is_zero ( (*square).a * x2 * x2 + (*square).b * x2 + (*square).c))
        printf (RED "Test FAILED: a = %lg, b = %lg, c = %lg\n" RESET_COLOR
                "got:      %d roots, x1    = %lg, x2    = %lg\n"
                "These roots are not a solution.\n",
                (*square).a, (*square).b, (*square).c, nRoots, x1, x2);// check for non-coinciding roots

        else//Success case
            printf (GREEN "Success\n" RESET_COLOR);
    }

}

/**
 * @brief function for generating random coefficients and the launch of the verification function
 * @param amount - number of tests
 */
void RunTests_rand (int amount)  
{
    srand (time (NULL));

    struct coeff square = { };

    for (int i = 0; i < amount; i++) // cycle for receipt random coefficients 
    {
        printf (BLUE "Test №: %d\n" RESET_COLOR, i+1);
        square.a = (double)( rand () % 20001 - 10000) /100;
        square.b = (double)( rand () % 20001 - 10000) /100;
        square.c = (double)( rand () % 20001 - 10000) /100;

        RunOneTest_rand (&square);

        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);
    }

}

/**
 * @brief function to process the entered mod
 * @param struct coeff* square - pointer to the equation coefficients structure
 * @param double* x1, double* x2 - pointers to the address where the roots of the equation are located
 * @param char* mode - input mode
 * @param FILE* note - file for write results 
 */
void mode_choise (double* x1, double* x2, struct coeff* square, char* mode, FILE* note) 
{
    coefficient_mode (x1, x2, square, mode, note);

    complete_mode (x1, x2, square, mode, note);

    test_mode (mode, note);

    help_mode (mode);

    if (strcmp (mode, "complete") != 0 && strcmp (mode, "coefficient") != 0
        && strcmp (mode, "test") != 0 && strcmp (mode, "help") != 0) // in case none of the modes worked
    {

        printf (RED "Вы ввели неправильный mode\n" RESET_COLOR);

        printf ("При запуске программы введите флаг компиляции help / или при выборе moda введите help, чтобы ознакомиться со списком возможностей\n");
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);
    }

}

/**
 * @brief function for test mode
 * @param char* mode - input mode
 * @param FILE* note - file for write results 
 */
void test_mode (char* mode, FILE* note)
{
    if (strcmp (mode, "test") == 0) // check for mode match
    {
        printf (GREEN "Режим создан для теста программы square_main.c\n" RESET_COLOR);

        printf (YELLOW "Выберите режим, в котором вы хотите тестить\n"RESET_COLOR);

        printf (GREEN "Вариант 1: Использовать случайные коэффициенты\n"RESET_COLOR);
        printf (GREEN "Вариант 2: Использовать данные из unit_test.txt\n"RESET_COLOR);

        int choise = 0; // variable for launching test mode

        while (scanf ("%d", &choise) == 0 || choise > 2 || choise < 1) // input validation condition
        {
            printf ("Вы ввели некорректное значение\n");
            printf (GREEN "Вариант 1: Использовать случайные коэффициенты\n"RESET_COLOR);
            printf (GREEN "Вариант 2: Использовать данные из unit_test.txt\n"RESET_COLOR);
        }

        int amount = 0; // variable for the number of tests
        printf (BLUE "Введите количество тестов: " RESET_COLOR);
        scanf ("%d", &amount);
        hand_assert (amount >= 0, "Error Input amount tests", __LINE__ - 1, __FILE__); // check for correctly variable

        clear_buffer (); // clear buffer after scanf 

        switch (choise) // launch test mode case 
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
/**
 * @brief function for entering coefficients individually
 * @param struct coeff* square - pointer to the equation coefficients structure
 * @param double* x1, double* x2 - pointers to the address where the roots of the equation are located
 * @param char* mode - input mode
 * @param FILE* note - file for write results 
 */
void coefficient_mode (double* x1, double* x2, struct coeff* square, char* mode, FILE* note)
{
    char choise = '0'; // symbol for exiting the program

    if (strcmp (mode, "coefficient") == 0) // check for mode match
    {

        while (choise != 'q') // cycle for exit check
        {
            if (get_info (square) == 0) // condition for correctly input
            {
                printf (RED "Вы ввели некорректное значение\n" RESET_COLOR);
                break;
            }

            clear_buffer (); // clear buffer after function get_info

            enum decision quantity = solve_square (square, x1, x2); // obtaining the number of solutions

            show_info (x1, x2, quantity, square, note);

            printf (YELLOW "Введите q для выхода из moda (другую букву для продолжения):\n" RESET_COLOR);
            choise = getchar (); // input for exit check

            clear_buffer (); // clear buffer after getchar ()
        }

    }

}

/**
 * @brief function for entering the entire equation
 * @param struct coeff* square - pointer to the equation coefficients structure
 * @param double* x1, double* x2 - pointers to the address where the roots of the equation are located
 * @param char* mode - input mode
 * @param FILE* note - file for write results 
 */
void complete_mode (double* x1, double* x2, struct coeff* square, char* mode, FILE* note)
{
    char choise = '0'; // symbol for exiting the program

    if (strcmp (mode, "complete") == 0) // check for mode match
    {
        char string_squarte[100]; // array for equation input

        clear_massive (string_squarte); // clear string

        while (choise != 'q') // cycle for exit check
        {
            printf ("Введите уравнение в каноническом виде  \"ax*x+bx+c\" \n");

            (*square).a = 0, (*square).b = 0, (*square).c = 0;

            fgets (string_squarte, sizeof (string_squarte), stdin); // input equation

            char* right_string = corrected_string (string_squarte); // obtaining a string in the correct format
            hand_assert (right_string != NULL, "ERROR NULL adress", __LINE__ -1, __FILE__); // check pointer

            while (sscanf (right_string, "%lgxx%lgx%lg", &(*square).a, &(*square).b, &(*square).c) != 3 )  // check correct input
            {
                printf (RED "Введите уравнение в правильно виде!\n" RESET_COLOR);

                printf ("Введите уравнение в каноническом ввиде  \"ax*x+bx+c\" \n");

                clear_massive (string_squarte);

                fgets (string_squarte, sizeof (string_squarte), stdin);

                char* right_string = corrected_string (string_squarte);
                hand_assert (right_string != NULL, "ERROR NULL adress", __LINE__ -1, __FILE__);
            }

            printf (GREEN "Успешно прочитано\n" RESET_COLOR);

            enum decision quantity = solve_square (square, x1, x2); // obtaining the number of solutions

            show_info (x1, x2, quantity, square, note);

            printf (YELLOW "Введите q для выхода из moda (другую букву для продолжения):\n" RESET_COLOR);

            clear_massive (string_squarte); // clear array

            choise = getchar (); // input for exit check

            clear_buffer (); // clear buffer after getchar()
        }

    }

}

/**
 * @brief function for show operations
 * @param char* mode - input mode 
 */
void help_mode (char* mode)
{

    if (strcmp (mode, "help") == 0) // check for mode match
    {
    printf ("В этом разделе вы можете ознакомиться со списком режимов программы\n");

    printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

    printf ("Введите coefficient: ввести коэффициенты квадратного уравнения\n");
    printf ("Введите complete: ввести уравнение в каноническом виде\n");
    printf ("Введите test: запустить режим тестов программы\n");
    }

}

/**
 * @brief function for get information
 * @param struct coeff* square - pointer to the equation coefficients structure 
 * @return 1 - if program execution success, 0 - if program execution error
 */
int get_info (struct coeff* square)
{
    hand_assert ( square != NULL, "! adress structure == NULL !", __LINE__, __FILE__);

    printf (BLUE "Введите значения для коэффициента a: " RESET_COLOR);
    if (scanf ("%lg", &(*square).a) == 0) // cycle for correct input cpefficient a
    {
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);
        printf (GREEN "You are Poltoraskha meow!\n" RESET_COLOR);
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

        return 0;
    }

    printf (BLUE "Введите значения для коэффициента b: " RESET_COLOR); // vsnprintf
    if (scanf ("%lg", &(*square).b) == 0)// cycle for correct input coefficient b
    {
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);
        printf (GREEN "You are Poltoraskha meow!\n" RESET_COLOR);
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

        return 0;
    }

    printf (BLUE "Введите значения для коэффициента c: " RESET_COLOR);
    if (scanf ("%lg", &(*square).c) == 0)
    {
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);
        printf (GREEN "You are Poltoraskha meow!\n" RESET_COLOR);
        printf (BLUE "-------------------------------------------------------------------\n" RESET_COLOR);

        return 0;
    }

    return 1;
}
/**
 * @brief function to obtain the number of solutions and calculate the roots
 * @param struct coeff* square - pointer to the equation coefficients structure
 * @param double* x1, double* x2 - pointers to the address where the roots of the equation are located
 * @return ONE_DECISION - if equation has one root.
 * @return TWO_DECISION - if equation has two roots
 * @return ENDLESSLY_DESICION - if equation has endlessly roots.
 * @return NO_DESICION - if equation doesnt have roots
 */
int solve_square (struct coeff* square, double* x1, double* x2)
{
    double d = (*square).b * (*square).b - 4 * (*square).a * (*square).c; // d - discriminant 

    hand_assert (x1 != NULL, "! x1 == NULL !", __LINE__, __FILE__); // NULL pointer check
    hand_assert (x2 != NULL, "! x2 == NULL !", __LINE__, __FILE__); // NULL pointer check

    if (is_zero (d)) // null check d
    {
        if (is_zero ( (*square).a ) != 1) // non-nullity check coefficient a
        {
            *x1 = - (*square).b / (2 * (*square).a); // calculation first root

            return ONE_DECISION;
        }

        if (is_zero ( (*square).a ) == 1 && is_zero ( (*square).b ) != 1)  // null check coefficient a and non-nullity check coefficient b
        {
            *x1 = ( -(*square).c ) / ( (*square).b ); // calculation first root

            return ONE_DECISION;
        }

        if (is_zero ( (*square).a ) == 1 && is_zero ( (*square).b ) == 1) // null check coefficient coefficient a and null check coefficient coefficient b 
        {
            if (is_zero ( (*square).c ) == 1) // null check coefficient c
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
        if (is_zero ( (*square).a) != 1) // non-nullity check coefficient a
        {
            *x1 = (- (*square).b - sqrt(d)) / (2 * (*square).a); // calculation first root
            *x2 = (- (*square).b + sqrt(d)) / (2 * (*square).a); // calculation second root

            return TWO_DECISION;
        }

        else
        {
            *x1 = - (*square).c / (*square).b; // calculation first root

            return ONE_DECISION;
        }

    }

    return NO_DECISION;
}

/**
 * @brief function for comparing a number with a range close to zero
 * @param double value - number being compared 
 * @return 1 - if value = zero, 0 - if value != 0
 */
int is_zero (double value)
{
    if (value > -1e-5 && value < 1e-5) // comparison with a range that is very close to zero
        return 1;

    else
        return 0;
}

/**
 * @brief function to process the entered mod
 * @param double* x1, double* x2 - pointers to the address where the roots of the equation are located
 * @param enum decision quantity - set of constants for the number of solutions
 * @param struct coeff* square - pointer to the equation coefficients structure
 * @param FILE* note - file for write results 
 */
void show_info (double* x1, double* x2, enum decision quantity, struct coeff* square, FILE* note) 
{
    switch (quantity) // cases of number of solutions
    {
        case NO_DECISION:
        {
            printf (RED "No decision\n" RESET_COLOR); // conslution to an output

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c); // writing to a file
            fprintf (note, "NO_DECISION\n");
            fprintf (note, "-------------------------------------------------------------------\n");

            break;
        }

        case ENDLESSLY_DECISION:
        {
            printf (BLUE "Endlessly decision\n" RESET_COLOR); // conslution to an output

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c); // writing to a file
            fprintf (note, "ENDENSLY_DECISION\n");
            fprintf (note, "-------------------------------------------------------------------\n");

            break;
        }

        case ONE_DECISION:
        {
            printf (GREEN "X = %lg \n" RESET_COLOR, *x1); // conslution to an output

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c); // writing to a file
            fprintf (note, "ONE_DECISION: x: %lg\n", *x1);
            fprintf (note, "-------------------------------------------------------------------\n");

            break;
        }

        case TWO_DECISION:
        {
            printf (GREEN "X1 = %lg\n" RESET_COLOR, *x1); // conslution to an output
            printf (GREEN "X2 = %lg\n" RESET_COLOR, *x2); 

            fprintf (note, "a: %lg b: %lg c: %lg\n", (*square).a, (*square).b, (*square).c); // writing to a file
            fprintf (note, "TWO_DECISION: x1: %lg x2: %lg\n", *x1, *x2);
            fprintf (note, "-------------------------------------------------------------------\n");

            theorem_Vieta (*x1, *x2, square); // conslution uotput theorem vieta

            break;
        }

        default:
        {
            printf (RED "Error" RESET_COLOR);

            break;
        }

    }

}

/**
 * @brief function for clear buffer 
 */
void clear_buffer ()
{
    while (getchar () != '\n') // cyle for find symbol '\n'
    {
        continue;
    }

}

/**
 * @brief function for show theorem vieta
 * @param x1 - first root equation
 * @param x2 - second root equation
 * @param struct coeff* square - pointer to the equation coefficients structure
 */
void theorem_Vieta (double x1, double x2, struct coeff* square)
{
    if ( (*square).a != 1) // non-nullity check coefficient a
    {
        printf ("Theorem Vieta: %lg + %lg = %lg  / %lg\n", x1, x2, (*square).b * (-1), (*square).a); 
        printf ("               %lg * %lg =  %lg / %lg\n", x1, x2, (*square).c, (*square).a);
    }

    else
    {
        printf ("Theorem Vieta: %lg + %lg = %lg \n", x1, x2, (*square).b * (-1)); 
        printf ("               %lg * %lg = %lg \n", x1, x2, (*square).c);
    }

}

/**
 * @brief function for condition check
 * @param int result_expression result of the expression
 * @param char* string - output string
 * @param int line_of_string - error string
 * @param char* file_error - error file
 * @return 1 - if result of the expression is false
 * @return 0 - if result of the expression is true
 */
int hand_assert (int result_expression, char* string, int line_of_string, char* file_error)
{
    if (result_expression == 0)
    {
        printf (RED "ERROR %s in %d line, %s" RESET_COLOR, string, line_of_string, file_error);
        return 1;
    }

    return 0;
}

/**
 * @brief function for calculating the power of a number
 * @param double square - the number being raised to a power
 * @param int degree - the power to which a number is raised
 * @return calculation result
 */

double my_pow (double square, int degree)
{
    double results = square;

    for (int i =1; i < degree; i++)
    {
        results *= square;
    }

    return results;
}

/**
 * @brief function for clear array
 * @param char* massive - array for cleaning
 */
void clear_massive (char* massive)
{
    size_t i;

    for ( i = 0; i < strlen (massive) && massive[i] != '\n'; i++)
    {
        massive [i] = '\0';
    }

    if (massive[i] == '\n')
    {
        massive[i] = '\0';
    }

}

/**
 * @brief function for adjusting an array
 * @param char* string_squarte - the line that needs to be adjusted
 * return pointer on correct string
 */
char* corrected_string (char* string_squarte)
{
    char buffer[100]; // support array
    int j = 0;

    for (size_t i = 0; i < strlen (string_squarte); i ++) // cycle for deleted excess symbols
    {
        if (string_squarte[i] == ' ' || string_squarte[i] == '*' || string_squarte[i] == '\n')
            continue;

        buffer[j] = string_squarte[i] ;

        j++;
    }

    clear_massive (string_squarte); // clear array

    int i;
    for (i = 0; i < j; i++) // cyccle to return the correct string
    {
        string_squarte[i] = buffer[i];
    }

    string_squarte[i] = '\0';

    return string_squarte;
}
