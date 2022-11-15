#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include "Golomb.h"
using namespace std;
using namespace cv;

vector<vector<int>> cmp_vec;
string image;
int m;
Mat rgb;
Mat yuv;

int jpeg_ls_prediction(int a, int b, int c)
{
    if (c >= max(a, b))
        return min(a, b);
    else if (c <= min(a, b))
        return max(a, b);
    else
        return a + b - c;
}

bool cmp(pair<int, int> &a, pair<int, int> &b)
{
    return (a.first < b.first);
}

void sort_map(map<int, int> &m)
{
    vector<pair<int, int>> v;
    for (auto &i : m)
    {
        v.push_back(i);
    }
    sort(v.begin(), v.end(), cmp);
    /* map<int, int> new_map;
    for (auto &i : v)
    {
        new_map.emplace(i.first, i.second);
    }
    return new_map; */
}

_Float32 vector_mean(vector<int> x)
{
    _Float32 mean = 0;
    for (auto &i : x)
    {
        mean += i;
    }
    return mean / x.size();
}

_Float32 vector_std_dev(vector<int> x)
{
    _Float32 var = 0;
    _Float32 mean_x = vector_mean(x);
    for (auto &i : x)
    {
        var += pow(i - mean_x, 2);
    }
    var /= x.size();
    return sqrt(var);
}

template <typename T>
void show_vector(vector<T> x)
{
    int n = 0;
    int show = 1000;
    for (int i = show; i < show + 10; i++)
    {
        cout << x[i] << " ";
    }
    cout << endl;
}

template <typename T>
int index_wrong(vector<T> x, vector<T> y)
{
    int size = min(x.size(), y.size());
    for (int i = 0; i < size; i++)
    {
        if (x[i] != y[i])
            return i;
    }
}

void encode_image(Mat rgb, Mat yuv, vector<int> &Y, vector<int> &U, vector<int> &V, map<int, int> &char_count, vector<string> filenames)
{
    vector<int> file;
    for (int y = 0; y < yuv.cols; y++)
    {
        for (int x = 0; x < yuv.rows; x++)
        {
            int elem = (int)yuv.at<uchar>(y, x);

            if (char_count.count(elem) > 0)
            {
                char_count.at(elem) += 1;
            }
            else
            {
                char_count.emplace(elem, 1);
            }

            int a = (y == 0) ? 0 : (int)yuv.at<uchar>(y - 1, x);
            int b = (x == 0 || x == rgb.rows || x == yuv.rows - (rgb.rows / 4)) ? 0 : (int)yuv.at<uchar>(y, x - 1);
            int c = ((y == 0) || (x == 0 || x == rgb.rows || x == yuv.rows - (rgb.rows / 4))) ? 0 : (int)yuv.at<uchar>(y - 1, x - 1);
            if (x < rgb.rows)
            {
                int enc = elem - jpeg_ls_prediction(a, b, c);
                Y.push_back(enc);
                file.push_back(enc);
            }
            else if (x < yuv.rows - (rgb.rows / 4))
            {
                int enc = elem - jpeg_ls_prediction(a, b, c);
                U.push_back(enc);
                file.push_back(enc);
            }
            else
            {
                int enc = elem - jpeg_ls_prediction(a, b, c);
                V.push_back(enc);
                file.push_back(enc);
            }
        }
    }

    file.insert(file.cbegin(), rgb.cols);
    file.insert(file.cbegin(), rgb.rows);

    file.insert(file.cbegin(), rgb.cols);
    file.insert(file.cbegin(), rgb.rows / 4);

    file.insert(file.cbegin(), rgb.cols);
    file.insert(file.cbegin(), rgb.rows / 4);

    Y.insert(Y.cbegin(), rgb.cols); // parameters to recover Y size
    Y.insert(Y.cbegin(), rgb.rows);

    U.insert(U.cbegin(), rgb.cols); // parameters to recover U size
    U.insert(U.cbegin(), rgb.rows / 4);

    V.insert(V.cbegin(), rgb.cols); // parameters to recover V size
    V.insert(V.cbegin(), rgb.rows / 4);

    Golomb gol_y(m, filenames[0]);
    gol_y.encode(Y);

    Golomb gol_u(m, filenames[1]);
    gol_u.encode(U);

    Golomb gol_v(m, filenames[2]);
    gol_v.encode(V);

    /* cmp_vec.push_back(Y);
    cmp_vec.push_back(U);
    cmp_vec.push_back(V); */

    float size = Y.size();
    float num_chars = gol_y.get_filename_num_chars();
    cout << size << " characters were encoded for Y" << endl;
    cout << "Space used is " << num_chars << " bytes. " << endl;
    cout << "Entropy is " << num_chars * 8 / size << " bits/char" << endl
         << endl;

    size = U.size();
    num_chars = gol_u.get_filename_num_chars();
    cout << size << " characters were encoded for Cb" << endl;
    cout << "Space used is " << num_chars << " bytes. " << endl;
    cout << "Entropy is " << num_chars * 8 / size << " bits/char" << endl
         << endl;

    size = V.size();
    num_chars = gol_v.get_filename_num_chars();
    cout << size << " characters were encoded for Cr" << endl;
    cout << "Space used is " << num_chars << " bytes. " << endl;
    cout << "Entropy is " << num_chars * 8 / size << " bits/char" << endl
         << endl;

    cout << "----- Full Image ------" << endl
         << endl;

    cout << "Original" << endl
         << endl;

    Golomb tmp(1, image);
    num_chars = tmp.get_filename_num_chars();
    cout << "Space used is " << num_chars << " bytes. " << endl
         << endl;

    cout << "Compressed" << endl
         << endl;
    Golomb f(m, "full_image.bits");
    f.encode(file);

    size = file.size();
    num_chars = f.get_filename_num_chars();
    cout << size << " characters were encoded for the full image" << endl;
    cout << "Space used is " << num_chars << " bytes. " << endl;
    cout << "Entropy is " << num_chars * 8 / size << " bits/char" << endl
         << endl;

    f.delete_bin_file();
}

