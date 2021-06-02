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
 *  Example:      InvertAxis
 *  Description:  Takes an input value from a potentiometer connected to the board's
 *                analog to digital converter (ADC) and inverts it.
 */

#include <CtrlUtil.h>

const int InputPin = A0;

const int AnalogMin = 0;     // min value returned from the ADC
const int AnalogMax = 1023;  // max value returned from the ADC

void setup() {
	Serial.begin(115200);

	pinMode(InputPin, INPUT);
}

void loop() {
	int value = analogRead(InputPin);

	// int inverted = invertAxis(value);  // using 0-1023
	int inverted = invertAxis(value, AnalogMin, AnalogMax);  // using custom values (set above)

	Serial.print("Axis: ");
	Serial.print(value);
	Serial.print('\t');
	Serial.print("Inverted: ");
	Serial.print(inverted);
	Serial.println();

	delay(100);
}
