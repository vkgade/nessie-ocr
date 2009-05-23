/// @file
/// @brief Definition of FeatureExtractor class

#include "FeatureExtractor.hpp"
#include "Pattern.hpp"
#include "FeatureVector.hpp"
#include <boost/timer.hpp>
#include <cmath>


FeatureExtractor::FeatureExtractor ()
:	statistics_(),
	featureVectors_(0)
{}


void FeatureExtractor::computeMoments (const std::vector<Pattern>& patterns)
{
	boost::timer timer;
	timer.restart();

	featureVectors_.reserve(patterns.size());
	for ( std::vector<Pattern>::const_iterator i = patterns.begin(); i != patterns.end(); ++i )
	{
		FeatureVector fv(27);

		double area = imageMoment(*i, 0, 0);
		if ( area == 0.0 )
			area = 1.0;

		std::pair<unsigned int, unsigned int> centroid = i->centroid();
		double xc = centroid.first;
		double yc = centroid.second;

		double eta11 = imageMoment(*i, 1, 1, xc, yc) / pow(area, ((1+1)/2) + 1);
		double eta20 = imageMoment(*i, 2, 0, xc, yc) / pow(area, ((2+0)/2) + 1);
		double eta02 = imageMoment(*i, 0, 2, xc, yc) / pow(area, ((0+2)/2) + 1);
		double eta21 = imageMoment(*i, 2, 1, xc, yc) / pow(area, ((2+1)/2) + 1);
		double eta12 = imageMoment(*i, 1, 2, xc, yc) / pow(area, ((1+2)/2) + 1);
		double eta22 = imageMoment(*i, 2, 2, xc, yc) / pow(area, ((2+2)/2) + 1);
		double eta30 = imageMoment(*i, 3, 0, xc, yc) / pow(area, ((3+0)/2) + 1);
		double eta03 = imageMoment(*i, 0, 3, xc, yc) / pow(area, ((0+3)/2) + 1);
		double eta31 = imageMoment(*i, 3, 1, xc, yc) / pow(area, ((3+1)/2) + 1);
		double eta13 = imageMoment(*i, 1, 3, xc, yc) / pow(area, ((1+3)/2) + 1);
		double eta32 = imageMoment(*i, 3, 2, xc, yc) / pow(area, ((3+2)/2) + 1);
		double eta23 = imageMoment(*i, 2, 3, xc, yc) / pow(area, ((2+3)/2) + 1);

		fv.at(0) = 0.0;		// m00
		fv.at(1) = xc;		// m10
		fv.at(2) = yc;		// m01
		fv.at(3) = eta11;	// m11
		fv.at(4) = eta20;	// m20
		fv.at(5) = eta02;	// m02
		fv.at(6) = eta21;	// m21
		fv.at(7) = eta12;	// m12
		fv.at(8) = eta22;	// m22
		fv.at(9) = eta30;	// m30
		fv.at(10) = eta03;	// m03
		fv.at(11) = eta31;	// m31
		fv.at(12) = eta13;	// m13
		fv.at(13) = eta32;	// m32
		fv.at(14) = eta23;	// m23

		featureVectors_.push_back(fv);
	}

	try
	{
		statistics_.momentsComputingTime(timer.elapsed());
	}
	catch(...) {}
}


double FeatureExtractor::imageMoment (const Pattern& pattern, const unsigned int& p, const unsigned int& q, const double& xc, const double& yc) const
{
	double t = 0.0;
	for ( unsigned int i = 0; i < pattern.height(); ++i )
	{
		double ti = pow(i-xc, p);

		for (unsigned int j = 0; j < pattern.width(); ++j )
		{
			double tj = pow(j-yc, q);

			t += ti * tj * pattern.at(i,j);
		}
	}
	return t;
}

