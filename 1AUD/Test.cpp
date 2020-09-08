#include <iostream>
#include "1AUDfilter.h"

using namespace std;

int main()
{
    cout << "1AUD example\n";

    float minCutoff = 100;  // lowest frequency for the dynamic filter (Hz)
    float maxCutoff = 200;  // highest frequency for the dynamic filter (Hz)
    float beta = 0.01;      // cutoff frequency dynamic control, higher values will more 
                            // quickly raise the filter cutoff frequency from min to max
    float sampleRate = 1000; // The rate at which the input data was sampled (samples per second)

    // create an instance of the filter
    OneAUDfilter filter(minCutoff, maxCutoff, beta, sampleRate);

    // update the filter with new values
    for(int x = 0; x < 10; x++)
    {
        float f = filter.update(x);
        cout << f << endl;
    }

    // TODO simulated data with noise, tabulated output for plotting

    return 0;
}