#include <stdio.h>
#include <stdlib.h>

#define n_SIZE			65536 // 2^16
#define k_SIZE			18    // loglog(n) + logloglog(n) + 7 + 1
#define MARKER_SIZE		3	  // logloglog(n)

int NOW[10] = { 1,2,4,7,13,24,44,81,149,274 };

void deepCopy(char* toWord, int indexTo, char* fromWord, int indexFrom, int howMuch) {
	for (int i = 0; i < howMuch; i++) {
		toWord[indexTo + i] = fromWord[indexFrom + i];
	}
}

void MoveCharsRight(char* word, int fromIndex, int distance) {
	char tmp1;
	char tmp2;
	tmp1 = word[fromIndex];
	for (int i = fromIndex; i < n_SIZE-distance; i++) {
		tmp2 = word[i + 1];
		word[i + 1] = tmp1;
		tmp1 = tmp2;
	}
}

int DecodeIndex(char* iIndexEncoded , int currnum) {
	if (currnum == 0)
		return 0;
	if (currnum == 1) {
		if (iIndexEncoded[8] == '0')
			return 0;
		else
			return 1;
	}
	if (currnum == 2) {
		if (iIndexEncoded[7] == '0' && iIndexEncoded[8] == '0')
			return 0;
		if (iIndexEncoded[7] == '0' && iIndexEncoded[8] == '1')
			return 1;
		if (iIndexEncoded[7] == '1' && iIndexEncoded[8] == '0')
			return 2;
		else // case 11
			return 3;
	}
	if (iIndexEncoded[9 - (currnum - 2)] == '1' && iIndexEncoded[9 - (currnum - 1)] == '0' && iIndexEncoded[9 - currnum] == '0') {
		return DecodeIndex(iIndexEncoded, currnum - 3);
	}
	if (iIndexEncoded[9 - (currnum - 1)] == '1' && iIndexEncoded[9 - currnum] == '0') {
		return NOW[currnum - 3] + DecodeIndex(iIndexEncoded, currnum - 2);
	}
	if (iIndexEncoded[9 - currnum] == '1') {
		return NOW[currnum - 3] + NOW[currnum - 2] + DecodeIndex(iIndexEncoded, currnum - 1);
	}

}


void Restore(char* iIndexEncoded, int jIndex, char* word) { //Jindex is also the place to set the restored word
	int dist = DecodeIndex(iIndexEncoded , 9);

	for (int i = 0; i < k_SIZE; i++) {
		word[jIndex + i] = word[jIndex - dist + (i % dist)];
	}
}

int CheckIfMarker(char* w, int index) {
	for (int i = 0; i < MARKER_SIZE; i++) {
		if (w[index + i] != '0')
			return 0;
	}
	return 1;
}

int CheckIfFlagIsUp(char* word,int index) {
	if (word[index + MARKER_SIZE + 1] == '1')
		return 1;
	return 0;
}

void DecodeRepeatedK(char* word, int index) {
	char iIndexEncoded[k_SIZE - 6 - MARKER_SIZE + 1];
	iIndexEncoded[k_SIZE - 6 - MARKER_SIZE] = '\0';
	deepCopy(iIndexEncoded, 0, word, index + MARKER_SIZE + 3, k_SIZE - 6 - MARKER_SIZE); //tofinish
	MoveCharsRight(word, index - 1, 1);
	Restore(iIndexEncoded, index - 1, word);
}

void DecodeAlgorithm2(char* word, int index) {
	int didntFind = 1;
	int i = index;
	while (didntFind) {
		if (CheckIfMarker(word, i)) {
			printf("Found Marker at index %d inside a flag\n", i);
			if (CheckIfFlagIsUp(word, i)) {
				printf("Found a flag after a flag\n");
				DecodeAlgorithm2(word, i - 1);
			}
			DecodeRepeatedK(word, i);
			didntFind = 0;
		}
		i--;
	}
}


void DecodeAlgorithm(char* word , int index) {
	int i = index;
	while (i >= 0) {
		if (CheckIfMarker(word, i)) {
			printf("Found Marker at index %d\n", i);
			if (CheckIfFlagIsUp(word, i)) {
				printf("flag Is Up\n");
				DecodeAlgorithm2(word, i - 1);
			}
			DecodeRepeatedK(word, i);
		}
		i--;
	}
}

int main() {
	FILE* EncodedWordFile;
	FILE* DecodedWordFile;
	char word[n_SIZE];

	if (fopen_s(&EncodedWordFile, "EncodedWord.txt", "r") != 0) {
		perror("Error opening EncodedWord.txt");
		return 1;
	}
	fread(word, 1, n_SIZE, EncodedWordFile);
	fclose(EncodedWordFile);

	if (fopen_s(&DecodedWordFile, "DecodedWord.txt", "w+") != 0) {
		perror("Error opening DecodedWord.txt");
		return 1;
	}
	DecodeAlgorithm(word , n_SIZE - MARKER_SIZE);
	fprintf(DecodedWordFile,"%s", word); //after is works, change to fwrite
	fclose(DecodedWordFile);

	return 0;
}
