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

#include <iostream>
#include <limits>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/binomial.hpp>

#include "HaslFormula.hpp"
#include "parameters.hpp"

/**
 * Trivial confidence interval containing all R.
 */
ConfInt::ConfInt(){
    mean = 0;
    low = - std::numeric_limits<double>::infinity();
    up = std::numeric_limits<double>::infinity();
    min = - std::numeric_limits<double>::infinity();
    max = std::numeric_limits<double>::infinity();
    conf = 1;
}

/**
 * Symetric confidence interval
 * @param meanArg is the middle of the confidence interval
 * @param width is the width of the confidence interval
 * @param min the smallest value observed
 * @param max the maximal value observed
 */
ConfInt::ConfInt(double meanArg,double width,double minval,double maxval,double c){
    mean = meanArg;
    low = meanArg - width/2;
    up = meanArg + width/2;
    min= minval;
    max= maxval;
    conf=c;
}

/**
 * Asymetric confidence interval.
 * @param meanArg is the mean of the confidence interval it is not necessary
 * the middle of the confidence interval if the distribution is not symetric
 * and is not approximated by the normal distribution wich is symetric.
 * @param lowArg the Lower bound of the confidence interval.
 * @param upArg the Upper bound of the confidence interval.
 * @param min the smallest value observed
 * @param max the maximal value observed
 */
ConfInt::ConfInt(double meanArg,double lowArg,double upArg,double minval,double maxval,double c){
    mean = meanArg;
    low = lowArg;
    up = upArg;
    min= minval;
    max= maxval;
    conf =c;
}

ConfInt::~ConfInt(){}

/**
 * Return the width of a confidence interval
 */
double ConfInt::width(){
    return (up-low);
}

ConfInt & ConfInt::operator+=(const ConfInt& rhs){
    low += rhs.low;
    mean += rhs.mean;
    up += rhs.up;
    min += rhs.min;
    max += rhs.max;
    conf = conf +rhs.conf -1;
    return *this;
}

ConfInt & ConfInt::operator-=(const ConfInt& rhs){
    low -= rhs.up;
    mean -= rhs.mean;
    up -= rhs.low;
    min -= rhs.max;
    max -= rhs.min;
    conf = conf +rhs.conf -1;
    return *this;
}

ConfInt & ConfInt::operator*=(const ConfInt& rhs){
    mean = mean*rhs.mean;
    low = fmin(fmin(low*rhs.low,up*rhs.up),
               fmin(low*rhs.up,up*rhs.low));
    up = fmax(fmax(low*rhs.low,up*rhs.up),
              fmax(low*rhs.up,up*rhs.low));
    min = fmin(fmin(min*rhs.min,max*rhs.max),
               fmin(min*rhs.max,max*rhs.min));
    max = fmax(fmax(min*rhs.min,max*rhs.max),
               fmax(min*rhs.max,max*rhs.min));
    conf = conf +rhs.conf -1;
    return *this;
}

ConfInt & ConfInt::operator/=(const ConfInt& rhs){
    mean /= rhs.mean;

    if(rhs.low * rhs.up<=0){
        low = - std::numeric_limits<double>::infinity();
        up = std::numeric_limits<double>::infinity();
    } else {
        if(rhs.low > 0){
            low /= rhs.up;
            up /= rhs.low;
        }else{
            low /= rhs.low;
            up  /= rhs.up;
        }
    }
    if(rhs.min * rhs.max<=0){
        min = - std::numeric_limits<double>::infinity();
        max = std::numeric_limits<double>::infinity();
    } else {
        if(rhs.min > 0){
            min /= rhs.max;
            max /= rhs.min;
        }else{
            min /= rhs.min;
            max  /= rhs.max;
        }
    }
    conf = conf +rhs.conf -1;
    return *this;
}

/**
 * Build an HASL formula with the PROB,EXISTS,ALLS operators.
 * Compute exact confidence interval.
 */
