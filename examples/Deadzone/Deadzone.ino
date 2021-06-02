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
 *  Example:      Deadzone
 *  Description:  Takes an input value from a potentiometer connected to the board's
 *                analog to digital converter (ADC) and runs it through a deadzone
 *                filter.
 *
 *                If the value is within the deadzone, returns the '0' value based on
 *                the alignment of the deadzone in the range (min, center, or max).
 *
 *                If it's outside of the deadzone, rescales the value to encompass the
 *                entire output range (ignoring the area taken up by the deadzone).
 */

#include <CtrlUtil.h>

const int InputPin = A0;

const float ZoneSize = 0.15;  // 15 %
const auto  ZonePosition = DeadzoneFilter::Middle;  // can also set to either Top or Bottom

DeadzoneFilter dz(ZoneSize, ZonePosition);

void setup() {
	Serial.begin(115200);

	pinMode(InputPin, INPUT);
}

void loop() {
	int rawValue = analogRead(InputPin);
	int dzValue = dz.filter(rawValue);

	Serial.print("Raw: ");
	Serial.print(rawValue);
	Serial.print('\t');
	Serial.print("Deadzone Filtered: ");
	Serial.print(dzValue);
	Serial.println();

	delay(100);
}
