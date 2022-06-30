/*
 *  Project     Controller Utilities Library
 *  @author     David Madison
 *  @link       github.com/dmadison/CtrlUtil
 *  @license    MIT - Copyright (c) 2021 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef CTRLUTIL_H
#define CTRLUTIL_H

#include <stdint.h>

long remap(long value, long inMin, long inMax, long outMin, long outMax);


template <typename T>
constexpr T invertAxis(T value, T adc_min, T adc_max)
{
	return 
		value >= adc_max ? adc_min :  // check if at max
		value <= adc_min ? adc_max :  // check if at min
		(adc_max - value) + adc_min;  // otherwise calculate
}

template <typename T>
constexpr T invertAxis(T value)
{
	return invertAxis<T>(value, 0, 1023);  // ADC defaults for AVR
}


template <typename T>
T recenterAxis(T value, T inMin, T inCenter, T inMax, T outMin, T outMax)
{
	// sort input range values so min is lower than max
	if (inMin > inMax) {
		T temp = inMin;
		inMin = inMax;
		inMax = temp;
	}

	// quit if center is out of range
	if (inCenter < inMin || inCenter > inMax) return value;

	// calculate output center for range checks
	const long outCenter = (long) (outMin + outMax) / 2;

	// evaluate range for lower range
	if (value <= inCenter) {
		return remap(value, inMin, inCenter, outMin, outCenter);
	}
	// otherwise, evaluate range for upper range
	else {
		return remap(value, inCenter, inMax, outCenter, outMax);
	}
}


template <typename T>
class DeadzoneFilterType {
public:
	enum Alignment : uint8_t {
		Bottom = 0,
		Middle = 1,
		Top = 2,
	};

	DeadzoneFilterType(float dz, Alignment align = Alignment::Middle)
		: DeadzoneFilterType(dz, 0, 1023, align)  // typical 10-bit ADC values
	{}

	DeadzoneFilterType(float dz, T min, T max, Alignment align = Alignment::Middle)
		: alignment(align)
	{
		this->setRangeInternal(min, max);  // validate and set range min, max, center
		this->setDeadzoneInternal(dz);  // validate and set deadzone float
		this->recalculate();  // calculate deadzone range value
	}

	T filter(T input, long outMin, long outMax) {
		T output;

		switch (alignment) {
		case(Alignment::Top):
			if (input > deadzoneThreshold)
				output = outMax;  // in deadzone, output top of range
			else
				output = remap(input, rangeMin, deadzoneThreshold, outMin, outMax);
			break;
		case(Alignment::Bottom):
			if (input < deadzoneThreshold)
				output = outMin;  // in deadzone, output bottom of range
			else
				output = remap(input, deadzoneThreshold, rangeMax, outMin, outMax);
			break;
		case(Alignment::Middle):
		{
			const T thresholdLow = rangeCenter - deadzoneThreshold;
			const T thresholdHigh = rangeCenter + deadzoneThreshold;
			const long outCenter = (outMin + outMax) / 2;

			if (input < thresholdLow)
				output = remap(input, rangeMin, thresholdLow, outMin, outCenter);  // low, remap to lower half
			else if (input > thresholdHigh)
				output = remap(input, thresholdHigh, rangeMax, outCenter, outMax);  // high, remap to upper half
			else
				output = outCenter;  // in deadzone, output center
			break;
		}
		default:
			output = input;  // if alignment is set to an unenumerated value
		}

		return output;
	}

	T filter(T input) {
		return filter(input, rangeMin, rangeMax);
	}

	void setDeadzone(float dz) {
		if (deadzone == dz) return;
		this->setDeadzoneInternal(dz);
		this->recalculate();
	}

	void setAlignment(Alignment align) {
		if (alignment == align) return;
		alignment = align;
		this->recalculate();
	}

	void setRange(T max) {
		setRange(0, max);
	}

	void setRange(T min, T max) {
		if (rangeMin == min && rangeMax == max) return;
		this->setRangeInternal(min, max);
		this->recalculate();
	}

private:
	Alignment alignment;  // alignment of the deadzone in the input range
	T rangeMin;  // lower bound of the input range
	T rangeMax;  // upper bound of the input range
	T rangeCenter;  // middle of the input range (save on recalculating this on the fly)

	float deadzone;   // size of the deadzone, in percent (0.0 - 1.0)
	T deadzoneThreshold;  // threshold for the deadzone, in units of the range

	void setRangeInternal(T min, T max) {
		if (min > max) {  // inverse range, swap values
			T temp = min;
			min = max;
			max = temp;
		}
		rangeMin = min;
		rangeMax = max;
		rangeCenter = (max + min) / 2;
	}

	void setDeadzoneInternal(float dz) {
		if (dz < 0.0) dz = 0.0;
		else if (dz > 1.0) dz = 1.0;
		deadzone = dz;
	}

	void recalculate() {
		const T range = rangeMax - rangeMin;  // total range
		const T deadzoneSize = range * deadzone;  // deadzone in units of the range, total size

		switch (alignment) {
		case(Alignment::Top):
			deadzoneThreshold = rangeMax - deadzoneSize;  // store as high level, anything above this is in deadzone
			break;
		case(Alignment::Bottom):
			deadzoneThreshold = rangeMin + deadzoneSize;  // store as low level, anything below this is in deadzone
			break;
		case(Alignment::Middle):
			deadzoneThreshold = deadzoneSize / 2;  // store deadzone as half of each side, for easier comparison
			break;
		}
	}
};

using DeadzoneFilter = DeadzoneFilterType<long>;


#endif