HaslFormulasTop::HaslFormulasTop(const HaslType t){
    assert( t==PROBABILITY || t==EXISTS || t== NOTALLS);
    TypeOp = t;
    Level =0;
    Value =0;
    Value2=0;
    Algebraic = 0;
    left = NULL;
    right = NULL;
}

/**
 * Build an HASL formula with the PROB,EXISTS,ALLS operators.
 * Compute exact confidence interval.
 */
HaslFormulasTop::HaslFormulasTop(const HaslType t,size_t al){
    assert( t==PROBCOND);
    TypeOp = t;
    Level =0;
    Value =0;
    Value2=0;
    Algebraic = al;
    left = NULL;
    right = NULL;
}

/**
 * Build a CONSTANT Hasl formula.
 * @param v the value of the constant
 */
HaslFormulasTop::HaslFormulasTop(double v1,double v2,double l){
    TypeOp = CONSTANT;
    Level = l;
    Value = v1;
    Value2= v2;
    Algebraic = 0;
    left = NULL;
    right = NULL;
}

/**
 * Build an HASL formula computing the expected value of a subformula.
 * Use the approximation to the normal low to compute the confidence interval
 * @param al the index of the sub formula return by the simulator.
 * @param l the confidence level of the confidence interval.
 */
HaslFormulasTop::HaslFormulasTop(size_t al){
    TypeOp = EXPECTANCY;
    Level = 0;
    Value = 0;
    Value2 = 0;
    Algebraic = al;
    left = NULL;
    right = NULL;
}

/**
 * Build an HASL formula testing if the probability of accepting a run is above some threshold.
 * In this setting the confidence level is the probability of type
 * I and type II errors.
 * @param p is the threshold.
 * @param delta is the half width of the indiference region.
 */
HaslFormulasTop::HaslFormulasTop(double p, double delta){
    TypeOp = HYPOTHESIS	;
    Level = 0;
    Value = p -delta;
    Value2= p + delta;
    Algebraic = 0;
    left = NULL;
    right = NULL;
}


/*
 * Build an Hasl formula of a given type as a tree with two descendant.
 * @param t is the type of the node either HASL_PLUS, HASL_TIME or HASL_DIV.
 * @param l The left Hasl Formula.
 * @param r the right Hasl formula.
 */
HaslFormulasTop::HaslFormulasTop(HaslType t, HaslFormulasTop* l,HaslFormulasTop* r){
    TypeOp = t;
    Algebraic = 0;
    Level = 0;
    Value = 0;
    Value2= 0;
    left = l;
    right = r;
}

/**
 * Destructor call destructor recusively on descendant if neaded.
 */
HaslFormulasTop::~HaslFormulasTop(){
    if(!left) delete left;
    if(!right) delete right;
}

/**
 * This method return true if a haslformula is constant.
 * This is usfull to know if a subformula have a confidence level
 * different than 1.
 */
bool HaslFormulasTop::isConstant(){
    switch (TypeOp) {
        case CONSTANT:
            return true;

        case HASL_PLUS:
        case HASL_TIME:
        case HASL_DIV:
            return (left->isConstant() && right->isConstant());
        default:
            return false;
    }
}

/**
 * This function set the confidence level of an HASL formula.
 * When the fomula contain
 * @param l The confidence level
 */
void HaslFormulasTop::setLevel(double l){
    switch (TypeOp) {
        case HYPOTHESIS:
        case PROBABILITY:
        case PROBCOND:
            Level = l;
            break;

        case EXPECTANCY:
        case CDF_PART:
        case PDF_PART:
        case RE_Likelyhood:
            Level = l;
            Value = quantile(boost::math::normal() , 0.5 + l / 2.0);
            break;

        case RE_AVG://temporary
            Level = l;
            //Here we divide the error by two because half the error come from the bernoulli evaluation.
            Value = quantile(boost::math::normal() , 0.75 + l / 4.0);
            break;

        case RE_Continuous:
            Level = l;
            Value = quantile(boost::math::normal() , 0.75 + l / 4.0);
            break;

        case CONSTANT:
            break;

        case HASL_PLUS:
        case HASL_MINUS:
        case HASL_TIME:
        case HASL_DIV:
            Level=l;
            if (left->isConstant()){right->setLevel(l);}
            else if (right->isConstant()){left->setLevel(l);}
            else{
                //We need to increase the condidence level on each subformula
                left->setLevel((1+l)/2);
                right->setLevel((1+l)/2);
            }
            break;
        case NOTALLS:
        case EXISTS:
            Level =1;
            break;

        default:
            std::cerr << "Fail to parse Hasl Formula"<< std::endl;
            exit(EXIT_FAILURE);
    }
}


