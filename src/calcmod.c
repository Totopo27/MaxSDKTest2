#include "ext.h"
#include "ext_obex.h"

typedef struct _calcmod {
    t_object obj;
    void *p_proxy1;    // Proxy for inlet 1 (sum_in2)
    void *p_proxy2;    // Proxy for inlet 2 (sub_in1)
    void *p_proxy3;    // Proxy for inlet 3 (sub_in2)
    void *p_proxy4;    // Proxy for inlet 4 (temperament)
    long sum_in1;      // First input for sum (inlet 0)
    long sum_in2;      // Second input for sum (inlet 1)
    long sub_in1;      // First input for subtraction (inlet 2)
    long sub_in2;      // Second input for subtraction (inlet 3)
    long temperament;  // Temperament value for modulo (inlet 4)
    long inlet_num;    // Which inlet is being accessed
    void *out;        // Outlet
} t_calcmod;

// Function prototypes
void *calcmod_new(void);
void calcmod_free(t_calcmod *x);
void calcmod_int(t_calcmod *x, long n);
void calcmod_assist(t_calcmod *x, void *b, long m, long a, char *s);
void calcmod_calculate(t_calcmod *x);

// Class pointer
static t_class *calcmod_class;

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("calcmod", (method)calcmod_new, (method)calcmod_free, sizeof(t_calcmod), 0L, 0);
    
    // Add methods
    class_addmethod(c, (method)calcmod_int, "int", A_LONG, 0);
    class_addmethod(c, (method)calcmod_assist, "assist", A_CANT, 0);
    
    class_register(CLASS_BOX, c);
    calcmod_class = c;
    
    post("calcmod: External loaded");
}

void *calcmod_new(void)
{
    t_calcmod *x = (t_calcmod *)object_alloc(calcmod_class);
    
    // Create proxies for additional inlets (inlets 1-4, since inlet 0 is created automatically)
    x->p_proxy1 = proxy_new((t_object *)x, 1, &x->inlet_num);  // sum_in2 (inlet 1)
    x->p_proxy2 = proxy_new((t_object *)x, 2, &x->inlet_num);  // sub_in1 (inlet 2)
    x->p_proxy3 = proxy_new((t_object *)x, 3, &x->inlet_num);  // sub_in2 (inlet 3)
    x->p_proxy4 = proxy_new((t_object *)x, 4, &x->inlet_num);  // temperament (inlet 4)
    
    // Create outlet
    x->out = intout((t_object *)x);
    
    // Initialize values
    x->sum_in1 = 0;
    x->sum_in2 = 0;
    x->sub_in1 = 0;
    x->sub_in2 = 0;
    x->temperament = 12; // Default temperament
    
    post("calcmod: New instance created with default temperament 12");
    return x;
}

void calcmod_free(t_calcmod *x)
{
    object_free(x->p_proxy1);
    object_free(x->p_proxy2);
    object_free(x->p_proxy3);
    object_free(x->p_proxy4);
    post("calcmod: Instance freed");
}

void calcmod_assist(t_calcmod *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        switch (a) {
            case 0:
                sprintf(s, "First sum input (int)");
                break;
            case 1:
                sprintf(s, "Second sum input (int)");
                break;
            case 2:
                sprintf(s, "First subtraction input (int)");
                break;
            case 3:
                sprintf(s, "Second subtraction input (int)");
                break;
            case 4:
                sprintf(s, "Temperament value (int)");
                break;
        }
    }
    else if (m == ASSIST_OUTLET) {
        sprintf(s, "Result (int)");
    }
}

// Helper function to handle modulo with negative numbers
static long proper_modulo(long n, long m) {
    long result = n % m;
    if (result < 0) {
        result += m;
    }
    return result;
}

void calcmod_int(t_calcmod *x, long n)
{
    int inlet = proxy_getinlet((t_object *)x);
    post("calcmod: Received %ld on inlet %d", n, inlet);
    
    // Store the value based on which inlet received it
    switch (inlet) {
        case 0: // First inlet (sum_in1)
            x->sum_in1 = n;
            post("calcmod: Set sum_in1 to %ld", n);
            calcmod_calculate(x); // Calculate on hot inlet
            break;
        case 1: // Second inlet (sum_in2)
            x->sum_in2 = n;
            post("calcmod: Set sum_in2 to %ld", n);
            break;
        case 2: // Third inlet (sub_in1)
            x->sub_in1 = n;
            post("calcmod: Set sub_in1 to %ld", n);
            break;
        case 3: // Fourth inlet (sub_in2)
            x->sub_in2 = n;
            post("calcmod: Set sub_in2 to %ld", n);
            break;
        case 4: // Fifth inlet (temperament)
            if (n > 0) {
                x->temperament = n;
                post("calcmod: Set temperament to %ld", n);
            } else {
                post("calcmod: Invalid temperament value %ld (must be > 0), keeping previous value: %ld", n, x->temperament);
            }
            break;
    }
}

void calcmod_calculate(t_calcmod *x)
{
    if (x->temperament <= 0) {
        post("calcmod: Error - Invalid temperament value %ld", x->temperament);
        return;
    }

    post("calcmod: Starting calculation with values:");
    post("  sum_in1 = %ld", x->sum_in1);
    post("  sum_in2 = %ld", x->sum_in2);
    post("  sub_in1 = %ld", x->sub_in1);
    post("  sub_in2 = %ld", x->sub_in2);
    post("  temperament = %ld", x->temperament);

    // Calculate sum
    long raw_sum = x->sum_in1 + x->sum_in2;
    post("  Raw sum = %ld", raw_sum);
    
    // Apply modulo to sum
    long final_result = proper_modulo(raw_sum, x->temperament);
    post("  Final result after modulo = %ld", final_result);

    // Output result
    post("calcmod: Outputting final result: %ld", final_result);
    outlet_int(x->out, final_result);
}