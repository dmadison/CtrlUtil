/*
 *  Project     Controller Utilities Library
 *  @author     David Madison
 *  @link       github.com/dmadison/CtrlUtil
 *  @license    MIT - Copyright (c) 2022 David Madison
 */

#include "CtrlUtil.h"

long remap(long value, long inMin, long inMax, long outMin, long outMax)
{
	if (value <= inMin) return outMin;  // below min threshold
	if (value >= inMax) return outMax;  // above max threshold
	return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;  // otherwise, rescale
}
