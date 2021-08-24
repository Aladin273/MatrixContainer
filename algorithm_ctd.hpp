#include <iostream>
#pragma once

namespace ctd
{
    //Iterator traits struct
	template <class T>
	struct iter_traits
	{
		using difference_type = typename T::difference_type;
		using value_type = typename T::value_type;
		using pointer = typename T::pointer;
		using reference = typename T::reference;
		using iterator_category = typename T::iterator_category;
	};

    // Template functions
    template <class T>
    void swap(T &a, T&b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

	template <class iterator>
	void print(iterator begin, iterator end)
	{
		while (begin < end)
        {
			std::cout << *begin << " ";
            ++begin;
        }
        std::cout << std::endl;
	}

    template <class iterator>
	iterator find(iterator begin, iterator end, const typename iter_traits<iterator>::value_type& val)
	{
		while ((begin < end) && ((*begin) != val))
        {
            ++begin;
        }

		return begin;
	}


    // Comparision functions
    template<class T>
    bool ascending(T a, T b)
    {
        return a > b;
    }

    template<class T>
    bool descending(T a, T b)
    {
        return a < b;
    }

    template <class iterator>
    void sort(iterator begin, iterator end, bool (*comparisonFcn)(typename iter_traits<iterator>::value_type, typename iter_traits<iterator>::value_type) = ascending)
    {
        // Перебираем каждый элемент матрицы, итератор сам перескакивает между строками
        for(int startIndex = 0; (begin+startIndex) < end; ++startIndex)
        {
            int bestIndex = startIndex;

            for(int currentIndex = startIndex + 1; (begin+currentIndex) < end; ++currentIndex)
            {
                if(comparisonFcn(*(begin+bestIndex), *(begin+currentIndex)))
                    bestIndex = currentIndex;
            }
            swap(*(begin+startIndex), *(begin+bestIndex));
        }
    }
}
