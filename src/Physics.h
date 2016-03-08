/**
 * @file		Physics.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		7.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief		Conversion of physical quantities
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

/**
 * Speed of sound in air (at room temperature)
 */
const float   v_Schall=343.2; // m/s

/**
 * @brief Conversion of physical quantities
 */
class Physics
{
public:
	/**
	 * Convert sound seconds to meter distance
	 * @param s seconds
	 * @return meter distance
	 */
	static double secToMeter(double s) { return s*v_Schall; }

	/**
	 * Convert meter distance to sound seconds
	 * @param m meter distance
	 * @return sound seconds
	 */
	static double meterToSec(double m) { return m/v_Schall; }
};

#endif /* PHYSICS_H_ */
