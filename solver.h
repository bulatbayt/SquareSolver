enum desion { NO_DECISION, ONE_DECISION, TWO_DECISION, ENDLESSLY_DECISION};

struct coeff {
    double a;
    double b;
    double c;
};

int info_processing (struct coeff values, double* x1, double* x2);

int is_zero (double value);