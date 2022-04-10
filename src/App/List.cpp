#include <iostream>

#include "List.h"

template <class T>
void op(const std::string s, const List<T>& list) {
    std::cout << s << ' ';
    list.for_each([](const T& x) { std::cout << x << ' '; });
    std::cout << '\n';
}

void list_examples() {
    // Comments above each method shows its syntax,
    // explains its function, returning values, notes and cautions,
    // then shows expected output.

    // Iterator
    // Iterator here functions pretty much the same as those of other containers
    // in std.
    std::cout << "--- ITERATOR ---\n";
    {
        List<int> list{1, 2, 3};
        op<int>("list: ", list);

        // Returns an iterator to the first element of the list, i.e. iterator to
        // "1". If the list is empty, returns end iterator (see below).
        List<int>::iterator begin = list.begin();
        List<int>::const_iterator cbegin = list.begin();

        // Returns an iterator to the end of the list.
        // Mind u that this is not at the last element; rather, at an arbitrary
        // element AFTER the last element. So, `--list.end()` actually points to the
        // last element of the list.
        List<int>::iterator end = list.end();
        List<int>::const_iterator cend = list.end();

        // Arithmetic operators
        // (*it_last) == 3, i.e. last element
        auto it_last = end - 1;
        std::cout << "it_last: " << (*it_last) << '\n';
        // (*it_first) == 2
        auto it_first = cbegin + 1;
        std::cout << "it_first: " << (*it_first) << '\n';

        // Usage in for loops
        for (auto it = list.begin(); it != list.end(); ++it) {
            // Modify element's value that `it` is pointing to.
            // Doesn't work if `it` is `const_iterator`.
            (*it) = 1;
        }
        op<int>("list: ", list);
    }


    // Constructor
    std::cout << "\n--- CONSTRUCTOR ---\n";
    {
        {// empty
            List<int> list;
        }
        {
            // 1 2 3
            List<int> list{1, 2, 3};
            op<int>("list: ", list);
            // equivalent to
            // List<int> list = { 1,2,3 };
        }
        {
            List<char> l1 = {'a', 'b', 'c'};
            // a b c
            List<char> list(l1);
            op<char>("list: ", list);
            // equivalent to
            // List<char> list = l1;
        }
        {
            List<int> l1 = {0, 1, 2, 3, 4, 5};
            // 1 2 3 4
            List<int> list(++l1.begin(), --l1.end());
            op<int>("list: ", list);
        }
        {
            // 1 2 3
            List<int> list = List<int>{1, 2, 3};
            op<int>("list: ", list);
        }
    }


    // Methods
    {
        List<int> list;

        // Trivial methods
        std::cout << "\n--- TRIVIAL METHODS ---\n";
        {
            list = {1, 2, 3};

            // True if empty, otherwise false.
            //
            // false
            std::cout << "empty: " << list.empty() << '\n';

            // Returns current size of the list.
            //
            // 3
            std::cout << "size: " << list.size() << '\n';
            std::cout << "length " << list.length() << '\n';

            // Returns reference to the front/back element of the list.
            // Modifiable.
            // Make sure list is non-empty.
            //
            // list: 0 2 3
            list.front() = 0;
            op<int>("list: ", list);
            // list: 0 2 0
            list.back() = 0;
            op<int>("list: ", list);

            // Accessing to an element at an index.
            // Modifiable
            // Make sure the element exists.
            //
            // list: 0 5 0
            list[1] = 5;
            op<int>("list: ", list);
            // list: 10 5 0
            list.at(0) = 10;
            op<int>("list: ", list);

            // Empty a list.
            list.clear();
            op<int>("list: ", list);
        }


        // Insertions
        std::cout << "\n--- INSERTIONS ---\n";
        {
            list = {1, 2, 3};

            // Adds a value to front/back of the list.
            //
            // list: 1 2 3 `100`
            list.push_back(100);
            op<int>("list: ", list);
            // list: `100` 1 2 3 100
            list.push_front(100);
            op<int>("list: ", list);

            // Syntax: insert_at(int index, T value);
            //
            // Inserts value at the specified index.
            // Returns an iterator to the new element.
            // NOTE: this runs in O(n) time complexity.
            //
            // Make sure index <= list.size().
            //
            // list: 100 1 2 3 100 `150`
            list.insert_at(5, 150);
            op<int>("list: ", list);

            // Syntax: insert(iterator pos, T value);
            //
            // Inserts value BEFORE pos in that list.
            // Returns an iterator to the new element.
            //
            // Make sure the specified iterator is pointing to an element of the list,
            // or pointing to list.end().
            //
            // This example is equivalent to push_front(-1).
            // list: `-1` 100 1 2 3 100 150
            list.insert(list.begin(), -1);
            op<int>("list: ", list);

            // Syntax: insert(iterator pos, int count, T value);
            //
            // Inserts counts of value BEFORE pos.
            // Returns an iterator to the start of the inserted range.
            //
            // Make sure the specified iterator is pointing to an element of the list,
            // or pointing to list.end().
            //
            // This example is equivalent to push_back(-10) 2 times.
            // Returns iterator to the first `-10`.
            // list: -1 100 1 2 3 100 150 `-10 -10`
            list.insert(list.end(), 2, -10);
            op<int>("list: ", list);

            // Syntax: insert(iterator pos, iterator first, iterator last)
            //
            // Inserts values in the range of [first, last) BEFORE pos.
            // Returns an iterator to the start of the inserted range.
            // NOTE: this is a copy function, no value or element is moved.
            //
            // list: -1 `20 21 22` 100 1 2 3 100 150 -10 -10
            List<int> list2{20, 21, 22};
            list.insert(++list.begin(), list2.begin(), list2.end());
            op<int>("list: ", list);
        }


        // Removals
        // `list` in here is constantly refilled (due to the nature of this section
        // :v, so keep an eye out.
        std::cout << "\n--- REMOVALS ===\n";
        {
            list = {1, 2, 3};
            // Removes the front/back element.
            //
            // list: 1 2
            list.pop_back();
            op<int>("list: ", list);
            // list: 2
            list.pop_front();
            op<int>("list: ", list);


            list = {1, 2, 3};
            // Syntax: remove_at(int index);
            //
            // Removes the element at the specified index.
            // Returns an iterator to the next element.
            // NOTE: this runs in O(n) time complexity.
            //
            // Make sure index < list.size()
            //
            // Returns iterator to `1`.
            // list: 1 3
            list.remove_at(1);
            op<int>("list: ", list);

            // Syntax: remove(iterator pos);
            //
            // Removes the element pos is pointing to.
            // Returns an iterator to the next element.
            //
            // Make sure the iterator is pointing to an element of the list,
            // and not list.end().
            //
            // Returns iterator to list.end().
            // list: 1
            list.remove(--list.end());
            op<int>("list: ", list);


            list = {1, 2, 1, 3, 2};
            // Syntax: remove(T value);
            //
            // Removes all elements with the speciifed value.
            // Returns the new size of the list.
            //
            // Returns 3
            // list: 2 3 2
            list.remove(1);
            op<int>("list: ", list);
            // Syntax: remove(T value, iterator first, iterator last);
            //
            // Removes in a specified range [first, last).
            //
            // Notice how the second `2` doesn't get removed?
            // list: 3 2
            list.remove(2, list.begin(), --list.end());
            op<int>("list: ", list);


            list = {1, 4, 2, 3, -1, 5};
            // Syntax: remove_if(function<bool(const T&)> func);
            //
            // Removes all elements satisfying some conditions (i.e. when the boolean
            // function returns true). Returns the new size of the list.
            //
            // Returns 3
            // list: 4 3 5
            list.remove_if([](const int& x) { return x <= 2; });
            op<int>("list: ", list);
            // Syntax: remove_if(function<bool(const T&)> func, iterator first, iterator
            // last);
            //
            // Removes in a specified range [first, last).
            //
            // Notice how `4` doesn't get removed?
            // list: 4 5
            list.remove_if([](const int& x) { return x <= 4; }, ++list.begin(),
                           list.end());
            op<int>("list: ", list);
        }


        // Resizing
        // This is the combination of both insertions and removals, ensuring that the
        // resulting list has the length of a speicified value.
        std::cout << "\n--- RESIZING ---\n";
        {
            list = {};
            // Resize the list to a specified length.
            // If more elements need to be added in, default value of current data type
            // will be used (0 for int).
            //
            // list: 0 0 0
            list.resize(3);
            op<int>("list: ", list);
            // list: 0 0
            list.resize(2);
            op<int>("list: ", list);

            // Syntax: resize(int count, T value);
            //
            // Resize the list, but with a specified value instead of default one.
            //
            // list: 0 0 2
            list.resize(3, 2);
            op<int>("list: ", list);
            // list: 0
            list.resize(1, 2);
            op<int>("list: ", list);
        }


        // Assigning
        // This completely replaces the list with something else.
        std::cout << "\n--- ASSIGNING ---\n";
        {
            list = {1, 2, 3};

            // Syntax: assign(int count, T value);
            //
            // Replaces the list with counts of a value.
            //
            // list: 1 1 1
            list.assign(3, 1);
            op<int>("list: ", list);

            // Syntax: assign(iterator first, iterator last);
            //
            // Replaces the list with values from the range of [first, last).
            // NOTE: this is a copy function.
            //
            // Make sure first and last does not point to any element of this list.
            //
            // list: 3 2 1
            List<int> l2{3, 2, 1};
            list.assign(l2.begin(), l2.end());
            op<int>("list: ", list);

            // Aside from `assign` methods, `=` operator functions in the same manner
            // also.
            //
            // list: 3 2 1
            list = {1, 2, 3};  // NOTE: this moves.
            list = l2;         // NOTE: this copies.
            op<int>("list: ", list);
        }


        // Utilities
        // None of the methods below modify the list.
        //
        // Each of the methods below comes with another version that allows performing
        // on a the range of [first, last) (rather than on the whole list). Ex:
        // list.find(<value>, <first_iterator>, <last_iterator>);
        std::cout << "\n--- UTILITIES ---\n";
        {
            list = {2, 2, 3};

            // Syntax: find(T value);
            //
            // Finds the first element of the specified value.
            // Returns iterator at that element. If not found, returns list.end().
            //
            // Returns iterator to the first `2`.
            if (list.find(2) != list.end()) {
                std::cout << "find: Found 2!\n";
            }

            // Syntax: find_if(function<bool(const T&)> func);
            //
            // Finds the first element that causes func to return true.
            //
            // Returns iterator to `3`.
            std::cout << "find_if: "
                      << (*list.find_if([](const int& x) { return x != 2; })) << '\n';

            // Syntax: count(T value);
            //
            // Counts the number of elements of the specified value.
            // Returns the count.
            //
            // 2
            std::cout << "count: " << list.count(2) << '\n';

            // Syntax: count_if(function<bool(const T&)> func);
            //
            // Counts the number of elements that cause func to return true.
            // Returns the count.
            //
            // 3
            std::cout << "count_if: "
                      << list.count_if([](const int& x) { return x > 1; }) << '\n';

            // Syntax: all_of(function<bool(const T&)> func));
            //
            // Checks if all elements cause func to return true.
            // Returns true if all elements satisfies, otherwise false.
            //
            // false
            std::cout << "all_of: " << list.all_of([](const int& x) { return x == 0; })
                      << '\n';

            // Syntax: any_of(function<bool(const T&)> func));
            //
            // Checks if there is AT LEAST ONE element satisfying some conditions (i.e.
            // when boolean function returns true). Returns true if there is one,
            // otherwise false.
            //
            // true
            std::cout << "any_of: " << list.any_of([](const int& x) { return x == 3; })
                      << '\n';

            // Syntax: none_of(function<bool(const T&)> func));
            //
            // The reverse of `any_of`.
            //
            // false
            std::cout << "none_of: "
                      << list.none_of([](const int& x) { return x == 3; }) << '\n';

            // Syntax: for_each(function<void(const T&)> func));
            //
            // Runs a void function on every element.
            // Doesn't return anything.
            //
            // count == 2.
            int count = 0;
            list.for_each([&](const int& x) {
                if (x % 2 == 0) ++count;
            });
            std::cout << "for each: " << count << '\n';

            // Syntax: map<T>(function<TT(const T&)> func));
            //
            // Applies a function to every element, then gathers all returned values to
            // another list. Returns the new list.
            //
            // l2: c c d
            List<char> l2 = list.map<char>([](const int& x) { return x + 'a'; });
            std::cout << "map: ";
            op("l2: ", l2);

            // Syntax: filter(function<bool(const T&)> func));
            //
            // Gathers all satisfying elements to another list.
            // Returns the new list.
            //
            // l3: 2 2
            List<int> l3 = list.filter([](const int& x) { return x == 2; });
            std::cout << "filter: ";
            op("l3: ", l3);

            // Syntax: reduce(function<T(const T&, const T&)> func));
            //
            // Passes every element to a function and gets a cumulative value.
            // Returns value of current data type.
            //
            // sum == 2 + 2 + 3 = 7
            int sum = list.reduce([](const int& x, const int& y) { return x + y; });
            std::cout << "reduce: sum: " << sum << '\n';
            // Syntax: reduce(function<T(const T&, const T&)> func), T initial_value);
            //
            // Second argument is a custom initial value. If not provided,
            // the first element is seen as the initial value.
            //
            // product == 2 * 2 * 3 = 12
            int product =
                    list.reduce([](const int& x, const int& y) { return x * y; }, 1);
            std::cout << "reduce: product: " << product << '\n';
        }


        // Other non-trivial methods
        std::cout << "\n--- NON-TRIVIAL METHODS ---\n";
        {
            // Removes consecutive elements with same value, leaving only one of them.
            //
            // list: 1 2 3 2
            list = {1, 1, 2, 3, 3, 2, 2};
            list.unique();
            op<int>("list: ", list);
            // equivalent to
            // list.unique([](const int& x, const int& y) { return x == y; });

            // Reverses the list.
            // NOTE: this reverses the nodes, not the values.
            //
            // list: 3 2 1
            list = {1, 2, 3};
            list.reverse();
            op<int>("list: ", list);

            // Reverses the value of the list.
            // NOTE: this reverses the values, the nodes stay in their order.
            //
            // list: 3 2 1
            list = {1, 2, 3};
            list.reverse_value();
            op<int>("list: ", list);

            // Merges 2 sorted lists into one.
            // If `list.merge(list2)`, list2 will be empty afterwards.
            //
            // list: 1 2 3 4 5 6
            List<int> l2{2, 4, 6};
            list = {1, 3, 5};
            list.merge(l2);
            op<int>("list: ", list);
            // equivalent to
            // list.merge(l2, [](const int& x, const int& y) { return x < y; });

            // Sorts a list.
            //
            // list: 1 2 3 4
            list = {4, 1, 3, 2};
            list.sort();
            op<int>("list: ", list);
            // equivalent to
            // list.sort([](const int& x, const int& y) { return x < y; });

            // Swaps 2 lists.
            //
            // l3: 1 2 3
            // list: 100 101 102
            list = {1, 2, 3};
            List<int> l3 = {100, 101, 102};
            list.swap(l3);
            op<int>("l3: ", l3);
            op<int>("list: ", list);
        }
    }
}