/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini Benoît Barbot & Hilal Djafri                       *
 * Website: http://www.lsv.ens-cachan.fr/Software/cosmos                       *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 * file parameters.hpp                                                         *
 * Created by Benoit Barbot on 12/12/12.                                       *
 *******************************************************************************
 */

#

#ifndef __Cosmos__HaslFormula__
#define __Cosmos__HaslFormula__

#include <utility>

#include "BatchR.hpp"

enum HaslType {
	EXPECTANCY,
	BOUNDED_EXPECTANCY,
	PROBABILITY,
    PROBCOND,
	HYPOTHESIS,
	PDF_PART,
	CDF_PART,
	CONSTANT,
	HASL_PLUS,
	HASL_MINUS,
	HASL_TIME,
	HASL_DIV,
	RE_AVG,
	RE_Likelyhood,
	RE_Continuous,EXISTS, NOTALLS
};

/**
 *
 * A class to manage Confidence interval.
 */
class ConfInt {
public:
	ConfInt(); //!< Interval representing \f$ \mathbb{R}  \f$.
	ConfInt(double,double,double,double,double); //!< Symetric interval.
	ConfInt(double,double,double,double,double,double); //!< Asymetric interval.
	~ConfInt();
	
	double mean; //!< Middle of the confidence interval.
	double low; //!< Lower bound of the confidence interval.
	double up; //!< Upper bound of the confidence interval.
	double min;
	double max;
    double conf; //! Confidence level

	double width(void); //!< Width of the confidence interval.

    ConfInt &operator+=(const ConfInt& rhs);
    ConfInt &operator-=(const ConfInt& rhs);
    ConfInt &operator*=(const ConfInt& rhs);
    ConfInt &operator/=(const ConfInt& rhs);
};

/**
 * A class to manage and evaluate an HASL formula.
 * An HASL formula is either a probabilistic operator like PROB or EXPECTANCY
 * or combination of HASL formula.
 * The evaluation of an HASL formula require the first and second moment of
 * of the result of a simulation contain in a BatchResult object.
 * The resul of an evaluation is a confidence interval.
 */
class HaslFormulasTop {
public:

	HaslFormulasTop(const HaslType); //!< PROB operator use to compute \f$ \mathbb{P} \f$ .
    HaslFormulasTop(const HaslType,size_t); //!< PROBCOND operator use to compute \f$ \mathbb{P} \f$ .
	HaslFormulasTop(double,double,double); //!< Constant confidence interval.
	HaslFormulasTop(size_t); //!< Expectancy operator use to compute \f$ \mathbb{E} \f$ .
	HaslFormulasTop(double,double); //!< Hypothesis operator: is the given probability above some thershold.
	HaslFormulasTop(HaslType , HaslFormulasTop*, HaslFormulasTop*); //!< Build a tree of operator.
	//	HaslFormulasTop(const HaslFormulasTop&);
	
	~HaslFormulasTop();
	
	bool isConstant(); //!Return true if a formula is constant
	void setLevel(double); //!< set correct confidence level for complex HASL Formula.
	ConfInt eval(const BatchR&)const; //!< Evaluate the formula over the batch of simulation
	double bound()const; //!< Return a bound on the formula result
	HaslType TypeOp; //!< type of the HaslFormula.

    double Value; //!< A double value, have different purpose depending of the type of the operator
	double Value2; //!< A double value, have different purpose depending of the type of the operator

    // Compute chernoff-Hoeffding bounds.
    static void computeChernoff();
	
protected:
	double Level; //!< The confidence level which should be used for the computation of the confidence interval
	
	size_t Algebraic; //!< The index of the data in the table of result return by the simulator.
	HaslFormulasTop* left;
	HaslFormulasTop* right;
};



#endif /* defined(__Cosmos__HaslFormula__) */
