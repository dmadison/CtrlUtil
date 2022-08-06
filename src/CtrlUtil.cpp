/*
 *  Project     Controller Utilities Library
 *  @author     David Madison
 *  @link       github.com/dmadison/CtrlUtil
 *  @license    MIT - Copyright (c) 2022 David Madison
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

#include "CtrlUtil.h"

long remap(long value, long inMin, long inMax, long outMin, long outMax)
{
	if (value <= inMin) return outMin;  // below min threshold
	if (value >= inMax) return outMax;  // above max threshold
	return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;  // otherwise, rescale
}

uint8_t encodePovHat(DPad dpad) {
	// Directional pad (hat) encoding is contained within a single nybble,
	// where cardinal direction starts at north and increases going 
	// clockwise with eight total increments (with 0xF being centered).
	const uint8_t Dpad_Up         = 0;  // 0°
	const uint8_t Dpad_Up_Right   = 1;  // 45°
	const uint8_t Dpad_Right      = 2;  // 90°
	const uint8_t Dpad_Down_Right = 3;  // 135°
	const uint8_t Dpad_Down       = 4;  // 180°
	const uint8_t Dpad_Down_Left  = 5;  // 225°
	const uint8_t Dpad_Left       = 6;  // 270°
	const uint8_t Dpad_Up_Left    = 7;  // 315°
	const uint8_t Dpad_Centered   = 0xF;

	// Simultaneous Opposite Cardinal Directions (SOCD) cleaner
	// up + down is neutral, left + right is neutral
	if (dpad.up && dpad.down) {
		dpad.up = false;
		dpad.down = false;
	}
	if (dpad.left && dpad.right) {
		dpad.left = false;
		dpad.right = false;
	}

	// Logic tree to merge into a single encoded nybble
	uint8_t output;
	if (dpad.up) {
		     if (dpad.left)  output = Dpad_Up_Left;
		else if (dpad.right) output = Dpad_Up_Right;
		else                 output = Dpad_Up;
	}
	else if (dpad.down) {
		     if (dpad.left)  output = Dpad_Down_Left;
		else if (dpad.right) output = Dpad_Down_Right;
		else                 output = Dpad_Down;
	}
	else if (dpad.left) {
		output = Dpad_Left;
	}
	else if (dpad.right) {
		output = Dpad_Right;
	}
	else {
		output = Dpad_Centered;
	}

	return output;
}

uint8_t encodePovHat(bool up, bool down, bool left, bool right) {
	DPad dpad;
	dpad.up    = up;
	dpad.down  = down;
	dpad.left  = left;
	dpad.right = right;
	return encodePovHat(dpad);
}

DPad decodePovHat(uint8_t pov) {
	DPad output;
	output.up    = (pov == 7 || pov <= 1);
	output.right = (pov >= 1 && pov <= 3);
	output.down  = (pov >= 3 && pov <= 5);
	output.left  = (pov >= 5 && pov <= 7);
	return output;
}
