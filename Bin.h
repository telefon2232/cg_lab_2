#pragma once
#include <iostream>

using namespace std;

class Bin{
public:
	int X, Y, Z;
	short *array;
	Bin();
	~Bin();
	void readBIN(string path) {
		FILE *file;
		const char *path_char = path.c_str();
		file = fopen(path_char, "rb");
		if (file) {
			fread(&X, sizeof(int32_t), 1, file);
			fread(&Y, sizeof(int32_t), 1, file);
			fread(&Z, sizeof(int32_t), 1, file);
			int array_size = X * Y * Z;
			array = new short[array_size];//То же самое, что и int16_t
			for (int i = 0; i < array_size; i++) {
				fread(array + i, sizeof(int16_t), 1, file);
			}
		}
		else {
			cout << "File doesn't exist!!!!" << endl;
		}
		fclose(file);
	}
};