void HaslFormulasTop::computeChernoff(){
    double b = 0.0;
    for(let it : P.HaslFormulas) b = fmax(b,it->bound());
    if(b== INFINITY){
        std::cerr << "Cannot use Chernoff-Hoeffding bounds: no bounds on the computed value" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(P.MaxRuns== (unsigned long)-1){
        P.MaxRuns = (int)(2.0*2.0*2.0*b*b/(P.Width*P.Width) * log(2/(1-P.Level)));
    }else if(P.Width == 0){
        P.Width = 2.0*b * sqrt( (2.0/P.MaxRuns) * log(2.0/(1.0-P.Level)));
    }else if(P.Level ==0){
        P.Level = (1.0 - (2.0* fmin(0.5 ,exp( (double)P.MaxRuns * P.Width*P.Width / (-2.0*2.0*2.0*b*b)))));
    }
}


/**
 * The function eval compute confidence interval for the HASL formula
 * from the simualation result return by the simulators.
 * It always return a confdience interval containing a confidence level
 * Depending of the context it should be interpreted differently.
 * In sequential setting, one should check that the intervals satisfy the
 * stopping criteria.
 * In static setting, one should check independantly the number of sample to
 * decide whether the simulation should be stopped.
 *
 * Some remarks about the estimation of the confidence interval adopted here
 * Let \f$ l \f$ =ConfLevel, the confidence level
 * \f$ l=1-alpha \f$
 * Let \f$ w \f$ = ConfWidth, the size of the confidence interval
 */
ConfInt HaslFormulasTop::eval(const BatchR &batch)const{
    switch (TypeOp) {
        case PROBABILITY:

            //if (P.Width ==0)// In static setting use exact binomial computation
            {
                /*
                 * Here we used the boost library for computing the binomial
                 * confidence interval.
                 * According to boost documentation the algorithme used is
                 * Clopper-person:
                 * Clopper, C. J. and Pearson, E. S. (1934). The use of confidence or fiducial limits illustrated in the case of the binomial. Biometrika 26 404-413.
                 */
                double mean = (double)batch.Isucc / (double)batch.I;
                double l = boost::math::binomial_distribution<>::find_lower_bound_on_p(batch.I,batch.Isucc, (1-Level)/2);
                double u = boost::math::binomial_distribution<>::find_upper_bound_on_p(batch.I,batch.Isucc, (1-Level)/2);

                if(batch.Isucc ==0)return ConfInt(mean,l,u,0.0,0.0,Level);
                if(batch.Isucc == batch.I)return ConfInt(mean,l,u,1.0,1.0,Level);

                return ConfInt(mean,l,u,0.0,1.0,Level);
            } // In Sequential case fall back to show robin algorithm

            // NO BREAK on purpose here !
        case PROBCOND:
        {
            //Same as PROB but with a more specific constraint
            double mean = (double)batch.bernVar[Algebraic] / (double)batch.Isucc;
            double l = boost::math::binomial_distribution<>::find_lower_bound_on_p(batch.Isucc,batch.bernVar[Algebraic], (1-Level)/2);
            double u = boost::math::binomial_distribution<>::find_upper_bound_on_p(batch.Isucc,batch.bernVar[Algebraic], (1-Level)/2);

            if(batch.bernVar[Algebraic] ==0)return ConfInt(mean,l,u,0.0,0.0,Level);
            if(batch.bernVar[Algebraic] == batch.Isucc)return ConfInt(mean,l,u,1.0,1.0,Level);

            return ConfInt(mean,l,u,0.0,1.0,Level);

        }

            /*
             * Here Gaussian confidence interval are computed.
             * Let \f$ \mu \f$ the value to estimate, and \f$ x \f$ the
             * estimation of \f$ \mu \f$
             * then \f$ \mathbb{P}( \mu \in [x-\frac{w}{2} , x+\frac{w}{2}]) = 1-alpha  \f$
             *
             * The gaussian confidence interval is given by :
             * \f$  [x-z(1-\frac{alpha}{2}) * \frac{StandardDeviation~ }
             * { \sqrt{NumberOfObservations}} ,
             *         x+z(1-\frac{alpha}{2}) * \frac{StandardDeviation~}
             * { \sqrt{NumberOfObservations}}] \f$
             * with : \f$ z(1-\frac{alpha}{2})=z(1-\frac{1-l}{2}) = z(0.5+\frac{l}{2}) \f$
             *
             * \f$ StandartDeviation~ = \sqrt{ Variance +\frac{1}{n} } \f$
             * This correction come from the Chows and Robbin algorithm to ensure
             * The correctness of the stopping condition.
             *
             * We use the boost library: quantile(boost::math::normal() , level)
             * to compute quantile of the normal low. Its value have been stored
             * in Value by the function setLevel.
             */
        case EXPECTANCY:
        case CDF_PART:
        case PDF_PART:
        {
        //No accepting trajectory the condidence interval is R.
        if(batch.Isucc==0)return ConfInt();

        double mean = batch.Mean[Algebraic]/batch.Isucc;
        double m2 = batch.M2[Algebraic]/batch.Isucc;
        double variance = m2 - mean * mean;

        if(P.Width!=0){ //P.width =0 indicate static setting
            variance += 1.0/batch.Isucc;
            //Here the +1 come from the Chows and Robbin algorithm in Sequential setting
        }

        double width = 2 * Value * sqrt(variance/(batch.Isucc -1));

        return ConfInt(mean,width,batch.Min[Algebraic],batch.Max[Algebraic],Level);
        }

        case RE_Likelyhood:
        {
        //No accepting trajectory the condidence interval is R.
        if(batch.Isucc==0)return ConfInt();
        double mean = batch.Mean[Algebraic]/batch.Isucc;
        double m2 = batch.M2[Algebraic]/batch.Isucc;

        //If the variance is equal to 0 numerical instability can make it a negative number.
        double variance = fmax(0,m2 - mean * mean);

        double width = 2 * Value * sqrt(variance/batch.Isucc);

        return ConfInt(mean,width,batch.Min[Algebraic],batch.Max[Algebraic],Level);
        }

        case RE_AVG://temporary
        {
        //No accepting trajectory => the condidence interval is R.
        if(batch.Isucc==0)return ConfInt();

        double mean = (double)batch.Isucc / (double)batch.I;
        double l = boost::math::binomial_distribution<>::find_lower_bound_on_p(batch.I,batch.Isucc, (1-Level)/2);
        double u = boost::math::binomial_distribution<>::find_upper_bound_on_p(batch.I,batch.Isucc, (1-Level)/2);
        double mean2 = batch.Mean[Algebraic]/batch.Isucc;
        double m2 = batch.M2[Algebraic]/batch.Isucc;
        double variance = fmax(0,m2 - mean2 * mean2);
        double width = Value * sqrt(variance/batch.Isucc);

        return ConfInt(mean*mean2,
                       (mean2 - width)*l,
                       (mean2 + width)*u,0.0,1.0 * batch.Max[Algebraic],Level );
        }

        case RE_Continuous:
        {
        /*
         * Only used by the rare event engine in for countinuous bounded
         * Until property. This case apply the uniformization method.
         * The simulator returns the estimate of each \mu_u as well
         * as the Poisson probabilities computed using Fox-glynn algorithm.
         *
         */
        //batch.print();
        size_t N = batch.Mean.size()/2;
        std::cout << "tablelength = "<< batch.Mean.size() << std::endl;
        double error = 1- Level;
        double LevelN = 1- error/(N+2);
        double ValueN = quantile(boost::math::normal() , (1+LevelN)/2);

        ConfInt totalInt = ConfInt(0.0,0.0,0.0,0.0,Level);
        ConfInt likelyhood = ConfInt(0.0,0.0,0.0,0.0,Level);
        ConfInt reacheabilityprob =  ConfInt(0.0,0.0,0.0,0.0,Level);
        ConfInt poisson = ConfInt(0.0,0.0,0.0,0.0,Level);

        if(P.verbose>1)std::cerr << "i,\tsucc,\tbatch,\tMean,\tM2,\tMin,\tMax,\tPoisson" << std::endl;
        if(P.verbose>1)std::cerr << "continuous step:\t" << P.continuousStep <<  std::endl;
        for(size_t i=0; i< N; i++){
            int i2 = (i>0 && P.continuousStep>1.0 )? i-1 : i;

            //evaluate the likelyhood:
            likelyhood.mean = batch.Mean[2*i+1]/batch.Mean[2*i];
            double var = (batch.M2[2*i+1]/batch.Mean[2*i]) - pow((batch.Mean[2*i+1]/batch.Mean[2*i]),2);
            double widthN = 0.0;
            if(var>0)widthN = 2* ValueN * sqrt(var/batch.Mean[2*i]);
            likelyhood.low = batch.Min[2*i2+1];
            likelyhood.up = batch.Max[2*i+1];  //likelyhood->mean + widthN/2.0;
            likelyhood.min = batch.Min[2*i2+1];
            likelyhood.max = batch.Max[2*i+1];

            //evaluate probability to reach final state:
            reacheabilityprob.mean = batch.Mean[2*i]/batch.M2[2*i];
            reacheabilityprob.low = boost::math::binomial_distribution<>::find_lower_bound_on_p(batch.M2[2*i2],batch.Mean[2*i2], (1-LevelN)/2);
            reacheabilityprob.up = boost::math::binomial_distribution<>::find_upper_bound_on_p(batch.M2[2*i],batch.Mean[2*i], (1-LevelN)/2);
            reacheabilityprob.min = 0.0;
            reacheabilityprob.max = 1.0;
            //evaluate poisson:
            poisson.mean = batch.Min[2*i];
            poisson.low = (1-Value2)*batch.Min[2*i];
            poisson.up = batch.Min[2*i];
            poisson.min=poisson.low;
            poisson.max=poisson.max;

            if(P.verbose>1){
                std::cout << "i: " << i<< "\tsucc: "<< batch.Mean[2*i]<< "\tLikelyhood: "  << likelyhood.mean << "\t[" << likelyhood.low<<";"<<likelyhood.up << "]\twidth: "<< widthN <<"\tpoisson: " << batch.Min[2*i] << "\tconfint: ["<< poisson.low <<";"<< poisson.up << "]";
                std::cerr << i << ",\t" << batch.Mean[2*i] << ",\t" << batch.M2[2*i] << ",\t" << batch.Mean[2*i+1]/batch.Mean[2*i] << ",\t" << batch.M2[2*i+1]/batch.Mean[2*i] << ",\t" << batch.Min[2*i+1] << ",\t" << batch.Max[2*i+1] << ",\t" << batch.Mean[2*i] << std::endl;
            }

            //Multiply the three confidence interval:
            poisson *= likelyhood;
            poisson *= reacheabilityprob;

            if(P.verbose>1){
                std::cout << "\tresult: ["<< poisson.low <<";"<< poisson.up << "]" << std::endl;
            }


            //Add the confidence interval to the total one.
            //if(poisson->width() < 1.0)
            totalInt += poisson;

        }

        //Add the error made on the left of the truncation point.
        ConfInt leftTruncationError =
        ConfInt(0.0,0.0,batch.Mean[1]/batch.M2[0]* Value2/2,0,batch.Mean[1]/batch.M2[0]* Value2/2,Level);

        //Add the error made on the rigth trucation point TODO!
        ConfInt rightTruncationError = ConfInt(0.0,0.0,0.0,0.0,Level);

        totalInt += leftTruncationError;
        totalInt += rightTruncationError;

        return totalInt;
        }

        case HYPOTHESIS:
        {
        /*
         * Implementation of the SPRT.
         *  Value < Value2
         *  Value <= p <= Value2 => Indiferrence region
         * H0 = { Value2 < p  <= 1    } => p0=Value2
         * H1 = { 0      <= p < Value } => p1=Value
         *
         * To comply with the architecture of cosmos requiring confidence
         * interval this function also return confidence interval.
         * Let l be the minimal confidence level for wich either H0 or H1 is
         * accepted
         * return [0,Value2] with confidence level l when H0 is accepted
         * return [Value, 1] with confidence level l when H1 is accepted
         *
         * The stopping criterion is reached when l is greater than the expected
         * level. This stopping criterion is exaclty the one of
         * A. Wald. Sequential tests of statistical hypotheses.
         * The Annals of Mathematical Statistics, 16(2):117–186, 06 1945.
         */

        const double alog = log((1-Value)/(1-Value2));
        const double blog = log(Value/Value2);

        double loglikelyhoodRatio = alog * (double)(batch.I - batch.Isucc) + blog * (double)batch.Isucc;


        //std::cerr << "[" << blog << "  --  " << loglikelyhoodRatio << "  --  " << alog << "]" << std::endl;
        double l = 1- 1/(1+ exp(fabs(loglikelyhoodRatio)));
        
        if(loglikelyhoodRatio <= 0){ //Accept H0
            return ConfInt((double)batch.Isucc/(double)batch.I, Value ,1,0.0,1.0,l);
        }else{
            return ConfInt((double)batch.Isucc/(double)batch.I, 0 ,Value2,0.0,1.0,l);
        }
        }
            
        case CONSTANT:
            return ConfInt(Value,Value2,Value-Value2/2.0,Value+Value2/2.0,1.0);
            
        case HASL_PLUS:
            return (left->eval(batch) += right->eval(batch));
            
        case HASL_MINUS:
            return (left->eval(batch) -= right->eval(batch));
            
        case HASL_TIME:
            return (left->eval(batch) *= right->eval(batch));
            
        case HASL_DIV:
            return (left->eval(batch) /= right->eval(batch));
            
        case EXISTS:
            if ( batch.Isucc > 0) return ConfInt(1.0,1.0,1.0,0.0,1.0,1.0);
            else return ConfInt(0.0,0.0,1.0,0.0,0.0,1.0);
        case NOTALLS:
            if ( batch.Isucc < batch.I) return ConfInt(1.0,1.0,1.0,0.0,1.0,1.0);
            else return ConfInt(0.0,0.0,1.0,0.0,0.0,1.0);
            
            
        default:
            std::cerr << "Fail to parse Hasl Formula"<< std::endl;
            exit(EXIT_FAILURE);
    }
}

double HaslFormulasTop::bound()const{
    switch (TypeOp) {
        case PROBABILITY:
        case PROBCOND:
            return 0.5;
            
        case EXPECTANCY:
        case CDF_PART:
        case PDF_PART:
        case RE_AVG:
        case RE_Continuous:
            return std::numeric_limits<double>::infinity();
            
        case CONSTANT:
            return fabs(Value);
            
        case HASL_PLUS:
        {
        double v1 = left->bound();
        double v2 = right->bound();
        return (fmax(v1,v2));
        }
            
        case HASL_TIME:
        {
        double v1 = left->bound();
        double v2 = right->bound();
        return (v1*v2);
        }
            
        case HASL_DIV:
        {
        return (std::numeric_limits<double>::infinity());
        }
            
        default:
            std::cerr << "Fail to parse Hasl Formula"<< std::endl;
            exit(EXIT_FAILURE);
    }
}
