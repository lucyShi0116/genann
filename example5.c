#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "genann.h"

/* This example is to illustrate how to use GENANN.
 * It is NOT an example of good machine learning techniques.
 */

const char *simulate_data = "example/simulate.csv";

double *input, *class;
int samples;
const char *class_names[] = {"ON", "OFF"};

void load_data() {
    /* Load the simulate data-set. */
    FILE *in = fopen("example/simulate.csv", "r");
    if (!in) {
        printf("Could not open file: %s\n", simulate_data);
        exit(1);
    }

    /* Loop through the data to get a count. */
    char line[1024];
    while (!feof(in) && fgets(line, 1024, in)) {
        ++samples;
    }
    fseek(in, 0, SEEK_SET);

    printf("Loading %d data points from %s\n", samples, simulate_data);

    /* Allocate memory for input and output data. */
    input = malloc(sizeof(double) * samples * 3);
    class = malloc(sizeof(double) * samples * 2);

    /* Read the file into our arrays. */
    int i, j;
    for (i = 0; i < samples; ++i) {
        double *p = input + i * 3;
        double *c = class + i * 2;
        c[0] = c[1] = 0.0;

        if (fgets(line, 1024, in) == NULL) {
            perror("fgets");
            exit(1);
        }

        char *split = strtok(line, ",");
        for (j = 0; j < 3; ++j) {
            p[j] = atof(split);
            split = strtok(0, ",");
        }

        split[strlen(split)-2] = 0;
        if (strcmp(split, class_names[0]) == 0) {c[0] = 1.0;}
        else if (strcmp(split, class_names[1]) == 0) {c[1] = 1.0;}
        else {
            printf("Unknown class %s.\n", split);
            exit(1);
        }

        /* printf("Data point %d is %f %f %f %f  ->   %f %f %f\n", i, p[0], p[1], c[0], c[1]); */
    }

    fclose(in);
}


int main(int argc, char *argv[])
{
    printf("GENANN example 5.\n");
    printf("Train an ANN on the dataset using backpropagation.\n");

    srand(time(0));

    /* Load the data from file. */
    load_data();

    /* 3 inputs: weekday(Monday~Friday), hour(0~23), minute(0~59).
     * 1 hidden layer(s) of 4 neurons.
     * 2 outputs (1 per class): ON, OFF
     */
    genann *ann = genann_init(3, 1, 3, 2);

    int i, j;
    int loops = 5000;

    /* Train the network with backpropagation. */
    printf("Training for %d loops over data.\n", loops);
    for (i = 0; i < loops; ++i) {
        for (j = 0; j < (samples*2/3); ++j) {
            genann_train(ann, input + j*3, class + j*2, .01);
        }
        /* printf("%1.2f ", xor_score(ann)); */
    }

    int correct = 0;
    for (j = samples*2/3; j < samples; ++j) {
        const double *guess = genann_run(ann, input + j*3);
        // printf("j:%d, guess[0]:%f, guess[1]:%f\n", j, guess[0], guess[1]);
        if (class[j*2+0] == 1.0) {if (guess[0] > guess[1]) ++correct;}
        else if (class[j*2+1] == 1.0) {if (guess[1] > guess[0]) ++correct;}
        else {printf("Logic error.\n"); exit(1);}
    }

    printf("%d/%d correct (%0.1f%%).\n", correct, samples/3, (double)correct / samples * 3 * 100.0);


    genann_free(ann);
    free(input);
    free(class);

    return 0;
}
