#include <iostream>
#include <vector>
using namespace std;

class BitStream
{
public:
    vector<bool> buffer;
    FILE *file;
    const char *perm;
    int bit_count;
    char current_char;

    BitStream(const char *path, const char *perm)
    {
        try
        {
            if (*perm != 'r' && *perm != 'w')
            {
                throw *perm;
            }
            this->perm = perm;
            this->file = fopen(path, this->perm);
            this->bit_count = 0;

            if (this->file == nullptr)
            {
                throw 0;
            }
        }
        catch (char my_exception)
        {
            if (my_exception == *perm)
            {
                cout << "Illegal Permission: " << my_exception << endl;
                cout << "Only Read (r) and Write (w) are allowed!" << endl;
                exit(1);
            }
        }
        catch (int my_exception)
        {
            if (my_exception == 0)
            {
                cout << "File Does Not Exist: " << path << endl;
                cout << "It's not possible to create or locate the specified file!" << endl;
                exit(1);
            }
        }
    }

    // reads one bit from a file
    bool read_bit()
    {
        if (*this->perm != 'r')
        {
            cout << "Reading Permission Required" << endl;
            exit(2);
        }

        if (this->bit_count == 0)
        {
            this->current_char = fgetc(this->file);
        }

        if (feof(this->file))
        {
            // cout << "End of file reached" << endl;
            this->current_char = 0;
            return false;
        }

        buffer.push_back((this->current_char >> (7 - this->bit_count)) & 0x01);
        this->bit_count = (this->bit_count + 1) % 8;

        return true;
    }

    // call the read_bit function "n" times
    bool read_n_bits(int n)
    {
        int i;
        for (i = 0; i < n; i++)
        {
            if (!this->read_bit())
            {
                // cout << i << " of " << n << " bits read" << endl;
                return true;
            }
        }
        return false;
    }

    // waits until the buffer has 8 bits to write a character to a file
    bool write_bit(bool bit)
    {
        if (*this->perm != 'w')
        {
            cout << "Writing Permission Required" << endl;
            exit(2);
        }
        buffer.push_back(bit);

        if (this->can_write())
        {
            char tmp = this->bool_array_to_char();
            return fputc(tmp, this->file) == tmp;
        }
        return true;
    }

    // calls the write_bit function "n" times
    bool write_n_bits(vector<bool> c)
    {
        int n = 0;
        for (auto i = c.cbegin(); i != c.cend(); ++i)
        {
            this->write_bit(*i);
        }
        return true;
    }

    // closes the opened file and flushes the buffer
    bool close()
    {
        if (*this->perm == 'w')
        {
            while (this->buffer.size() % 8 != 0) // flush the buffer
            {
                this->write_bit(false);
            }
        }

        fclose(this->file);
        return 0;
    }

    // show the content of the buffer
    void show_buffer()
    {
        int n = 0;
        for (auto i = this->buffer.cbegin(); i != this->buffer.cend(); i++)
        {
            if (n % 8 == 0)
            {
                cout << endl;
            }
            cout << *i << " ";
            n++;
        }
        cout << endl;
    }

    // checks of the buffer is empty
    bool buffer_is_empty()
    {
        return this->buffer.empty();
    }

    // checks if if the buffer has enough bits(8) to let the writing process start
    bool can_write()
    {
        return this->buffer.size() >= 8;
    }

    // converts the first 8 elements(bits) of the buffer to a char
    char bool_array_to_char()
    {
        int n = 0, i = 0;
        for (auto it = this->buffer.cbegin(); it != this->buffer.cend(); ++it)
        {
            if (*it)
            {
                n |= (1 << (7 - i));
            }
            i++;
        }
        this->buffer.erase(this->buffer.cbegin(), this->buffer.cbegin() + 8);
        return (char)n;
    }
};