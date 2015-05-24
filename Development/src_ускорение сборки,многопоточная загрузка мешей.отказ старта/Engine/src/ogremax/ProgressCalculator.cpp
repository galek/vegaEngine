#include "EnginePrivate.h"
//Includes---------------------------------------------------------------------
#include "ogremax/ProgressCalculator.h"

namespace vega
{
	using namespace Ogre;


	//Implementation---------------------------------------------------------------
	ProgressCalculator::ProgressCalculator()
	{
		this->progress = 0;
		this->range = 0;
	}

	ProgressCalculator::ProgressCalculator(const Ogre::String& name)
	{
		this->name = name;
		this->progress = 0;
		this->range = 0;
	}

	ProgressCalculator::~ProgressCalculator()
	{
		//Delete child calculators
		for (Calculators::iterator calculator = this->childCalculators.begin();
			calculator != this->childCalculators.end();
			++calculator)
		{
			delete *calculator;
		}
	}

	const String& ProgressCalculator::GetName() const
	{
		return this->name;
	}

	Real ProgressCalculator::GetProgress()
	{
		if (!this->childCalculators.empty())
		{
			//Reset progress
			this->progress = 0;

			//Determine the influence of each child calculator
			Real influence = (Real)1.0 / this->childCalculators.size();

			//Add the progress of all child calculators
			for (Calculators::iterator calculator = this->childCalculators.begin();
				calculator != this->childCalculators.end();
				++calculator)
			{
				this->progress += (*calculator)->GetProgress() * influence;
			}
		}

		return this->progress;
	}

	void ProgressCalculator::SetProgress(Ogre::Real progress)
	{
		this->progress = progress;
	}

	void ProgressCalculator::Update(Real amount)
	{
		if (this->range > 0)
		{
			//Update the progress, scaling it by the inverse range
			this->progress += amount / this->range;

			//Limit the progress to [0, 1]
			this->progress = std::max(this->progress, (Real)0);
			this->progress = std::min(this->progress, (Real)1);
		}
	}

	Real ProgressCalculator::GetRange() const
	{
		return this->range;
	}

	void ProgressCalculator::SetRange(Real range)
	{
		this->progress = 0;
		this->range = range;
	}

	ProgressCalculator* ProgressCalculator::AddCalculator(const Ogre::String& name)
	{
		ProgressCalculator* calculator = new ProgressCalculator(name);
		this->childCalculators.push_back(calculator);
		return calculator;
	}

}