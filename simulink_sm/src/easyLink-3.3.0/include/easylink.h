//------------------------------------------------------------------------------
//
//  This file is part of easyLink Library.
//
//  Copyright (c) 2014 FEMTO-ST, ENSMM, UFC, CNRS.
//
//  License: GNU General Public License 3
//  Author: Guillaume J. Laurent
//
//------------------------------------------------------------------------------
/**

\defgroup initialization S-function initialization methods
\defgroup runtime S-function runtime methods
\defgroup inputPort Input ports managing methods
\defgroup outputPort Output ports managing methods
\defgroup parameterPort S-function parameters managing methods
\defgroup statePort S-function state ports managing methods
\defgroup matlabArray Matlab variables managing methods
\defgroup utils Miscellaneous functions on strings and numbers

\mainpage Overview

You need to write a MEX functions or S-functions?

You hate the MATLAB C API? You do not like the code produced by the S-function
generator either?

You would like to use C++ and to write classes? 

easyLink is for you!!!

### Description

easyLink is a C++ rapid development kit for creating new MEX-functions for
MATLAB and new blocks for SIMULINK (S-functions).

easyLink allows to develop C++ MEX-functions and S-functions with less code and
using friendly classes.

easyLink is simpler and as efficient than the MATLAB C API. It provides a
direct access to input, output and parameter ports as well as MATLAB
variables thanks to a C++ Array class.

easyLink is a multi-platform library that works under Linux, OSX and Windows.

### Documentation content

  - \ref pageQuickReference
  - \ref pageExamples
  - <a href="modules.html">User reference</a>

### Download

The latest release can be downloaded <a href="https://sourcesup.renater.fr/frs/?group_id=1500">
here</a>.

The project SVN repository can be checked out through anonymous access with
the following command:

\verbatim svn checkout https://subversion.renater.fr/easylink \endverbatim

Bugs reports, questions and suggestions can be posted on
<a href="https://sourcesup.renater.fr/projects/easylink/">sourcesup forge</a>.

### Installation

  -# Unpack the archive in a local directory
  -# Open Matlab
  -# Set the matlab current directory where easyLink is located
  -# Execute the startup file "install.m" and follow
     instructions to select an installed C++ compiler
\verbatim >>install \endverbatim
  -# Execute the following command to compile and link a demo file
\verbatim >>make sfun_offset.cpp \endverbatim
  -# Open Simulink
  -# Open the file "test_offset.mdl" and run it to check the installation

Use the file sfun_offset.cpp as a template to write a new S-function (SIMULINK).

Use the file mex_array_product.cpp as a template to write a new MEX-function (MATLAB).

### Licence

easyLink is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

easyLink is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with the sources files.  If not, see http://www.gnu.org/licenses/.

Copyright (c) 2014-2015 FEMTO-ST, ENSMM, UFC, CNRS.

### About

easyLink is developed and maintained within the FEMTO-ST Institute located in
Besançon, France and hosted by Renater.

easyLink official site is: http://sourcesup.renater.fr/easylink/

contact: Guillaume J. Laurent, http://www.femto-st.fr/~guillaume.laurent/


\page pageQuickReference Quick reference 

### Input port methods

\code{.cpp}
    double in1=getInputDouble(S,IN1);
    Array  in2=getInputArray(S,IN2);
    int    in3=getInputInt(S,IN3);
\endcode

### Output port methods

\code{.cpp}
    setOutputDouble(S,OUT1,out1);
    setOutputArray(S,OUT2,out2);
    setOutputImage(S,OUT3,Im2);
\endcode

### Parameter port methods

\code{.cpp}
    int par1 = getParameterInt(S,PAR1);
    double par2 = getParameterDouble(S,PAR2);
    string par3 = getParameterString(S,PAR3);
    Array par4 = getParameterArray(S,PAR4);
\endcode

### Arrays

\code{.cpp}
    if (existMatlabArray("test")) ...

    Array test = newMatlabArray(2,2,"test");
    test(0,0)=2;
    test(1,1)=4;

    Array test = getMatlabArray("test");
    test += 1;
    test.print();

    Array A(2,2,"A");
    A(0,0) = -3;
    A(1,1) = 4;
    A.print();
\endcode


\page pageExamples Examples

S-function basic example (use it as a template to write new S-functions):

  - sfun_offset.cpp shows how to write a S-function that adds a scalar value
    to a scalar input

S-function examples that demonstrate all the features:

  - sfun_inputs.cpp shows all the different ways to use the input ports.

  - sfun_outputs.cpp shows all the different ways to use the output ports.

  - sfun_matlab_arrays.cpp shows all the different ways to use the matlab arrays.

  - sfun_parameters.cpp shows all the different ways to use the parameters ports.

S-function advanced examples:

  - sfun_state_space.cpp shows how to write S-functions with continuous states.

  - sfun_size_change.cpp shows how to change the size of an output according
    to the value of a parameter.

  - sfun_times_two.cpp show how to change the size of an output according to
    the size of a dynamically-sized input array

S-function guru examples:

  - sfun_no_macro.cpp shows how to write a S-function without using global
    variables that define input, output and parameter port dimensions.

  - sfun_direct_output_access.cpp shows how to write directly in the output
    array without using setOutputArray.

MEX-function basic example (use it as a template to write new MEX-functions)::

  - mex_array_product.cpp shows how to write a MEX-function that multiplies
    a matrix and a scalar.


*/

//------------------------------------------------------------------------------
#ifndef EASYLINK_H
#define EASYLINK_H

//------------------------------------------------------------------------------
#ifndef MATLAB_MEX_FILE
#define MATLAB_MEX_FILE
#endif

//------------------------------------------------------------------------------
#ifdef S_FUNCTION_NAME
    #ifndef S_FUNCTION_LEVEL
        #define S_FUNCTION_LEVEL 2
    #endif
    #include "simstruc.h"
    #include "baseblock.h"
#else
    #include "mex.h"
    #include "basefunction.h"
#endif

#include "matlabarray.h"

//------------------------------------------------------------------------------
#endif
