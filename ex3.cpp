#include <iostream>
#include "Golomb.h"
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    string to_encode("Write here stuff to encode! :)");
    int m = 30;
    if (argc == 3)
    {
        m = atoi(argv[1]);
        to_encode = argv[2];

        vector<int> enc;
        for (int i = 0; i < to_encode.length(); i++)
        {
            enc.push_back(to_encode.at(i));
        }
        vector<int> dec;

        cout << "Golomb m = " << m << endl;
        Golomb g(m, "g.bits");
        g.encode(enc);
        cout << "Encoded the values: ";
        for (auto i = enc.cbegin(); i != enc.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec = g.decode();
        cout << "Decoded the values: ";
        for (auto i = dec.cbegin(); i != dec.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec.erase(dec.cbegin(), dec.cend());
        cout << "Encoded file used " << g.get_filename_num_chars() * 8 << " bits" << endl;
        g.delete_bin_file();
    }
    else
    {
        vector<int> enc;
        for (int i = 0; i < to_encode.length(); i++)
        {
            enc.push_back(to_encode.at(i));
        }
        vector<int> dec;

        cout << "Golomb m = 4" << endl;
        Golomb g4(4, "g4.bits");
        g4.encode(enc);
        cout << "Encoded the values: ";
        for (auto i = enc.cbegin(); i != enc.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec = g4.decode();
        cout << "Decoded the values: ";
        for (auto i = dec.cbegin(); i != dec.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec.erase(dec.cbegin(), dec.cend());
        cout << "Encoded file used " << g4.get_filename_num_chars() * 8 << " bits" << endl;
        g4.delete_bin_file();

        // -------------------------------------------------------------------------------------
        cout << "\nGolomb m = 7" << endl;
        Golomb g7(7, "g7.bits");
        g7.encode(enc);
        cout << "Encoded the values: ";
        for (auto i = enc.cbegin(); i != enc.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec = g7.decode();
        cout << "Decoded the values: ";
        for (auto i = dec.cbegin(); i != dec.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec.erase(dec.cbegin(), dec.cend());
        cout << "Encoded file used " << g7.get_filename_num_chars() * 8 << " bits" << endl;
        g7.delete_bin_file();

        // -------------------------------------------------------------------------------------
        cout << "\nGolomb m = 9" << endl;
        Golomb g9(9, "g9.bits");
        g9.encode(enc);
        cout << "Encoded the values: ";
        for (auto i = enc.cbegin(); i != enc.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec = g9.decode();
        cout << "Decoded the values: ";
        for (auto i = dec.cbegin(); i != dec.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec.erase(dec.cbegin(), dec.cend());
        cout << "Encoded file used " << g9.get_filename_num_chars() * 8 << " bits" << endl;
        g9.delete_bin_file();

        // -------------------------------------------------------------------------------------
        cout << "\nGolomb m = 21" << endl;
        Golomb g21(21, "g21.bits");
        g21.encode(enc);
        cout << "Encoded the values: ";
        for (auto i = enc.cbegin(); i != enc.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec = g21.decode();
        cout << "Decoded the values: ";
        for (auto i = dec.cbegin(); i != dec.cend(); i++)
        {
            cout << (char)*i << " ";
        }
        cout << endl;
        dec.erase(dec.cbegin(), dec.cend());
        cout << "Encoded file used " << g21.get_filename_num_chars() * 8 << " bits" << endl;
        g21.delete_bin_file();
    }

    return 0;
}