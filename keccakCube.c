#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <dirent.h>

#define ENTROPY_BUFFER_SIZE 4096
#define NUM_THREADS 32
#define GRID_SIZE 16
#define DEPTH 16
#define ROUNDS 4095
#define HASH_SIZE 144

uint64_t rotation_offsets[GRID_SIZE][GRID_SIZE][DEPTH];
uint64_t RC[ROUNDS];

typedef struct {
    int thread_id;
    uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH];
} thread_args;

void gatherEntropy(unsigned char *buffer, size_t size);
uint64_t getRandomSliceFromEntropy();
void initializeRotationOffsets();
void initializeRoundConstants();
uint64_t rotateLeft(uint64_t x, unsigned int n);
void *generate_grid(void *arguments);
void theta(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]);
void rho(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]);
void pi(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]);
void chi(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]);
void iota(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH], int round);
void printCubeHash(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]);
void squeezeCube(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH], uint64_t squeezed[HASH_SIZE / 8]);

void gatherEntropy(unsigned char *buffer, size_t size) {
    ssize_t bytesRead = 0;

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd != -1) {
        bytesRead += read(fd, buffer, size / 2);
        close(fd);
    }

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    memcpy(buffer + bytesRead, &ts, sizeof(ts));
    bytesRead += sizeof(ts);

    const char* dmi_paths[] = {
        "/sys/class/dmi/id/board_vendor",
        "/sys/class/dmi/id/board_name",
        "/sys/class/dmi/id/board_version",
        "/sys/class/dmi/id/board_serial"
    };
    for (int i = 0; i < sizeof(dmi_paths) / sizeof(dmi_paths[0]); i++) {
        fd = open(dmi_paths[i], O_RDONLY);
        if (fd != -1) {
            bytesRead += read(fd, buffer + bytesRead, size - bytesRead);
            close(fd);
        }
    }
}

uint64_t getRandomSliceFromEntropy() {
    unsigned char entropyPool[ENTROPY_BUFFER_SIZE];
    gatherEntropy(entropyPool, sizeof(entropyPool));
    srand(time(NULL));
    int index = rand() % (ENTROPY_BUFFER_SIZE - sizeof(uint64_t));
    uint64_t *pSlice = (uint64_t *)(entropyPool + index);
    return *pSlice;
}

void initializeRotationOffsets() {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < DEPTH; z++) {
                rotation_offsets[x][y][z] = (x * 7 + y * 11 + z * 17) % 64;
            }
        }
    }
}

void initializeRoundConstants() {
    uint64_t lfsr = 0x1ULL;
    for (int i = 0; i < ROUNDS; i++) {
        uint64_t bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1;
        lfsr = (lfsr >> 1) | (bit << 62);
        RC[i] = lfsr;
    }
}

uint64_t rotateLeft(uint64_t x, unsigned int n) {
    return (x << n) | (x >> (64 - n));
}

void *generate_grid(void *arguments) {
    thread_args *args = (thread_args *)arguments;
    uint64_t seed = getRandomSliceFromEntropy();
    srand(seed);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            for (int k = 0; k < DEPTH; k++) {
                args->cube[i][j][k] = rand();
            }
        }
    }

    for (int round = 0; round < ROUNDS; round++) {
        theta(args->cube);
        rho(args->cube);
        pi(args->cube);
        chi(args->cube);
        iota(args->cube, round);
    }

    pthread_exit(NULL);
}

void theta(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]) {
    uint64_t C[GRID_SIZE][DEPTH], D[GRID_SIZE][DEPTH];
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int z = 0; z < DEPTH; z++) {
            C[x][z] = 0;
            for (int y = 0; y < GRID_SIZE; y++) {
                C[x][z] ^= cube[x][y][z];
            }
            D[x][z] = rotateLeft(C[(x-1+GRID_SIZE)%GRID_SIZE][z] ^ C[(x+1)%GRID_SIZE][z], 1);
        }
    }
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < DEPTH; z++) {
                cube[x][y][z] ^= D[x][z];
            }
        }
    }
}

void rho(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]) {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < DEPTH; z++) {
                cube[x][y][z] = rotateLeft(cube[x][y][z], rotation_offsets[x][y][z]);
            }
        }
    }
}

void pi(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]) {
    uint64_t temp[GRID_SIZE][GRID_SIZE][DEPTH];
    memcpy(temp, cube, sizeof(temp));
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < DEPTH; z++) {
                int newX = y;
                int newY = (2 * x + 3 * y + 5 * z) % GRID_SIZE;
                int newZ = (x + y + z) % DEPTH;
                cube[newX][newY][newZ] = temp[x][y][z];
            }
        }
    }
}

void chi(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]) {
    uint64_t temp[GRID_SIZE][GRID_SIZE][DEPTH];
    memcpy(temp, cube, sizeof(temp));
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < DEPTH; z++) {
                cube[x][y][z] = temp[x][y][z] ^ ((~temp[(x+1) % GRID_SIZE][y][z]) & temp[(x+2) % GRID_SIZE][y][z]);
            }
        }
    }
}

void iota(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH], int round) {
    cube[0][0][0] ^= RC[round % ROUNDS];
}

void printCubeHash(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH]) {
    for (int z = 0; z < DEPTH; z++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE; x++) {
                printf("%016lx", cube[x][y][z]);
            }
        }
    }
    printf("\n");
}

void squeezeCube(uint64_t cube[GRID_SIZE][GRID_SIZE][DEPTH], uint64_t squeezed[HASH_SIZE / 8]) {
    int index = 0;
    for (int z = 0; z < DEPTH && index < HASH_SIZE / 8; z++) {
        for (int y = 0; y < GRID_SIZE && index < HASH_SIZE / 8; y++) {
            for (int x = 0; x < GRID_SIZE && index < HASH_SIZE / 8; x++) {
                squeezed[index++] = cube[x][y][z];
            }
        }
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_args args[NUM_THREADS];
    uint64_t finalCube[GRID_SIZE][GRID_SIZE][DEPTH] = {0};
    uint64_t squeezed[HASH_SIZE / 8] = {0};

    initializeRotationOffsets();
    initializeRoundConstants();

    for (int i = 0; i < NUM_THREADS; ++i) {
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, generate_grid, (void *)&args[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    srand(time(NULL));
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (rand() % 2 == 0) {
            for (int x = 0; x < GRID_SIZE; x++) {
                for (int y = 0; y < GRID_SIZE; y++) {
                    for (int z = 0; z < DEPTH; z++) {
                        finalCube[x][y][z] ^= args[i].cube[x][y][z];
                    }
                }
            }
        }
    }

    squeezeCube(finalCube, squeezed);

    printf("Processing completed. Final squeezed bits:\n");
    for (int i = 0; i < HASH_SIZE / 8; i++) {
        printf("%016lx", squeezed[i]);
    }
    printf("\n");

    return 0;
}
