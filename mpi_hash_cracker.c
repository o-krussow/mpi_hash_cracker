#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

char* charset = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890,./<>?;':";

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void decimal_to_base_k(long long int decimal_num, char* base_k_num) {
	int new_base = strlen(charset); //For readability sake 

	int index = 0;
	while (decimal_num / new_base != 0) {
		base_k_num[index] = charset[decimal_num % new_base];
		decimal_num = decimal_num / new_base;
		index++;
	}
	//Get character at index (decimal_num % new_base)
	base_k_num[index] = charset[decimal_num % new_base];

	strrev(base_k_num);

	base_k_num[index+1] = '\0';
}

void start_hashing(char* processor_name, int world_rank, int world_size, char* testpass) {
	int MAX_PASS_LENGTH = 10;
	long long int UPPER_LIMIT = pow(strlen(charset), MAX_PASS_LENGTH);

	char base_k_num[200];
	long long int decimal_counter = world_rank;

	while (decimal_counter < UPPER_LIMIT) {
		decimal_to_base_k(decimal_counter, base_k_num);

		if (decimal_counter % 10000000 == 0) {
			printf("Current guess: %s\n", base_k_num);
		} 

		if (strcmp(base_k_num, testpass) == 0) {
			printf("%s found password is %s\n", processor_name, base_k_num);
			break;
		}
		decimal_counter += world_size;
	}	


}

int main(int argc, char* argv[]) {

        MPI_Init(NULL, NULL);      // initialize MPI environment
        int world_size; // number of processes
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        int world_rank; // the rank of the process
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        char processor_name[MPI_MAX_PROCESSOR_NAME]; // gets the name of the processor
        int name_len;
        MPI_Get_processor_name(processor_name, &name_len);

		if (argc == 2) {
			start_hashing(processor_name, world_rank, world_size, argv[1]);
		}

        MPI_Finalize();

        return 0;

}