void decode_image(vector<string> filenames)
{
    Golomb gol_y2(1, filenames[0]);
    vector<int> Y = gol_y2.decode();
    gol_y2.delete_bin_file();

    Golomb gol_u2(1, filenames[1]);
    vector<int> U = gol_u2.decode();
    gol_u2.delete_bin_file();

    Golomb gol_v2(1, filenames[2]);
    vector<int> V = gol_v2.decode();
    gol_v2.delete_bin_file();

    int rows = Y[0] + U[0] + V[0];
    int cols = V[1];

    Mat new_image = Mat::zeros(Size(Y[0], Y[1]), CV_8UC3);
    cvtColor(rgb, new_image, COLOR_BGR2YUV_I420);

    int i = 0, j = 0, k = 0;
    for (int y = 0; y < new_image.cols; y++)
    {
        for (int x = 0; x < new_image.rows; x++)
        {

            if (x < Y[0])
            {
                int a = (i < Y[0]) ? 0 : (int)new_image.at<uchar>(y - 1, x);
                int b = (i % Y[0] == 0) ? 0 : (int)new_image.at<uchar>(y, x - 1);
                int c = (i % Y[0] == 0 || i < Y[0]) ? 0 : (int)new_image.at<uchar>(y - 1, x - 1);

                char save = jpeg_ls_prediction(a, b, c) + Y.at(i++ + 2);
                new_image.at<uchar>(y, x) = save;
            }
            else if (x < Y[0] + U[0])
            {
                int a = (j < U[0]) ? 0 : (int)new_image.at<uchar>(y - 1, x);
                int b = (j % U[0] == 0) ? 0 : (int)new_image.at<uchar>(y, x - 1);
                int c = (j < U[0] || j % U[0] == 0) ? 0 : (int)new_image.at<uchar>(y - 1, x - 1);

                char save = jpeg_ls_prediction(a, b, c) + U.at(j++ + 2);
                new_image.at<uchar>(y, x) = save;
            }
            else
            {
                int a = (k < V[0]) ? 0 : (int)new_image.at<uchar>(y - 1, x);
                int b = (k % V[0] == 0) ? 0 : (int)new_image.at<uchar>(y, x - 1);
                int c = (k < V[0] || k % V[0] == 0) ? 0 : (int)new_image.at<uchar>(y - 1, x - 1);

                char save = jpeg_ls_prediction(a, b, c) + V.at(k++ + 2);
                new_image.at<uchar>(y, x) = save;
            }
        }
    }
    imshow("Original RGB", rgb);
    imshow("Original YUV", yuv);
    imshow("Restored YUV", new_image);
    cvtColor(new_image, new_image, COLOR_YUV2BGR_I420);
    imshow("Restored RGB", new_image);
    waitKey(0);
}

/* int get_m(map<int, int> &x, int size)
{
    sort_map(x);
    vector<int> m_values;
    int max = 1000;
    for (auto elem : x)
    {
        float n = elem.first;
        float n_count = elem.second;
        if (n_count == 0)
            continue;
        float prob = n_count / size;
        long min = LONG_MAX;
        long min_index = LONG_MAX;
        for (int i = 0; i < max; i++)
        {
            float alpha = (float)i / (float)max;
            float res = pow(alpha, n) * (1.0 - alpha);
            // cout << "*** = " << abs(res - prob) << endl;
            if (abs(res - prob) < min)
            {
                min = abs(res - prob);
                min_index = i;
                if (abs(res - prob) == 0)
                    break;
            }
        }
        cout << "min_index = " << min_index << endl;
        m_values.push_back(min_index);
    }
    float alpha_mean = floor(vector_mean(m_values)) / max;
    int m = ceil(-1 / log2(alpha_mean));
    return m;
} */

// g++ ex1.cpp -std=c++11 `pkg-config --cflags --libs opencv`
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "USAGE: ./ex1 <image1> <golomb_m_value>" << endl;
        exit(1);
    }
    image = argv[1];
    m = atoi(argv[2]);

    rgb = imread(image, IMREAD_COLOR);

    if (rgb.empty())
    {
        cout << "Could not load or find image. Please try again!" << endl;
        exit(1);
    }
    cvtColor(rgb, yuv, COLOR_BGR2YUV_I420);
    // imshow("yuv", yuv);
    //  cout << rgb.cols << "x" << rgb.rows << endl;
    //  cout << yuv.cols << "x" << yuv.rows << endl;

    vector<int> Y, U, V;
    map<int, int> count_char;
    vector<string> filenames = {"y_encoded.bits", "u_encoded.bits", "v_encoded.bits"};
    encode_image(rgb, yuv, Y, U, V, count_char, filenames);

    decode_image(filenames);

    return 0;
}