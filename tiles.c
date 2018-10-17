#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint64_t u64;

u64 powi(u64 base, u64 exponent) {
	u64 result = 1;
	for(u64 i = 0; i < exponent; i++) result *= base;
	return result;
}

typedef struct {
	u8 data[SIZE][SIZE];
} pattern;

#if STATES == 2
	const int permutationCount = 2;
	const u8 permutations[permutationCount][STATES] = {
		{0, 1},
		{1, 0}
	};
#elif STATES == 3
	const int permutationCount = 6;
	const u8 permutations[permutationCount][STATES] = {
		{0, 1, 2},
		{0, 2, 1},
		{1, 0, 2},
		{2, 0, 1},
		{1, 2, 0},
		{2, 1, 0}
	};
#else
	#error Unsupported state count
#endif

pattern patternFromNumber(u64 patternNumber) {
	pattern result;
	for(int i = SIZE - 1; i >= 0; i--) {
		for(int j = SIZE - 1; j >= 0; j--) {
			result.data[i][j] = patternNumber % STATES;
			patternNumber /= STATES;
		}
	}
	return result;
}

u64 patternNumber(pattern pattern) {
	u64 result = 0;
	for(int i = 0; i < SIZE; i++) {
		for(int j = 0; j < SIZE; j++) {
			result *= STATES;
			result += pattern.data[i][j];
		}
	}
	return result;
}

bool patternEqual(pattern x, pattern y) {
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			if(x.data[i][j] != y.data[i][j])
				return false;
	return true;
}

pattern right(pattern x) {
	pattern result;
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			result.data[(i + 1) % SIZE][j] = x.data[i][j];
	return result;
}

pattern down(pattern x) {
	pattern result;
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			result.data[i][(j + 1) % SIZE] = x.data[i][j];
	return result;
}

pattern transpose(pattern x) {
	pattern result;
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			result.data[j][i] = x.data[i][j];
	return result;
}

pattern reflect(pattern x) {
	pattern result;
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			result.data[SIZE - 1 - i][j] = x.data[i][j];
	return result;
}

pattern rotate(pattern x) {
	return reflect(transpose(x));
}

pattern permute(pattern x, int permutationNumber) {
	pattern result;
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			result.data[i][j] = permutations[permutationNumber][x.data[i][j]];
	return result;
}

void printPattern(pattern x) {
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			putchar('0' + (STATES - 1 - x.data[i][j]));
	putchar('\n');
}

#ifdef brick
	#if (SIZE & 1) == 1
		#error Brick symmetry only works with even sizes
	#endif
	u64 initialPatternCount() {
		return powi(STATES, SIZE * SIZE / 2);
	}

	pattern initialPatternNumber(u64 initialPatternNumber) {
		pattern result;
		for(int i = (SIZE / 2) - 1; i >= 0; i--) {
			for(int j = SIZE - 1; j >= 0; j--) {
				result.data[i][j] = result.data[SIZE / 2 + i][(j + SIZE / 2) % SIZE] = initialPatternNumber % STATES;
				initialPatternNumber /= STATES;
			}
		}
		return result;
	}
#else
	u64 initialPatternCount() {
		return powi(STATES, SIZE * SIZE);
	}

	pattern initialPatternNumber(u64 initialPatternNumber) {
		return patternFromNumber(initialPatternNumber);
	}
#endif

const bool doRight = true;
const bool doDown = true;
const bool doRotate = true;
const bool doTranspose = true;
const bool doReflect = true;
const bool doPermutations = true;

const int rightIterations = doRight ? SIZE : 1;
const int downIterations = doDown ? SIZE : 1;
const int rotateIterations = doRotate ? 4 : 1;
const int transposeIterations = doTranspose ? 2 : 1;
const int reflectIterations = doReflect ? 2 : 1;
const int permutationIterations = doPermutations ? permutationCount : 1;

const int iterations = rightIterations * downIterations * rotateIterations * transposeIterations * reflectIterations * permutationIterations;

u8 *alreadySeen;

bool hasAlreadySeen(pattern pattern) {
	u64 thisPatternNumber = patternNumber(pattern);
	return alreadySeen[thisPatternNumber / 8] & (1 << (thisPatternNumber % 8));
}

void recordSeen(pattern pattern) {
	u64 thisPatternNumber = patternNumber(pattern);
	alreadySeen[thisPatternNumber / 8] |= (1 << (thisPatternNumber % 8));
}

int main() {
	u64 patternCount = powi(STATES, SIZE * SIZE);
	alreadySeen = calloc(patternCount / 8 + 1 /* Round up */, sizeof(u8));
	int j, k, l, m, n, o;
	pattern a, b, c, d, e, f;
	pattern equivalenceClass[iterations];
	for(u64 i = 0; i < initialPatternCount(); i++)	{
		int equivalenceClassCounter = 0;
		for(j = 0, a = initialPatternNumber(i); j < rotateIterations; j++, a = doRotate ? rotate(a) : a) {
			for(k = 0, b = a; k < transposeIterations; k++, b = doTranspose ? transpose(b) : b) {
				for(l = 0, c = b; l < reflectIterations; l++, c = doReflect ? reflect(c) : c) {
					for(m = 0, d = c; m < rightIterations; m++, d = doRight ? right(d) : d) {
						for(n = 0, e = d; n < downIterations; n++, e = doDown ? down(e) : e) {
							for(o = 0, f = e; o < permutationIterations; o++, f = doPermutations ? permute(e, o % permutationIterations) : e) {
								if(hasAlreadySeen(f)) {
									assert(patternEqual(f, initialPatternNumber(i)));
									goto nextNumber;
								}
								equivalenceClass[equivalenceClassCounter++] = f;
							}
							assert(patternEqual(f, e));
						}
						assert(patternEqual(e, d));
					}
					assert(patternEqual(d, c));
				}
				assert(patternEqual(c, b));
			}
			assert(patternEqual(b, a));
		}
		assert(patternEqual(a, initialPatternNumber(i)));

		int uniqueCount = 0;
		for(j = 0; j < iterations; j++) {
			recordSeen(equivalenceClass[j]);
			for(k = 0; k < j; k++) {
				if(patternEqual(equivalenceClass[j], equivalenceClass[k])) goto isRepeated;
			}
			uniqueCount++;
			isRepeated:;
		}
		printf("%d ", uniqueCount);
		printPattern(equivalenceClass[0]);

		nextNumber:;
	}
	return 0;
}
