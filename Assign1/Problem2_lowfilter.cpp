#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>

#define size 256

using namespace std;

int main(int argc, char **argv){
	FILE *file_i[2];
	FILE *file_o;
	FILE *data_o;
	file_i[0] = fopen("sample3.raw", "rb");
	file_i[1] = fopen(argv[1], "rb");
	file_o = fopen(argv[2], "w");
	data_o = fopen("Problem2_lowfilter.csv", "w");
	if (file_i[0] == NULL) cout << "Cannot open the reading file!" << endl;
	if (file_i[1] == NULL) cout << "Cannot open the reading file!" << endl;
	if (file_o == NULL) cout << "Cannot open the writing file!" << endl;	
	if (data_o == NULL) cout << "Cannot open the writing file!" << endl;	

	unsigned char ori[size][size];
	fread(ori, sizeof(unsigned char), size * size, file_i[0]);
	fclose(file_i[0]);

	unsigned char img[size][size];
	unsigned char tmp[size][size];
	fread(img, sizeof(unsigned char), size * size, file_i[1]);
	fclose(file_i[1]);

	double max = 0.0;
	for (int b = 1; b < 21; b ++){
		int hlf = 1;
		int flt[3][3] = {{0}};
		flt[0][0] = flt[0][2] = flt[2][0] = flt[2][2] = 1;
		flt[0][1] = flt[1][0] = flt[1][2] = flt[2][1] = b;
		flt[1][1] = b * b;

		for (int i = 0; i < size; i ++)
			for (int j = 0; j < size; j ++){
				int ara = 0, sum = 0;
				for (int m = i - hlf; m <= i + hlf; m ++)
					for (int n = j - hlf; n <= j + hlf; n ++){
						if (m < 0 || m > size - 1) continue;
						if (n < 0 || n > size - 1) continue;

						sum += img[m][n] * flt[m - i + 1][n - j + 1];
						ara += flt[m - i + 1][n - j + 1];
					}

				tmp[i][j] = sum / ara;
			}

		double tot = 0, psnr = 0;
		for (int i = 0; i < 256; i ++)
			for (int j = 0; j < 256; j ++)
				tot += pow(ori[i][j] - tmp[i][j], 2);
		tot = tot / size / size;
		psnr = 10 * log10(255 * 255 / tot);
		fprintf(data_o, "%d,%f\n", b, psnr);

		if (psnr > max){
			max = psnr;
			fprintf(stderr, "%d, %f\n", b, psnr);
			fseek(file_o, 0, SEEK_SET);
			fwrite(tmp, sizeof(unsigned char), size * size, file_o);
		}
	}

	fclose(file_o);
	fclose(data_o);

	return 0;
}