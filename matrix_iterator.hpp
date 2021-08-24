#include <iterator>
#pragma once

namespace ctd // Custom Template Definition
{
    template<class T>
    class MatrixIterator
    {
    private:
        T **p = nullptr;
        T *p_current = nullptr;

        int size_col = 0;
        int current_row = 0;
        int current_col = 0;

    public:
        // компилятор решил пошалить с warning : declaration does not declare anything
        // потому использую typedef;
        typedef std::ptrdiff_t difference_type;
        typedef std::random_access_iterator_tag iterator_category;
	    using value_type = T;
	    using pointer = T*;
	    using reference = T&;

        MatrixIterator(T **a, int s_row, int s_col, bool begin) : p(a)
        {
            if(begin == true)
            {
                size_col = s_col;
                p_current = &p[current_row][current_col];
            }
            else
            {
                current_row = s_row - 1;
                current_col = size_col = s_col;
                p_current = *(p + s_row);
            }
        }

        ~MatrixIterator()
        {
            p = nullptr;
            p_current = nullptr;
        }

        reference operator*() const
        {
            return *p_current;
        }

        pointer operator[](const difference_type& i) const
        {
            return p[i];
        }

        pointer operator->() const
        {
            return &(*p_current);
        }

        MatrixIterator& operator++()
        {
            // переход на след строку при окончании текущей
            ++current_col;
            if (current_col == size_col)
            {
                ++current_row;
                current_col = 0;
            }
            p_current = &p[current_row][current_col];
            return *this;
        }

        MatrixIterator& operator--()
        {
            --current_col;
            if (current_col == -1)
            {
                --current_row;
                current_col = size_col-1;
            }

            p_current = &p[current_row][current_col];
            return *this;
        }

        MatrixIterator operator+(const difference_type& n) const
        {
            MatrixIterator temp = *this;
            int jump_col = temp.current_col + n - temp.size_col;
            int jump_row = jump_col / temp.size_col;

            if(jump_col >= 0)
            {
                temp.current_row += (jump_row + 1);
                temp.current_col = jump_col - jump_row*temp.size_col;
            }
            else
            {
                temp.current_col += n;
            }

            temp.p_current = &(temp.p[temp.current_row][temp.current_col]);
            return temp;
        }

	    MatrixIterator operator-(const difference_type& n) const
        {
            MatrixIterator temp = *this;
            int jump_col = temp.current_col - n + temp.size_col;
            int jump_row = 1;

            if(jump_col < 0)
            {
                jump_row = ((-jump_col+(temp.size_col/2)) / temp.size_col);
                temp.current_row -= (jump_row + 1);
                temp.current_col = jump_col + jump_row*temp.size_col;
            }
            else if(jump_col < temp.size_col)
            {
                temp.current_row -= jump_row;
                temp.current_col = jump_col;
            }
            else
            {
                temp.current_col -= n;
            }

            temp.p_current = &(temp.p[temp.current_row][temp.current_col]);
            return temp;
        }

	    MatrixIterator& operator+=(const difference_type& n)
        {
            MatrixIterator temp = (*this + n);
            *this = temp;
            return *this;
        }

	    MatrixIterator& operator-=(const difference_type& n)
        {
            MatrixIterator temp = (*this - n);
            *this = temp;
            return *this;
        }

        bool operator==(MatrixIterator const& other) const
        {
            return p_current == other.p_current;
        }

        bool operator!=(MatrixIterator const& other) const
        {
            return p_current != other.p_current;
        }

        bool operator<(const MatrixIterator& other) const
        {
            return p_current < other.p_current;
        }

	    bool operator<=(const MatrixIterator& other) const
        {
            return p_current <= other.p_current;
        }

	    bool operator>(const MatrixIterator& other) const
        {
            return p_current > other.p_current;
        }

	    bool operator>=(const MatrixIterator& other) const
        {
            return p_current >= other.p_current;
        }

	    difference_type operator-(const MatrixIterator& other) const
        {
            return p_current - other.p_current;
        }
    };

};
