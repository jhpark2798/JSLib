#pragma once
#include "VanillaOption.h"

namespace {
	double nd1(double s, double r, double sigma, double tau, double K);
	double nd2(double s, double r, double sigma, double tau, double K);
}

namespace JSLib {
	
	double blackFormula(OptionType type,
		double s, double r, double sigma, double tau, double K);

}