/*Sub-Gaussian Random Variables*/

#include <stdio.h>
#include <math.h>

// Gaussian Tails and MGF
double gaussian_tail(double sigma, double x, double mu){
    // Ensure sigma is positive
    if(sigma <= 0){
        return -1; // Return an error value.
    }
    double den = sqrt(2 * M_PI * pow(sigma, 2.0)); // sqrt of var term
    double exp_frac = exp(-pow((x-mu), 2.0) / (2 * pow(sigma, 2.0))); // exp term
    double ProbX = (1 / den) * exp_frac;
    return ProbX; // combined terms 
}

int main(){
    double mu = 0.0;
    double sigma = 1.0;
    double x = 1.0;
    double result = gaussian_tail(sigma, x, mu);

    printf("mu = %f\n", mu);
    printf("sigma = %f\n", sigma);
    printf("X = %f\n", x);

    if(result != -1){
        printf("Gaussian Probability: %f\n", result);
    } else{
        printf("Error: Invalid Parameters\n");
    }

    return 0;
}
