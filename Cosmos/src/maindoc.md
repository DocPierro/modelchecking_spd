/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2018 LSV & LACL                                          *
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
 * file maindoc.md created by Benoit Barbot on 16/03/2013.                     *
 *******************************************************************************
 */

\mainpage Cosmos
\author Benoît Barbot

# (C)oncept et (O)utils (S)tatistique pour les (Mo)deles (S)tochastiques


## Overview
This graph represent an overview of the structure of Cosmos.
Cosmos is divided in two main parts.
- The code generator,
	In blue on the graph do several things:
	+ Parse the command line and the input files
	+ Generate C++ code from the input files.
	+ Run one or several Simulators as independent processes and aggregate their
		result.
	+ Make some statistical analysis of the results and choose when to stop the
		simulators.
	The main function is in the file Cosmos.cpp .
- The Simulators, in green and red on the graph, simulate the trajectory
	of the system as fast as possible. results are send back to the Model
	Generator. Part of files of the simulators are generated, in red on the 
	graph. the main function is in the file clientsim.cpp

![Overview of the structure](../overview.png)


## Code generation and compilation
As part of the simulators is generated at runtime, those part must be compile
also at runtime. When the tool is compile the files in the directory Cosmos are
compiled and put in a library called libClientSim.a in the lib directory.

During the execution the model generator produce the missing part of the
simulator in the temporary directory.
Then it compiled them and link them to the library to build an executable.

## Changes

- 1.6 Add support for hybrid system simulation: multimodel and simulink model


##Speed history

commit       | date        | machine  |thread | trajectories per second
-------------|-------------|----------|-------|-------------------------
a8832fb      |  6/03/2014  | plouf    | 1     | 530
a8832fb      |  6/03/2014  | plouf    | 2     | 970
a8832fb      |  6/03/2014  | sume     | 1     | 510
a8832fb      |  6/03/2014  | sume     | 6     | 3000
8333be3c658e | 23/05/2014  | plouf    | 1     | 533
8333be3c658e | 23/05/2014  | plouf    | 2     | 981
8333be3c658e | 23/05/2014  | sume     | 1     | 538
8333be3c658e | 23/05/2014  | sume     | 6     | 2853
9be6a4cf964a | 13/10/2016  | sume     | 1     | 470
9be6a4cf964a | 13/10/2016  | sume     | 6     | 2500
76c261       | 18/01/2017  | tap      | 1     | 810
039410       | 18/01/2017  | sume     | 1     | 610
