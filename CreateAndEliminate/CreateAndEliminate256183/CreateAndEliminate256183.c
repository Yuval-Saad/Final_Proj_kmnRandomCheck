#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n_SIZE			65536 // 2^16
#define m_SIZE			256    // log(n)
#define k_SIZE			18    // loglog(n) + logloglog(n) + 7 + 1
#define MARKER_SIZE		3	  // logloglog(n)
#define ALL_POSSIBLE_KS 262144

int NOW[10] = { 1,2,4,7,13,24,44,81,149,274 };

void deepCopy(char* toWord, int indexTo, char* fromWord, int indexFrom, int howMuch) {
	for (int i = 0; i < howMuch; i++) {
		toWord[indexTo + i] = fromWord[indexFrom + i];
	}
}

void SetKMap(int* map) {
	for (int i = 0; i < ALL_POSSIBLE_KS; i++) {
		map[i] = -m_SIZE;
	}
}

int GetFirstKValue(char* w) {
	int toReturn = 0;
	for (int i = 0; i < k_SIZE; i++) {
		toReturn = (toReturn << 1);
		if (w[i] == '1')
			toReturn |= 1;
	}
	return toReturn;
}

int findPrimalKIndenticalWindow(int* i, int* j, char* w , FILE *logFile) {
	int* k_map = (int*)malloc(ALL_POSSIBLE_KS * sizeof(int));
	if (!k_map) {
		perror("Failed to allocate memory for k_map");
		exit(EXIT_FAILURE);
	}
	SetKMap((char*)k_map);
	int lastKValue = GetFirstKValue(w);
	k_map[lastKValue] = 0;
	for (int index = 1; index <= n_SIZE - k_SIZE; index++) {
		//lastKValue = lastKValue * 2;
		lastKValue = (lastKValue << 1);
		//lastKValue = lastKValue % ALL_POSSIBLE_KS;
		lastKValue &= (ALL_POSSIBLE_KS - 1);
		if (w[index + k_SIZE - 1] == '1') {
			lastKValue |= 1;
		}
		if ((index - k_map[lastKValue]) <= m_SIZE - k_SIZE) {
			*i = k_map[lastKValue];
			*j = index;
			free(k_map);
			return lastKValue;
		}
		else {
			k_map[lastKValue] = index;
		}
	}
	free(k_map);
	return 0;
}

void InsertMarker(char* w) {
	for (int i = 0; i < MARKER_SIZE; i++) {
		w[i] = '0';
	}
}


void InsertDistCode(char* w, int x,int numOfBits){
	if (numOfBits == 0) {
		return;
	}
	if (numOfBits == 1) {
		w[0] = (x == 0) ? '0' : '1';
	}
	else if (numOfBits == 2) {
		if (x == 0) {
			w[0] = '0';
			w[1] = '0';
		}
		else if (x == 1) {
			w[0] = '0';
			w[1] = '1';
		}
		else if (x == 2) {
			w[0] = '1';
			w[1] = '0';
		}
		else if(x == 3) {
			w[0] = '1';
			w[1] = '1';
		}
	}
	else {
		if (x < NOW[numOfBits - 3]) {
			w[0] = '0';
			w[1] = '0';
			w[2] = '1';
			InsertDistCode(w + 3, x, numOfBits - 3);
		}
		else if (x < (NOW[numOfBits - 3] + NOW[numOfBits - 2])) {
			w[0] = '0';
			w[1] = '1';
			InsertDistCode(w + 2, (x-NOW[numOfBits-3]), numOfBits - 2);
		}
		else {
			w[0] = '1';
			InsertDistCode(w + 1, (x - (NOW[numOfBits - 3]+NOW[numOfBits-2])), numOfBits - 1);
		}
	}
	return;
}

void BuildGfunc(char* gfunc, int distance) {
	gfunc[0] = '1';
	InsertMarker(gfunc + 1);
	gfunc[MARKER_SIZE + 1] = '1';
	gfunc[MARKER_SIZE + 2] = '0';
	gfunc[MARKER_SIZE + 3] = '1';
	InsertDistCode(gfunc + MARKER_SIZE + 4, distance , 9);
	gfunc[k_SIZE - 2] = '1';
	return;
}

