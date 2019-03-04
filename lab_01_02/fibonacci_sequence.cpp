#include <iostream>

// prototype for our recursive function
int fib(int number);

int main()
{
    // we need to ask the user for how many terms to display
    int fib_number;
    std::cout << "Please enter a number of fibonicci terms to display: ";
    std::cin >> fib_number;
    // loops while our number of terms is greater than 0
    while (fib_number > 0)
    {
        std::cout << fib(fib_number) << " ";
        // deducts one from terms to display each time weve calculated and
        // displayed a number in the sequence
        fib_number--;
    }
    std::cout << std::endl;
}

//recursivce function so that we cut down on code massively
int fib(int number)
{
    // base case #1
    if (number == 0)
    {
        return 0;
    }
    // base case #2
    if (number == 1)
    {
        return 1;
    }
    // recursively calls fib until we reach our base case
    else
    {
        return fib(number - 1) + fib(number - 2);
    }
}