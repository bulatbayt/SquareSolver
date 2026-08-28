enum decision { NO_DECISION, ONE_DECISION, TWO_DECISION, ENDLESSLY_DECISION};

struct coeff {
    double a;
    double b;
    double c;
};

int solve_square (struct coeff* values, double* x1, double* x2);

int is_zero (double value);

double my_pow (double square, int degree);

//double rate_discriminant (struct coeff* square);

char* corrected_string (char* string_squarte);

void theorem_Vieta (double x1, double x2, struct coeff* square);
