#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <initializer_list>
#include <limits>
#include <vector>
#include <tuple>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::vector;
using std::tuple;
using std::tie;
using std::make_tuple;

#include "io.h"
#include "matrix.h"

typedef tuple<uint, uint, uint, uint> Rect;

tuple<vector<Rect>, Image>
find_treasure(const Image& in)
{
    // Base: return Rect of treasure only
    // Bonus: return Rects of arrows and then treasure Rect
    auto path = vector<Rect>();
    return make_tuple(path, in.deep_copy());
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Usage: " << endl << argv[0]
             << " <in_image.bmp> <out_image.bmp> " << endl;
        return 0;
    }

    try {
        Image img = load_image(argv[1]);
        Image img2 = load_image(argv[1]);
       // ofstream fout(argv[3]);
		Matrix<uint> binary_map = Matrix<uint>(img.n_rows, img.n_cols);
		Matrix<uint> object_map = Matrix<uint>(img.n_rows, img.n_cols);
		uint r,g,b;
		double all = 0;

		for (uint i = 0; i < img.n_rows; i++) // Calculation of average color
		for (uint j = 0; j < img.n_cols; j++)
		{
			tie(r, g, b) = img(i, j);
			all = all + (r + g + b)/3;
		}

		all = all/(img.n_rows*img.n_cols);
	//	cout << all << endl;

		if (all >= 180) // for light pictures
		for (uint i = 0; i < img.n_rows; i++)
		for (uint j = 0; j < img.n_cols; j++)
		{
			tie(r, g, b) = img(i,j);
			if ((r <= 220) && (g <= 220) && (b <= 220))
			{
				r = (r*r) / 255; g = (g*g) / 255; b = (b*b) / 255;
				r = (r*r) / 255; g = (g*g) / 255; b = (b*b) / 255;
				img(i, j) = make_tuple(r, g, b);
			}
		};
		
		for (uint i = 0; i < img.n_rows; i++)
		for (uint j = 0; j < img.n_cols; j++)
		{
			tie(r, g, b) = img(i,j);
			if (abs(r-g) != abs(g-b))// > 1) && (abs(g-b) > 1) && (abs(r-b) > 1))
				img(i, j) = make_tuple(150, 150, 150);
		}

		int count = 0;
		uint min_i = 0;
		uint min_j = 0;
		int gh = 0;
		uint max_i = 0;
		uint max_j = 0;

		for (uint i = 0; i < img.n_rows; i++) // define left high corner of chessboard
		{
			for (uint j = 0; j < img.n_cols; j++)
			{
				tie(r, g, b) = img(i, j);
				if ((r < 100) && (g < 100) && (b < 100))
				{
					count++;
					if (count == 1) min_j = j;
					if (count > static_cast<int>(img.n_cols/3)) 
					{
						min_i = i;
						gh = 1;
						break;
					}
				}
				else
				{
					count = 0;
				}
			}
			if (gh == 1) break;
		}

		count = 0;
		gh = 0;
		for (uint i = img.n_rows-1; i > 0; i--) // define right lower corner of chessboard
		{
			for (uint j = img.n_cols-1; j > 0; j--)
			{
				tie(r, g, b) = img(i, j);
				if ((r < 100) && (g < 100) && (b < 100))
				{
					count++;
					if (count == 1) max_j = j;
					if (count > static_cast<int>(img.n_cols/3)) 
					{
						max_i = i;
						gh = 1;
						break;
					}
				}
				else
				{
					count = 0;
				}
			}
			if (gh == 1) break;
		}

	//	cout << min_i << endl;
	//	cout << min_j << endl;
	//	cout << max_i << endl;
	//	cout << max_j << endl;
	//	cout << img.n_rows << endl;
	//	cout << img.n_cols << endl;

		for (uint i = 0; i < img.n_rows; i++) // Making of white figures by black
		for (uint j = 0; j < img.n_cols; j++)
		{
			tie(r, g, b) = img(i,j);
			if ((r >= 200) && (g >= 200) && (b >= 200))
			img(i, j) = make_tuple(0, 0, 0);
		}

		for (uint i = 0; i < img.n_rows; i++) // binarization
		for (uint j = 0; j < img.n_cols; j++)
		{
			tie(r, g, b) = img(i, j);
			if ((r >= 100) || (g >= 100) || (b >= 100))
			{
				img(i, j) = make_tuple(255, 255, 255);
				binary_map(i, j) = 0;
			}
			else
			{
				img(i, j) = make_tuple(0, 0, 0);
				binary_map(i, j) = 1;
			}
		}


			/*	for (uint i = min_i; i <= max_i; i++)
		for (uint j = min_j; j <= max_j; j++)
		{
			img(i, j) = make_tuple(0, 255, 0);
		}*/
		/*
		for (uint i = 1; i < img.n_rows-1; i++)
		for (uint j = 1; j < img.n_cols-1; j++)
		{
			int k = 0; 
			if (binary_map(i-1, j) == 0) k++;
			if (binary_map(i+1, j) == 0) k++;
			if (binary_map(i, j-1) == 0) k++;
			if (binary_map(i, j+1) == 0) k++;
			if (k < 3)
			{
				img(i, j) = make_tuple(255, 255, 255);
				binary_map(i, j) = 1;
			}
		}*/
		
		uint n = 1;

		for (uint i = min_i; i <= max_i; i++)
		for (uint j = min_j; j <= max_j; j++)
		{
			if (binary_map(i, j) == 1)
			{
				if ((j == 0) && (i == 0))
				{
					object_map(i, j) = n;
					n++;
				}
				else if (i == 0)
				{
					if (binary_map(i, j-1) == 1)
					{	
						object_map(i, j) = object_map(i, j-1);
					}
					else
					{
						object_map(i, j) = n;
						n++;
					}
				}
				else if (j == 0)
				{
					if (binary_map(i-1, j) == 1)
					{	
						object_map(i, j) = object_map(i-1, j);
					}
					else
					{
						object_map(i, j) = n;
						n++;
					}
				}
				else
				{
					if ((binary_map(i-1, j) == 1) && (binary_map(i, j-1)) == 1)
					{
						object_map(i, j) = object_map(i, j-1);
						if (object_map(i, j-1) != object_map(i-1, j))
						{
							uint t = object_map(i-1, j);
							for (uint k = min_i; k <= i-1; k++)
							for (uint l = min_j; l <= max_j; l++)
							{
								if (object_map(k, l) == t) 
								{
									object_map(k, l) = object_map(i, j-1);
								}
							}
							for (uint l = min_i; l <= j; l++)
							{
								if (object_map(i, l) == t) 
								{
									object_map(i, l) = object_map(i, j-1);
								}
							}
						}
					}
					else if ((binary_map(i-1, j) == 1) && (binary_map(i, j-1) == 0))
					{
						object_map(i, j) = object_map(i-1, j);
					}
					else if ((binary_map(i-1, j) == 0) && (binary_map(i, j-1) == 1))
					{
						object_map(i, j) = object_map(i, j-1);
					}
					else
					{
						object_map(i, j) = n;
						n++;
					}
				}
			}
			else object_map(i, j) = 0;
		}

		vector<uint> P = vector<uint>(n);
		vector<uint> S = vector<uint>(n);
	//	vector<uint> Mx = vector<uint>(n);
	//	vector<uint> My = vector<uint>(n);
		vector<float> coef = vector<float>(n);

		for (uint k = 2; k < n; k++)
		{
			P[k] = 0;
			S[k] = 0;
			coef[k] = 0.0f;
			for (uint i = min_i+1; i <= max_i-1; i++)
			for (uint j = min_j+1; j <= max_j-1; j++)
			{		
				if (object_map(i, j) == k)
				{
					S[k]++;
					if ((object_map(i-1, j) != k) || (object_map(i-1, j+1) != k) || (object_map(i+1, j-1) != k) || (object_map(i+1, j+1) != k)) P[k]++;
				}
			}
			if (S[k] >= 10)
			{
				coef[k] = static_cast<float>(P[k]*P[k]) / S[k];
		//		cout << k << ": " << coef[k] << endl;
			}
		}
		/*
		for (uint k = 2; k < n; k++)
		{
			Mx[k] = 0;
			My[k] = 0;
			for (uint i = min_i+1; i <= max_i-1; i++)
			for (uint j = min_j+1; j <= max_j-1; j++)
			{		
				if (object_map(i, j) == k)
				{
					S[k]++;
					if ((object_map(i-1, j) != k) || (object_map(i-1, j+1) != k) || (object_map(i+1, j-1) != k) || (object_map(i+1, j+1) != k)) P[k]++;
				}
			}
		}
		*/
		for (uint i = min_i; i <= max_i; i++)
		{
			img2(i, max_j) = make_tuple(0, 0, 255);
			img2(i, max_j-1) = make_tuple(0, 0, 255);
		}
		for (uint i = min_i; i <= max_i; i++)
		{
			img2(i, min_j) = make_tuple(0, 0, 255);
			img2(i, min_j+1) = make_tuple(0, 0, 255);
		}
		for (uint j = min_j; j <= max_j; j++)
		{
			img2(min_i, j) = make_tuple(0, 0, 255);
			img2(min_i+1, j) = make_tuple(0, 0, 255);
		}
		for (uint j = min_j; j <= max_j; j++)
		{
			img2(max_i, j) = make_tuple(0, 0, 255);
			img2(max_i-1, j) = make_tuple(0, 0, 255);
		}


		uint temp_i = (max_i - min_i)/8;
		uint temp_j = (max_j - min_j)/8;
		Matrix<int> chess = Matrix<int>(8, 8);
		Matrix<int> beat = Matrix<int>(8, 8);

		for (uint i = 0; i <= 7; i++)
		for (uint j = 0; j <= 7; j++)
		{
			int as = 0;
			chess(i, j) = 0;
			beat(i, j) = 0;
			for (uint k = min_i + i*temp_i; k <= min_i + (i+1)*temp_i; k++)
			{
			for (uint l = min_j + j*temp_j; l <= min_j + (j+1)*temp_j; l++)
			{
				if (object_map(k, l) > 1)
				{
					uint p = object_map(k, l);
					if (S[p] >= 10) 
					{
						if (abs(coef[p] - 28.0257) <= 1) 
						{	
							tie(r, g, b) = img2(k+20, l);
							if ((r >= 200) && (g >= 200) && (b >= 200))
							{	
								chess(i, j) = 4;
								beat(i, j) = 1;
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + j*temp_j+1) = make_tuple(121, 11, 11);
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + (j+1)*temp_j-1) = make_tuple(121, 11, 11);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + i*temp_i+1, m) = make_tuple(121, 11, 11);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + (i+1)*temp_i-1, m) = make_tuple(121, 11, 11);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + j*temp_j+2) = make_tuple(121, 11, 11);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + (j+1)*temp_j-2) = make_tuple(121, 11, 11);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + i*temp_i+2, m) = make_tuple(121, 11, 11);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + (i+1)*temp_i-2, m) = make_tuple(121, 11, 11);
							}
							else
							{
								chess(i, j) = 1;
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + j*temp_j+1) = make_tuple(30, 89, 19);
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + (j+1)*temp_j-1) = make_tuple(30, 89, 19);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + i*temp_i+1, m) = make_tuple(30, 89, 19);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + (i+1)*temp_i-1, m) = make_tuple(30, 89, 19);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + j*temp_j+2) = make_tuple(30, 89, 19);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + (j+1)*temp_j-2) = make_tuple(30, 89, 19);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + i*temp_i+2, m) = make_tuple(30, 89, 19);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + (i+1)*temp_i-2, m) = make_tuple(30, 89, 19);
							}
						}
						else if (abs(coef[p] - 61.1391) <= 1)
						{								
							tie(r, g, b) = img2(k+20, l);
							if ((r >= 200) && (g >= 200) && (b >= 200))
							{
								chess(i, j) = 5;
								beat(i, j) = 1;
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + j*temp_j+1) = make_tuple(237, 44, 44);
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + (j+1)*temp_j-1) = make_tuple(237, 44, 44);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + i*temp_i+1, m) = make_tuple(237, 44, 44);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + (i+1)*temp_i-1, m) = make_tuple(237, 44, 44);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + j*temp_j+2) = make_tuple(237, 44, 44);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + (j+1)*temp_j-2) = make_tuple(237, 44, 44);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + i*temp_i+2, m) = make_tuple(237, 44, 44);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + (i+1)*temp_i-2, m) = make_tuple(237, 44, 44);
							}
							else 
							{
								chess(i, j) = 2;
								beat(i, j) = 1;
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + j*temp_j+1) = make_tuple(54, 179, 35);
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + (j+1)*temp_j-1) = make_tuple(54, 179, 35);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + i*temp_i+1, m) = make_tuple(54, 179, 35);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + (i+1)*temp_i-1, m) = make_tuple(54, 179, 35);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + j*temp_j+2) = make_tuple(54, 179, 35);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + (j+1)*temp_j-2) = make_tuple(54, 179, 35);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + i*temp_i+2, m) = make_tuple(54, 179, 35);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + (i+1)*temp_i-2, m) = make_tuple(54, 179, 35);
							}
						}
						else if (abs(coef[p] - 58.1638) <= 1)
						{							
							tie(r, g, b) = img2(k+20, l);
							if ((r >= 200) && (g >= 200) && (b >= 200))
							{
								chess(i, j) = 6;
								beat(i, j) = 1;
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + j*temp_j+1) = make_tuple(250, 192, 192);
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + (j+1)*temp_j-1) = make_tuple(250, 192, 192);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + i*temp_i+1, m) = make_tuple(250, 192, 192);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + (i+1)*temp_i-1, m) = make_tuple(250, 192, 192);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + j*temp_j+2) = make_tuple(250, 192, 192);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + (j+1)*temp_j-2) = make_tuple(250, 192, 192);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + i*temp_i+2, m) = make_tuple(250, 192, 192);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + (i+1)*temp_i-2, m) = make_tuple(250, 192, 192);
							}
							else 
							{
								chess(i, j) = 3;
								beat(i, j) = 1;
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + j*temp_j+1) = make_tuple(177, 235, 167);
								for (uint m = min_i + i*temp_i+1; m <= min_i + (i+1)*temp_i-1; m++) img2(m, min_j + (j+1)*temp_j-1) = make_tuple(177, 235, 167);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + i*temp_i+1, m) = make_tuple(177, 235, 167);
								for (uint m = min_j + j*temp_j+1; m <= min_j + (j+1)*temp_j-1; m++) img2(min_i + (i+1)*temp_i-1, m) = make_tuple(177, 235, 167);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + j*temp_j+2) = make_tuple(177, 235, 167);
								for (uint m = min_i + i*temp_i+2; m <= min_i + (i+1)*temp_i-2; m++) img2(m, min_j + (j+1)*temp_j-2) = make_tuple(177, 235, 167);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + i*temp_i+2, m) = make_tuple(177, 235, 167);
								for (uint m = min_j + j*temp_j+2; m <= min_j + (j+1)*temp_j-2; m++) img2(min_i + (i+1)*temp_i-2, m) = make_tuple(177, 235, 167);
							}
						}
						else  chess(i, j) = 7;
						as = 1;
						break;
					}
				}
			}
			if (as == 1) break;
			}
		}

		int k, l;
		int i_mat = 0;
		int j_mat = 0;
		int i_shach = 0;
		int j_shach = 0;
	//	int a = 0;
	//	int b = 0;

		for (int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
		{
			if (chess(i, j) == 4)
			{
				k = i-1;
				l = j;
				if ((k >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					k = k-1;
				}
				k = i+1;
				l = j;
				if ((k <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					k = k+1;
				}
				k = i;
				l = j-1;
				if ((l >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					l = l-1;
				}
				k = i;
				l = j+1;
				if ((l <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					l = l+1;
				}
				k = i-1;
				l = j-1;
				if ((k >= 0) && (l >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					k = k-1;
					l = l-1;
				}
				k = i-1;
				l = j+1;
				if ((k >= 0) && (l <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					k = k-1;
					l = l+1;
				}
				k = i+1;
				l = j-1;
				if ((k <= 7) && (l >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					k = k+1;
					l = l-1;
				}
				k = i+1;
				l = j+1;
				if ((k <= 7) && (l <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					beat(k, l) = 1;
					k = k+1;
					l = l+1;
				}
			}
			if (chess(i, j) == 5)
			{
				k = i-1;
				l = j;
				while ((k >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = 0;
	//					b = -1;
					}
					beat(k, l) = 1;
					k = k-1;
				}
				k = i+1;
				l = j;
				while ((k <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = 0;
	//					b = 1;
					}
					beat(k, l) = 1;
					k = k+1;
				}
				k = i;
				l = j-1;
				while ((l >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = -1;
	//					b = 0;
					}
					beat(k, l) = 1;
					l = l-1;
				}
				k = i;
				l = j+1;
				while ((l <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = 1;
	//					b = 0;
					}
					beat(k, l) = 1;
					l = l+1;
				}
				k = i-1;
				l = j-1;
				while ((k >= 0) && (l >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = -1;
	//					b = -1;
					}
					beat(k, l) = 1;
					k = k-1;
					l = l-1;
				}
				k = i-1;
				l = j+1;
				while ((k >= 0) && (l <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = 1;
	//					b = -1;
					}
					beat(k, l) = 1;
					k = k-1;
					l = l+1;
				}
				k = i+1;
				l = j-1;
				while ((k <= 7) && (l >= 0) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = -1;
	//					b = 1;
					}
					beat(k, l) = 1;
					k = k+1;
					l = l-1;
				}
				k = i+1;
				l = j+1;
				while ((k <= 7) && (l <= 7) && (chess(k, l) != 2) && (chess(k, l) != 3) && (chess(k, l) != 4) && (chess(k, l) != 5) && (chess(k, l) != 6))
				{
					if (chess(k, l) == 1)
					{
						i_mat = k;
						j_mat = l;
						i_shach = i;
						j_shach = j;
	//					a = 1;
	//					b = 1;
					}
					beat(k, l) = 1;
					k = k+1;
					l = l+1;
				}
			}
			if (chess(i, j) == 6)
			{
				if (i != 0)
				{
					if (j == 0)
					{	
						if (chess(i-1, j+1) == 1)
						{
							i_mat = i-1;
							j_mat = j+1;
							i_shach = i;
							j_shach = j;
	//						a = 1;
	//						b = -1;
						}
						beat(i-1, j+1);
					}
					else if (j == 7) 
					{
						if (chess(i-1, j-1) == 1)
						{
							i_mat = i-1;
							j_mat = j-1;
							i_shach = i;
							j_shach = j;
	//						a = -1;
	//						b = 1;
						}
						beat(i-1, j-1);
					}
					else
					{
						if (chess(i-1, j-1) == 1)
						{
							i_mat = i-1;
							j_mat = j-1;
							i_shach = i;
							j_shach = j;
	//						a = -1;
	//						b = -1;
						}
						beat(i-1, j-1) = 1;
						if (chess(i-1, j+1) == 1)
						{
							i_mat = i-1;
							j_mat = j+1;
							i_shach = i;
							j_shach = j;
	//						a = 1;
	//						b = -1;
						}
						beat(i-1, j+1) = 1;
					}
				}
			}
		}

		int mat = 0;
		int stop = 0;
		for (int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
		{
			if ((chess(i, j) == 1) && (beat(i, j) == 1))
			{
				if ((i == 0) && (j == 0) && beat(i+1, j) && beat(i, j+1) && beat(i+1, j+1)) mat = 1;
				else if ((i == 0) && (j == 7) && beat(i+1, j) && beat(i, j-1) && beat(i+1, j-1)) mat = 1;
				else if ((i == 7) && (j == 0) && beat(i-1, j) && beat(i-1, j+1) && beat(i, j+1)) mat = 1;
				else if ((i == 7) && (j == 7) && beat(i, j-1) && beat(i-1, j-1) && beat(i-1, j)) mat = 1;
				else if ((i == 0) && beat(i, j-1) && beat(i, j+1) && beat(i+1, j-1) && beat(i+1, j) && beat(i+1, j+1)) mat = 1;
				else if ((i == 7) && beat(i, j-1) && beat(i, j+1) && beat(i-1, j-1) && beat(i-1, j) && beat(i-1, j+1)) mat = 1;
				else if ((j == 0) && beat(i-1, j) && beat(i+1, j) && beat(i-1, j+1) && beat(i, j+1) && beat(i+1, j+1)) mat = 1;
				else if ((j == 7) && beat(i-1, j) && beat(i+1, j) && beat(i-1, j-1) && beat(i, j-1) && beat(i+1, j-1)) mat = 1;
				else if ((i != 0) && (j != 0) && beat(i-1, j-1) && beat(i-1, j) && beat(i-1, j+1) && beat(i, j-1) && beat(i, j) && beat(i, j+1) && beat(i+1, j-1) && beat(i+1, j) && beat(i+1, j+1)) mat = 1;
				if (mat == 0)
				{
					if ((i == 0) && (j == 0))
					{
						if (beat(i+1, j) == 0) { i_shach = i + 1; j_shach = j; }
						if (beat(i, j+1) == 0) { i_shach = i; j_shach = j + 1; }
						if (beat(i+1, j+1) == 0) { i_shach = i + 1; j_shach = j + 1; }
					}
					else if ((i == 0) && (j == 7))
					{
						if (beat(i+1, j) == 0) { i_shach = i + 1; j_shach = j; }
						if (beat(i, j-1) == 0) { i_shach = i; j_shach = j - 1; }
						if (beat(i+1, j-1) == 0) { i_shach = i + 1; j_shach = j - 1; }
					}
					else if ((i == 7) && (j == 0))
					{
						if (beat(i-1, j) == 0) { i_shach = i - 1; j_shach = j; }
						if (beat(i-1, j+1) == 0) { i_shach = i - 1; j_shach = j + 1; }
						if (beat(i, j+1) == 0) { i_shach = i; j_shach = j + 1; }
					}
					else if ((i == 7) && (j == 7))
					{
						if (beat(i, j-1) == 0) { i_shach = i; j_shach = j - 1; }
						if (beat(i-1, j-1) == 0) { i_shach = i - 1; j_shach = j - 1; }
						if (beat(i-1, j) == 0) { i_shach = i - 1; j_shach = j; }
					}
					else if (i == 0)
					{
						if (beat(i+1, j) == 0) { i_shach = i + 1; j_shach = j; }
						if (beat(i, j+1) == 0) { i_shach = i; j_shach = j + 1; }
						if (beat(i+1, j+1) == 0) { i_shach = i + 1; j_shach = j + 1; }
						if (beat(i+1, j-1)) { i_shach = i + 1; j_shach = j - 1; }
						if (beat(i, j-1) == 0) { i_shach = i; j_shach = j - 1; }
					}
					else if (i == 7)
					{
						if (beat(i, j-1) == 0) { i_shach = i; j_shach = j - 1; }
						if (beat(i, j+1) == 0) { i_shach = i; j_shach = j + 1; }
						if (beat(i-1, j-1) == 0) { i_shach = i - 1; j_shach = j - 1; }
						if (beat(i-1, j)) { i_shach = i - 1; j_shach = j; }
						if (beat(i-1, j+1) == 0) { i_shach = i - 1; j_shach = j + 1; }
					}
					else if (j == 0)
					{
						if (beat(i-1, j) == 0) { i_shach = i - 1; j_shach = j; }
						if (beat(i+1, j) == 0) { i_shach = i + 1; j_shach = j; }
						if (beat(i-1, j+1) == 0) { i_shach = i - 1; j_shach = j + 1; }
						if (beat(i, j+1)) { i_shach = i; j_shach = j + 1; }
						if (beat(i+1, j+1) == 0) { i_shach = i + 1; j_shach = j + 1; }
					}
					else if (j == 7)
					{
						if (beat(i-1, j) == 0) { i_shach = i - 1; j_shach = j; }
						if (beat(i+1, j) == 0) { i_shach = i + 1; j_shach = j; }
						if (beat(i-1, j-1) == 0) { i_shach = i - 1; j_shach = j - 1; }
						if (beat(i, j-1)) { i_shach = i; j_shach = j - 1; }
						if (beat(i+1, j-1) == 0) { i_shach = i + 1; j_shach = j - 1; }
					}
					else 
					{
						for (int e = i-1; e <= i+1; e++)
						for (int q = j-1; q <= j+1; q++)
						{
							if (beat(e, q) == 0) 
							{
								i_shach = e;
								j_shach = q;
							}
						}
					}
				}
			}
		//	else stop = 1;
		}
	if (stop == 0)
	{
	//	cout << "asd" << endl;
		uint point_i_mat = min_i + (2*i_mat+1)*temp_i/2;
		uint point_j_mat = min_j + (2*j_mat+1)*temp_j/2;
		uint point_i_shach = min_i + (2*i_shach+1)*temp_i/2;
		uint point_j_shach = min_j + (2*j_shach+1)*temp_j/2;

		for (uint i = point_i_mat-2; i <= point_i_mat+2; i++)
		for (uint j = point_j_mat-2; j <= point_j_mat+2; j++)
		{
			img2(i, j) = make_tuple(0, 255, 0);
		}

		for (uint i = point_i_shach-2; i <= point_i_shach+2; i++)
		for (uint j = point_j_shach-2; j <= point_j_shach+2; j++)
		{
			img2(i, j) = make_tuple(0, 255, 0);
		}

		int x1 = point_i_mat;
		int x2 = point_i_shach;
		int y1 = point_j_mat;
		int y2 = point_j_shach;
	//	cout << x1 << endl;
	//	cout << x2 << endl;
	//	cout << y1 << endl;
	//	cout << y2 << endl;
		int x = 0;
		int y = 0;

		if (x1 < x2)
		{
			for (x = x1 + 1; x <= x2; x++)
			{
				y = y1 + static_cast<float>(y2 - y1)*(x - x1)/(x2 - x1);
				img2(x, y) = make_tuple(0, 255, 0);
			}
		}
		else if (x1 > x2)
		{
			for (x = x1 - 1; x >= x2; x--)
			{
				y = y1 + static_cast<float>(y2 - y1)*(x - x1)/(x2 - x1);
				img2(x, y) = make_tuple(0, 255, 0);
			}
		}
		else
		{
			if (y1 > y2)
			for (y = y1; y >= y2; y--)
			{
				img2(x1, y) = make_tuple(0, 255, 0);
			}
			else
			for (y = y1; y <= y2; y++)
			{
				img2(x1, y) = make_tuple(0, 255, 0);
			}	
		}
	}

	/*
	int a = 0;
	int b = 0;

	if (static_cast<int>(point_i_shach) - point_i_mat > 3) a = 1; 
	if (static_cast<int>(point_i_shach) - point_i_mat < 3) a = -1;
	if (static_cast<int>(point_j_shach) - point_j_mat > 3) b = 1; 
	if (static_cast<int>(point_j_shach) - point_j_mat < 3) b = -1;

	while 

		for (uint i = 0; i <= 7; i++)
		{
		for (uint j = 0; j <= 7; j++)
		{
			cout << beat(i, j) << " ";
		}
		cout << endl;
		}
		cout << mat << endl;
				for (uint i = min_i; i <= max_i; i++)
		for (uint j = min_j; j <= max_j; j++)
		{
			if (object_map(i, j) == 1) 
			{
				img(i, j) = make_tuple (0, 255, 0);
			}
		}*/
		
	//	cout << n << endl;;

	//	img(0,3)=make_tuple(0,255,0);

        vector<Rect> path;
        Image dst_image;
        tie(path, dst_image) = find_treasure(img2);
        save_image(dst_image, argv[2]);

        uint x, y, width, height;
        for (const auto &obj : path)
        {
            tie(x, y, width, height) = obj;
         //   fout << x << " " << y << " " << width << " " << height << endl;
        }

    } catch (const string &s) {
        cerr << "Error: " << s << endl;
        return 1;
    }
}
