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
 * file result.hpp created by Benoit Barbot on 22/05/12.                       *
 *******************************************************************************
 */

#include <sys/time.h>
#include <sys/resource.h>

#include <istream>
#include <fstream>
#include <ostream>

#include "parameters.hpp"
#include "../Simulator/BatchR.hpp"

#ifndef Cosmos_result_h
#define Cosmos_result_h


/**
 * \brief A class holdings result of the simulation.
 * This class handle the result of the simulation, it aggregate new result
 * toghether and format all the output to the user.
 * This class also decide if the simulation should continue
 */
class result {
public:
	result();
	
	//! Add a new batch of result to the result.
	void addBatch(BatchR&);
	
	//! return true if the simulation should continue.
	bool continueSim();


    //! Print the result in a compact way on stdout.
    void printCompactResult();

	//! Print the progress of the computation on stdout.
	void printProgress();
	
	//! stop the computation clock.
	void stopclock();
	
	//! print the result of the simulation to a stream.
    void print(std::ostream &s);
	
	//! print the result of the simulation to a file.
	void printResultFile(std::string);
	
	//! print the result of the simulation to stdout in format well suited for alligator.
	void printAlligator();
	
	//! Make gnuplot draw the result.
	void printGnuplot();
	
	//! Output the data of CDF or PDF to a file in the gnuplot file format
	void outputCDFPDF(std::string);
	
	//! Output data of the simulation in the gnuplot file format.
	void outputData();
	
	//! Close the interactive gnuplot session.
	void close_gnuplot();

    //! The raw result of the computation.
    BatchR MeanM2;
    
private:
	
	//! A copy of the parameters.
	//parameters P;
	
	//! timestamps for the strart and end of the simulation.
	std::chrono::system_clock::time_point start, end;
	
	//! timestanmps fot the last time the commandline where updated.
	std::chrono::system_clock::time_point lastprint;
	
	//! timestanmps fot the last time the gnuplot screen where updated.
	std::chrono::system_clock::time_point lastdraw;
	
	//! Total cpu time.
	double cpu_time_used;
	
	//! Stream where to print data.
	std::fstream outdatastream;
	
	//! Flush the gnuplot stream and check for error.
	void flushgnuplot();
	
	//! Stream to the interactive gnuplot session.
	FILE* gnuplotstream;
        std::string gnuplotextenstion;
	
	//! Number of lines written by the function printProgress.
	int endline;
	
	//! The maximal relative error of the result.
	double Progress;
	
	//! The relative error for each formula.
	std::vector<double> ProgressArray;
	
	//! The result of the estimator of the confidence interval for each formula.
	std::vector<ConfInt> HaslResult;
	
	//! Print a progress bar.
	void printPercent(double i, double j);

    int nbColumnGraph;
    std::tuple<std::string,double> split_name(std::string);

    size_t maxformulaname;

};


#endif