void DeepCopyAndCountLen(char* toWord, char* fromWord, int howMuch, FILE* logFile) {
	int count = 0;
	for (int i = 0; i < howMuch; i++) {
		toWord[i] = fromWord[i];
		if (toWord[i] != '\0')
			count++;
	}
	fprintf(logFile, "Number of bits in the new word: %d\n", count);
}

void EliminationAlgorithm(char* inputWord, char* outputWord, FILE* logFile) {
	int i = 0;
	int j = 0;
	char w[n_SIZE];
	int value = 0;
	int counter = 1;
	int forComfort = 0;
	deepCopy(w, 0, inputWord, 0, n_SIZE);
	fprintf(logFile, "All the (i , j) couples found by order followd by the value of the window:\n");
	while ((value = findPrimalKIndenticalWindow(&i, &j, w , logFile)) !=0) {
		char neww[n_SIZE] = { 0 };
		char gfunc[k_SIZE - 1] = { 0 } ;
		fprintf(logFile, "(%d , %d) : 0x%X    ", i, j, value); // toCOmment
		BuildGfunc(gfunc, j - i); // toCOmment
		fprintf(logFile, "The Gfunc we coded: "); // toCOmment
		fwrite(gfunc, sizeof(char), k_SIZE - 1, logFile);
		fwrite("\n", sizeof(char), 1, logFile);
		if (w[j + k_SIZE] == '0' && w[j + k_SIZE + 1] == '0') {
			deepCopy(neww, 0, w, 0, j);
			deepCopy(neww, j, gfunc, 0, k_SIZE - 1);
			deepCopy(neww, j + k_SIZE - 1, w, j + k_SIZE, MARKER_SIZE + 1);
			neww[j + k_SIZE + MARKER_SIZE] = '1';
			deepCopy(neww, j + k_SIZE + MARKER_SIZE + 1, w, j + k_SIZE + MARKER_SIZE + 2, n_SIZE - (j + k_SIZE + MARKER_SIZE + 2));
			neww[n_SIZE - 1] = '\0';
		}
		else {
			deepCopy(neww, 0, w, 0, j);
			deepCopy(neww, j, gfunc, 0, k_SIZE - 1);
			deepCopy(neww, j + k_SIZE - 1, w, j + k_SIZE, n_SIZE - (j + k_SIZE));
			neww[n_SIZE - 1] = '\0';
		}
		deepCopy(w, 0, neww, 0, n_SIZE);
		if (((++forComfort) % 3) == 0)
			fprintf(logFile, "\n");
	}
	DeepCopyAndCountLen(outputWord, w, n_SIZE, logFile);
}


void CreateInputWord(char* inputWord) {
	inputWord[0] = (char)((rand() % 2) + '0');
	inputWord[1] = (char)((rand() % 2) + '0');
	for (int i = 2; i < n_SIZE; i++) {
		if (inputWord[i - 1] == '0' && inputWord[i - 2] == '0')
			inputWord[i] = '1';
		else
			inputWord[i] = (char)((rand() % 2) + '0');
	}
}

int main() {
	FILE* inputWordFile;
	FILE* outputWordFile;
	FILE* logFile;
	char inputWord[n_SIZE] = { 0 };
	char outputWord[n_SIZE] = { 0 };
	if (fopen_s(&inputWordFile, "InputWord.txt", "w+") != 0) {
		perror("Error opening InputWord.txt");
		return 1;
	}
	if (fopen_s(&outputWordFile, "OutputWord.txt", "w+") != 0) {
		perror("Error opening OutputWord.txt");
		return 1;
	}
	if (fopen_s(&logFile, "logFile.txt", "w+") != 0) {
		perror("Error opening logFile.txt");
		return 1;
	}
	srand((unsigned int)time(NULL));
	CreateInputWord(inputWord);
	fwrite(inputWord, sizeof(char), n_SIZE, inputWordFile);
	EliminationAlgorithm(inputWord, outputWord, logFile);
	fwrite(outputWord, sizeof(char), n_SIZE, outputWordFile);
	fclose(inputWordFile);
	fclose(outputWordFile);
	fclose(logFile);
	return 0;
}
