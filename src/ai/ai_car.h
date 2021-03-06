/************************************************************************/
/*                                                                      */
/* This file is part of VDrift.                                         */
/*                                                                      */
/* VDrift is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* VDrift is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with VDrift.  If not, see <http://www.gnu.org/licenses/>.      */
/*                                                                      */
/************************************************************************/

#ifndef _AI_CAR_H
#define _AI_CAR_H

#include "physics/carinput.h"

#include <vector>
#include <list>

class Car;

/// AI Car controller interface.
class AiCar
{
protected:
	Car* car;
	float difficulty;

	/// Contains the car inputs, which is the output of the AI.
	/// The vector is indexed by CARINPUT values.
	std::vector <float> inputs;

public:
	AiCar(Car* _car, float _difficulty) :
		car(_car), difficulty(_difficulty), inputs(CarInput::INVALID, 0.0)
	{ }
	virtual ~AiCar(){}

	Car*						GetCar() { return car; }
	float						GetDifficulty() { return difficulty; }
	const std::vector<float>&	GetInputs() { return inputs; }

	virtual void Update(float dt, const std::list<Car>& othercars) = 0;

	/// This is optional for drawing debug stuff.
	/// It will only be called, when VISUALIZE_AI_DEBUG macro is defined.
	virtual void Visualize() { }
};

#endif // _AI_CAR_H
