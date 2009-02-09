/// @file
/// @brief Definition of FeatureExtractor class

#include "FeatureExtractor.hpp"
#include "Region.hpp"
#include <boost/timer.hpp>
#include <cmath>


///	@brief	Computes the factorial of an integer.
static unsigned int factorial (const unsigned int& n)
{
	unsigned int result = 1;

	for( unsigned int i = 1; i <= n; ++i )
		result *= i;

	return result;
};


/// @brief	Auxiliary function for computing the scaled Tchebichef polynomial.
static double beta (const unsigned int& n, const unsigned int& N)
{
	double tmp	= factorial(n+N) / static_cast<double>(factorial(2*n + 1) * factorial((n+N) - (2*n+1)));
	double ro	= factorial(2*n) * tmp;	// Equation (8)

	return sqrt(ro);	// Equation (11)
};


///	@brief	Computes the expression of the modified Pochhammer symbol, suitable for simplifying the notation of other equations.
static double modifiedPochhammerSymbol (const unsigned& a, const unsigned& k)
{
	if ( k == 0 )
		return 1.0;

	double tmp = a;
	for ( unsigned int i = 1; i < (k+2); ++i )	// Equation (4)
		tmp *= tmp - i;

	return tmp;
};


///	@brief	Computes the discrete scaled Tchebichef polynomial of order n.
static double scaledTchebichefPolynomial(const unsigned int& n, const unsigned int& x, const unsigned int& N)
{
	double t = 0.0;
	for ( unsigned int k = 0; k <= n; ++k )
	{
		double a = modifiedPochhammerSymbol(x,k);

		double B = modifiedPochhammerSymbol(n-N, n-k);
		double tmp = factorial(n+k) / static_cast<double>(factorial(n-k) * pow(factorial(k),2));
		B = tmp * B;	// Equation (6)

		t += B * a;	// Equation (5)
	}

	return t / beta(n, N);	// Equation (9)
};


///	@brief	Computes the standard 2D Tchebichef moment of order (n + m) of a pattern.
static double tchebichefMoment (const Pattern& pattern, const unsigned int& n, const unsigned int& m)
{
	double t = 0.0;
	for ( unsigned int i = 0; i < pattern.height(); ++i )
	{
		for (unsigned int j = 0; j < pattern.width(); ++j )
		{
			double ti = scaledTchebichefPolynomial(n, i, pattern.height()) / beta(n, pattern.height());
			double tj = scaledTchebichefPolynomial(m, j, pattern.width()) / beta(m, pattern.width());

			t += ti * tj * pattern(i,j);	// Equation (12)
		}
	}
	return t;
};


FeatureExtractor::FeatureExtractor (const std::list<Region>& regions)
:	statistics_(FeatureExtractorStatistics()),
	patterns_(std::vector<Pattern>(0)),
	featureVectors_(std::vector<FeatureVector>(0, FeatureVector(9)))
{
	boost::timer timer;
	timer.restart();

	patterns_.reserve(regions.size());

	for ( std::list<Region>::const_iterator i = regions.begin(); i != regions.end(); ++i )
	{
		Region region(*i);
		region.normalizeCoordinates();
		patterns_.push_back( Pattern(region) );
	}
	statistics_.patternsBuildingTime(timer.elapsed());
};


void FeatureExtractor::computeMoments ()
{
	boost::timer timer;
	timer.restart();

	featureVectors_.reserve(patterns_.size());
	for ( std::vector<Pattern>::iterator k = patterns_.begin(); k != patterns_.end(); ++k )
	{
		FeatureVector fv(9);

		// Area
		fv(0) = tchebichefMoment(*k, 0, 0);

		// Centroid coordinates
		fv(1) = round(tchebichefMoment(*k, 1, 0) / fv(0));	// x-axis
		fv(2) = round(tchebichefMoment(*k, 0, 1) / fv(0));	// y-axis

		// Variance
		fv(3) = tchebichefMoment(*k, 2, 0);
		fv(4) = tchebichefMoment(*k, 0, 2);

		// Skewness
		fv(5) = tchebichefMoment(*k, 0, 3);
		fv(6) = tchebichefMoment(*k, 3, 0);

		// Kurtosis
		fv(7) = tchebichefMoment(*k, 0, 4);
		fv(8) = tchebichefMoment(*k, 4, 0);

		featureVectors_.push_back(fv);
	}

	statistics_.momentsComputingTime(timer.elapsed());
};

