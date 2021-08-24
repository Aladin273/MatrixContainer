#include "matrix_iterator.hpp"
#pragma once

namespace ctd // Custom Template Definition
{
    template <class T>
    class Matrix
    {
    private:
        size_t m_rows = 0;
        size_t m_cols = 0;
        T **m_data = nullptr;
        T m_empty = 0; // не могу придумать, чем заполнять матрицу (нулями), если это не стандартный тип данных,
                       //который мы не можем предугадать
                       // поэтому заставляю пользователя делать это самолично
                       // а делать специализацию для всех числовых типов будет непрактично
    public:
        using iterator = MatrixIterator<T>;

        Matrix(T empty) : m_empty(empty) {}

        Matrix(size_t rows, size_t cols, const T &empty) : m_rows(rows), m_cols(cols), m_empty(empty)
        {
            assert((m_cols > 0) && (m_rows > 0));

            m_data = new T*[m_rows];
            for (size_t i = 0; i < m_rows; ++i)
                m_data[i] = new T[m_cols];

            fill(m_empty);
        }

        Matrix(const Matrix<T> &source)
        {
            if (source.m_data)
            {
                m_rows = source.m_rows;
                m_cols = source.m_cols;
                m_empty = source.m_empty;

                m_data = new T*[m_rows];
                for (size_t i = 0; i < m_rows; ++i)
                    m_data[i] = new T[m_cols];

                for (size_t i = 0; i < m_rows; ++i)
                {
                    for (size_t j = 0; j < m_cols; ++j)
                    {
                        m_data[i][j] = source[i][j];
                    }
                }
            }
        }

        ~Matrix()
        {
            erase();
        }

        iterator begin()
        {
           return iterator(m_data, m_rows, m_cols, true);
        }

        iterator end()
        {
            return iterator(m_data, m_rows, m_cols, false);
        }

        size_t size_rows() const
        {
            return m_rows;
        }

        size_t size_cols() const
        {
            return m_cols;
        }

        size_t rank() const
        {
            if(m_rows > m_cols)
                return m_rows;
            return m_cols;
        }

        void fill(const T &value)
        {
            for (size_t i = 0; i < m_rows; ++i)
            {
                for (size_t j = 0; j < m_cols; ++j)
                {
                   m_data[i][j] = value;
                }
            }
        }

        void transponate()
        {
            size_t rows = m_cols;
            size_t cols = m_rows;

            T **data = new T*[rows];
            for (size_t i = 0; i < rows; ++i)
                data[i] = new T[cols];

            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    data[i][j] = m_data[j][i];
                }
            }

            for (size_t i = 0; i < m_rows; ++i)
                delete[] m_data[i];
            delete[] m_data;

