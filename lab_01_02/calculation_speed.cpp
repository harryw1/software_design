#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    // we are using a timing function to compare the speed of 8 byte integer addition,
    // 8 byte intger multiplication, 8 byte double addition, and 8 byte double multiplication

    long long int x = 4;
    long long int y = 4;
    long long int solution = 0;
    double a = 4.0;
    double b = 4.0;
    double solution_d = 0.0;

    // time struct to hold our elapsed time
    clock_t start;

    // start equals the current time
    start = clock();
    // FUNCTION GOES HERE
    for (int i = 0; i < 5000000; i++)
    {
        solution = x + y;
    }
    // ------------------
    // outputs the elapsed time of the function to the
    // terminal
    cout << "Stopwatch (Integer Addition): " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms." << endl;

    start = clock();
    for (int i = 0; i < 5000000; i++)
    {
        solution = x * y;
    }
    cout << "Stopwatch (Integer Multiplication): " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    
    start = clock();
    for (int i = 0; i < 5000000; i++)
    {
        solution_d = a + b;
    }
    cout << "Stopwatch (Double Addition): " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms." << endl;

    start = clock();
    for (int i = 0; i < 5000000; i++)
    {
        solution_d = a * b;
    }
    cout << "Stopwatch (Double Multiplication): " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    return 0;
}