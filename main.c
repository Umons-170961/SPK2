#include <stdio.h>
//#include <time.h>
#include <crypto_functions.h>
#include <sensor.h>
#include <gateway.h>
#include <random.h>

#include "src/parameters.h"
#include "src/math_functions.c"
#include "src/lattice_functions.c"
#include "src/print_functions.c"
#include "src/public_to_private.c"
#include "src/compute_intersection.c"
//#include "src/HNF.c"
#include "quality.c"



int main()
{

    printf("Building random generator\n");
    prepare_random();

    /*int i;
    for(i = 0; i<10; i++){
        float value = random_double(1000);
        printf("Le nombre choisis est : %d\n", (int) value);
    }*/




    int total_1 = 0;
    int total_2 = 0;
    int total_3 = 0;
    int total_pirate = 0;
    int total_g = 0;
#if defined(TEST)
    for(int count = 0; count < NBR_TEST; count++){
        srand(count);
#endif

    //Gateway
    double private_lattice_g[NBR_VECTORS][VECTOR_SIZE], public_lattice_g[NBR_VECTORS][VECTOR_SIZE];
    gateway(NBR_VECTORS, VECTOR_SIZE, private_lattice_g, public_lattice_g);

    //Sensor 1 and 2
    double intersection_1[NBR_VECTORS][VECTOR_SIZE], intersection_2[NBR_VECTORS][VECTOR_SIZE], intersection_3[NBR_VECTORS][VECTOR_SIZE];
    double private_lattice_1[NBR_VECTORS][VECTOR_SIZE], private_lattice_2[NBR_VECTORS][VECTOR_SIZE], private_lattice_3[NBR_VECTORS][VECTOR_SIZE];

    sensor(NBR_VECTORS, VECTOR_SIZE, public_lattice_g, private_lattice_1, intersection_1);
    //sensor(NBR_VECTORS, VECTOR_SIZE, intersection_1, private_lattice_2, intersection_2);
    //sensor(NBR_VECTORS, VECTOR_SIZE, intersection_2, private_lattice_3, intersection_3);


    double message[NBR_VECTORS], secret[NBR_VECTORS];
    secret_builder(NBR_VECTORS, VECTOR_SIZE, intersection_1, secret, message);


    //Decode
    double decoded_0[VECTOR_SIZE], decoded_1[VECTOR_SIZE], decoded_2[VECTOR_SIZE], decoded_3[VECTOR_SIZE], decoded_pirate[VECTOR_SIZE];

    decrypt_babai(NBR_VECTORS, VECTOR_SIZE, private_lattice_g, intersection_1, message, decoded_0);
    total_g = total_g + count_good_decrypt(NBR_VECTORS, secret, decoded_0);

    decrypt_babai(NBR_VECTORS, VECTOR_SIZE, private_lattice_1, intersection_1, message, decoded_1);
    total_1 = total_1 + count_good_decrypt(NBR_VECTORS, secret, decoded_1);

    /*decrypt(NBR_VECTORS, VECTOR_SIZE, private_lattice_2, intersection_3, message, decoded_2);
    total_2 = total_2 + count_good_decrypt(NBR_VECTORS, secret, decoded_2);

    decrypt(NBR_VECTORS, VECTOR_SIZE, private_lattice_3, intersection_3, message, decoded_3);
    total_3 = total_3 + count_good_decrypt(NBR_VECTORS, secret, decoded_3);

    decrypt(NBR_VECTORS, VECTOR_SIZE, intersection_3, intersection_3, message, decoded_pirate);
    total_pirate = total_pirate + count_good_decrypt(NBR_VECTORS, secret, decoded_pirate);*/

    print_final_output(NBR_VECTORS, secret, decoded_0, decoded_1);

    double temp[NBR_VECTORS][VECTOR_SIZE];
    //printf("Length of private lattice g : %f and the public : %f\n", gs_norm(NBR_VECTORS, VECTOR_SIZE, private_lattice_g, temp), gs_norm(NBR_VECTORS, VECTOR_SIZE, public_lattice_g, temp));
    //printf("Length of private lattice 1 : %f and the public : %f\n", gs_norm(NBR_VECTORS, VECTOR_SIZE, private_lattice_1, temp), gs_norm(NBR_VECTORS, VECTOR_SIZE, intersection_1, temp));
    //printf("Length of private lattice 2 : %f - %f and the public : %f - %f\n", gram_schimdt_norm(NBR_VECTORS, VECTOR_SIZE, private_lattice_2, intersection_2), max_norm(NBR_VECTORS, VECTOR_SIZE, private_lattice_2), gram_schimdt_norm(NBR_VECTORS, VECTOR_SIZE, intersection_2, intersection_2), max_norm(NBR_VECTORS, VECTOR_SIZE, intersection_2));

#if defined(TEST)
    }
    printf("Number of good decryption in base lattice : %d/%d\n", total_g, VECTOR_SIZE*NBR_TEST);
    printf("Number of good decryption in sensor 1 : %d/%d\n", total_1, VECTOR_SIZE*NBR_TEST);
    /*printf("Number of good decryption in sensor 2 : %d/%d\n", total_2, VECTOR_SIZE*NBR_TEST);
    printf("Number of good decryption in sensor 3 : %d/%d\n", total_3, VECTOR_SIZE*NBR_TEST);
    printf("Number of good decryption in sensor pirate : %d/%d\n", total_pirate, VECTOR_SIZE*NBR_TEST);*/
#endif

}//end main()