            m_data = data;
            m_rows = rows;
            m_cols = cols;
        }

        void swap(T &a, T&b)
        {
            T temp = a;
            a = b;
            b = temp;
        }

        void swap_rows(size_t index_a, size_t index_b)
        {
            if(index_a == index_b)
                return;

            for (size_t j = 0; j < m_cols; ++j)
            {
                swap(m_data[index_a][j], m_data[index_b][j]);
            }
        }

        void swap_cols(size_t index_a, size_t index_b)
        {
            if(index_a == index_b)
                return;

            for (size_t i = 0; i < m_rows; ++i)
            {
                swap(m_data[i][index_a], m_data[i][index_b]);
            }
        }

        void resize(size_t rows, size_t cols)
        {
            if((m_rows == rows) && (m_cols == cols))
                return;

            if ((m_rows <= 0) || (m_cols <= 0))
            {
                erase();
                return;
            }

            T **data = new T*[rows];
            for (size_t i = 0; i < rows; ++i)
                data[i] = new T[cols];

            fill(m_empty);

            size_t copy_rows = (rows > m_rows) ? m_rows : rows;
            size_t copy_cols = (cols > m_cols) ? m_cols : cols;

            for (size_t i = 0; i < copy_rows; ++i)
            {
                for (size_t j = 0; j < copy_cols; ++j)
                {
                    data[i][j] = m_data[i][j];
                }
            }

            for (size_t i = 0; i < m_rows; ++i)
                delete[] m_data[i];
            delete[] m_data;

            m_data = data;
            m_rows = rows;
            m_cols = cols;
        }

        void erase()
        {
            for (size_t i = 0; i < m_rows; ++i)
                delete[] m_data[i];
            delete[] m_data;

            m_rows = 0;
            m_cols = 0;
            m_data = nullptr;
        }

        void erase_rows(size_t index)
        {
            T **data = new T*[m_rows-1];
            for (size_t i = 0; i < m_rows-1; ++i)
                data[i] = new T[m_cols];

            for (size_t before = 0; before < index; ++before)
            {
                for (size_t j = 0; j < m_cols; ++j)
                {
                    data[before][j] = m_data[before][j];
                }
            }

            for (size_t after = index + 1; after < m_rows; ++after)
            {
                for (size_t j = 0; j < m_cols; ++j)
                {
                    data[after-1][j] = m_data[after][j];
                }
            }

            for (size_t i = 0; i < m_rows; ++i)
                delete[] m_data[i];
            delete[] m_data;

            m_data = data;
            --m_rows;
        }

        void erase_cols(size_t index)
        {
            T **data = new T*[m_rows];
            for (size_t i = 0; i < m_rows; ++i)
                data[i] = new T[m_cols-1];

            for (size_t i = 0; i < m_rows; ++i)
            {
                for (size_t before = 0; before < index; ++before)
                {
                    data[i][before] = m_data[i][before];
                }

                for (size_t after = index + 1; after < m_cols; ++after)
                {
                    data[i][after-1] = m_data[i][after];
                }
            }

            for (size_t i = 0; i < m_rows; ++i)
                delete[] m_data[i];
            delete[] m_data;

            m_data = data;
            --m_cols;
        }

        bool empty() const
        {
            return m_data == nullptr;
        }

        T* operator[](size_t index)
        {
            assert((index >= 0) && (index < m_rows));
            return m_data[index];
        }

        const T* operator[](size_t index) const
        {
            assert((index >= 0) && (index < m_rows));
            return m_data[index];
        }

        Matrix<T>& operator=(const Matrix<T> &source)
        {
            if (this == &source)
                return *this;

            if (source.m_data)
            {
                erase();
                m_rows = source.m_rows;
                m_cols = source.m_cols;
                m_empty = source.m_empty;

                m_data = new T*[m_rows];
                for (size_t i = 0; i < m_rows; ++i)
                    m_data[i] = new T[m_cols];

                for (size_t i = 0; i < m_rows; ++i)
                {
                    for (size_t j = 0; j < m_cols; ++j)
                    {
                        m_data[i][j] = source[i][j];
                    }
                }
            }

            return *this;
        }

        friend Matrix<T> operator+(const Matrix<T> &a, const Matrix<T> &b)
        {
            assert((a.size_rows() == b.size_rows()) && (a.size_cols() == b.size_cols()));

            Matrix temp = a;
            for (size_t i = 0; i < temp.m_rows; ++i)
            {
                for (size_t j = 0; j < temp.m_cols; ++j)
                {
                    temp[i][j] = temp[i][j] + b[i][j];
                }
            }

            return temp;
        }

        friend Matrix<T> operator-(const Matrix<T> &a, const Matrix<T> &b)
        {
            assert((a.size_rows() == b.size_rows()) && (a.size_cols() == b.size_cols()));

            Matrix temp = a;
            for (size_t i = 0; i < temp.m_rows; ++i)
            {
                for (size_t j = 0; j < temp.m_cols; ++j)
                {
                    temp[i][j] = temp[i][j] - b[i][j];
                }
            }

            return temp;
        }

        template<typename T1>
        friend Matrix<T> operator+(const Matrix<T> &a, T1 b)
        {
            Matrix temp = a;
            for (size_t i = 0; i < temp.m_rows; ++i)
            {
                for (size_t j = 0; j < temp.m_cols; ++j)
                {
                    temp[i][j] = temp[i][j] + b;
                }
            }

            return temp;
        }

        template<typename T1>
        friend Matrix<T> operator-(const Matrix<T> &a, T1 b)
        {
            Matrix temp = a;
            for (size_t i = 0; i < temp.m_rows; ++i)
            {
                for (size_t j = 0; j < temp.m_cols; ++j)
                {
                    temp[i][j] = temp[i][j] - b;
                }
            }

            return temp;
        }

        // not works for vectors
        friend Matrix<T> operator*(const Matrix<T> &a, const Matrix<T> &b)
        {
            assert((a.size_cols() == b.size_rows()));

            Matrix temp(a.size_rows(), b.size_cols(), b.m_empty);

            for (size_t i = 0; i < temp.m_rows; ++i)
            {
                for (size_t j = 0; j < temp.m_cols; ++j)
                {
                    for (size_t k = 0; k < temp.m_rows; ++k)
                    {
                        temp[i][j] = temp[i][j] + (a[i][k] * b[k][j]);
                    }
                }
            }

            return temp;
        }

        template<typename T1>
        friend Matrix<T> operator*(const Matrix<T> &a, T1 b)
        {
            Matrix temp = a;
            for (size_t i = 0; i < temp.m_rows; ++i)
            {
                for (size_t j = 0; j < temp.m_cols; ++j)
                {
                    temp[i][j] = temp[i][j] * b;
                }
            }

            return temp;
        }

    };
};
