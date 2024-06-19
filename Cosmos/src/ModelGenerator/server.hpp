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
 *******************************************************************************
 */

/*
 * This file implement low level interaction between the main program
 * and all its children. In particular it lauches the require copies of
 * simulator and wait for the result.
 */




#ifndef _SERVER_HPP
#define	_SERVER_HPP

#include "parameters.hpp"

/**
 * One of the main function of the tool.
 * lauchServer execute several instance of the simulator,
 * then wait for some simulators to retun result.
 * At this point the new result are aggreated to the previous one using
 * the class result.
 * The class result indicate if the simulation should continue, if not
 * this function kill all the instance of the simulator, return all
 * the results to files or to stdout and return.
 * @param is the parameters to use for the export.
 */
void launchServer(parameters& P);

/**
 * Signal handler.
 *
 * The signals this function is expecting are SIGCHLD which append if
 * child simulator terminate. In this case the return status of the child
 * must be retrive to know what make the simulator terminate.
 * If the termination is legit: The simulator terminate because it has finished
 * its computation which append in -v 4 mode and when exporting state space,
 * or the server just killed the simulator with signal 2.
 * Then do nothing. Otherwise report the error.
 *
 * If the signal SIGINT is caught then the variable continueSelect is set to
 * false, this is sufficient because the signal interrupt the select
 * system call.
 *
 * @param signum the number of a signal
 */
void signalHandler( int signum );
void signalHandlerIgn( int signum );

//istream streamOfFile(int fd);

/*
 * Open a child processes retring both PID and an a pipe
 * to the standart input of the child.
 * This function fork and execute the given binary
 * on the child.
 * @param bin the path to the binary to execute.
 * @param argv the list of argument.
 */
void popenClient(const char* bin, const char *argv[]);

/**
 * Launch the P.Njob copy of the simulator with the parameters define in P
 * @param P the structure of Parameters
 */
void launch_clients(parameters& P);

/**
 * Kill all the copy of the simulator at the end of the computation.
 */
void kill_client();

/**
 * Wait until all child terminate.
 * This allow to recover usage information of the simulators.
 */
void wait_client();

/**
 * Build a list of input files of all the simulators to collect results
 * This list is made to be used with the function <sys/select.h>/select
 */
void makeselectlist(void);

void freestr(const char *argv[],size_t t);

#endif	/* _SERVER_HPP */
