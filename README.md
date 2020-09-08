## The 1AUD filter, a modification of the [1Euro](http://cristal.univ-lille.fr/~casiez/1euro/) filter

This software is released under the MIT license; Copyright 2020 C Thompson, J Kingdon.

## Introduction

The 1Euro filter increases the cutoff frequency of a first order filter during rapid input movements.  The input is smoothed with a low frequency first order filter (1hz in most examples) before its derivative is used to modulate the cutoff frequency.

This filter uses the same principle with some modifications that appear to improve performance in certain applications, such as tracking the movement of a gimbal.

### Changes from the 1Euro

- Adding a slew limiting function to remove random spikes (optional, but useful)
- Using a second order filter before taking the derivative, to better smooth the derivative (and the output) with improved delay in responding to movement. 
- Using a second order filter on the output to gain better output smoothing

## Sample code

Here is an example of [how this filter can be coded](https://github.com/ctzsnooze/1AUD-filter/tree/master/1AUD).

## Sample results

![Gimbal smoothing example](https://github.com/ctzsnooze/1AUD-filter/blob/master/images/AUD_Filter_GimbalSmoothing.jpg "Gimbal smoothing example, longer min time constant")

![Gimbal smoothing example](https://github.com/ctzsnooze/1AUD-filter/blob/master/images/AUD_Filter_on_GimbalData.jpg "Gimbal smoothing example, shorter min time constant")

![Gimbal smoothing example](https://github.com/ctzsnooze/1AUD-filter/blob/master/images/AUDFilter_GimbalSmoothingLong.jpg "Gimbal smoothing example, showing faster cutoff and less delay during faster movements.")

## Tuning:

The user must set:
- the input cutoff frequency for smoothing before differentiation, to get a clean enough derivative control signal.  
- minimum and maximum cutoff frequencies for the smoothing function
- the 'beta' or gain factor for how quickly the cutoff should rise during fast movement.  

Stronger input smoothing will lead to a smoother derivative and less output jitter, at the cost of greater delay in changing output cutoff frequency.  

## Code basics:
- (optionally) apply a slew limiter to remove extreme solitary spikes
- apply a second order lowpass filter to smooth the data / suppress high frequency noise
- calculate the derivative of that smoothed input signal
- shift the minCutoff value upwards towards a maximum as derivative increases
- calculate the lowpass constant k for the final smoothing function
- constrain k to 1 or to the value that limits the maximum cutoff frequency
- apply a (first or second order) lowpass with this variable k factor over the (slew limited) input data

In pseudocode:

```
  ## Calculate lowpass k value
kInput = 2*PI*inputCutoff/sampleRate;

 ## (optional) slew limit input steps to 20 max
slewedInput = slewLimit(input);

 ## First order PT1 that slewed input
smoothedSlewedInput = prevSlewedInput + kInput * (slewedInput - prevSlewedInput);
prevSlewedInput = smoothedSlewedInput;

 ## Repeat to get second order smoothing
verySmoothedSlewedInput = prevVerySmoothedSlewedInput + kInput * (smoothedSlewedInput - prevVerySmoothedSlewedInput);
prevVerySmoothedSlewedInput = verySmoothedSlewedInput;

  ## get differential of very smoothed slewed input
dx = (verySmoothedSlewedInput-prevVerySmoothedSlewedInput) * sample rate;

  ## adjust cutoff upwards using dx and Beta
cutoff = minCutoff + Beta * ABS(dx);

  ## limit the highest possible cutoff frequency to the user-configured maximum
cutoff = min(cutoff, maxCutoff);

  ## get the k value for the cutoff 
kCutoff = 2*PI*cutoff/sampleRate

  ## limit kCutoff to a value that sets the upper limit of the cutoff
  ## NB: kCutoff must not exceed 1; at one, there is no filtering
kCutoff = MAX (kCutoff, 1);

  ## use kCutoff to filter the slewed input 
oneAudOut = prevOneAudOut + kCutoff * (slewedInput - prevOneAudOut)
prevOneAudOut = oneAudOut;

  ## for second order / smoother filtering
oneAudOut = prevOneAudOut + kCutoff * (oneAudOut - prevOneAudOut);
prevOneAudOut = oneAudOut;
output = oneAudOut;
```

I apologise if someone else has described this approach before.

