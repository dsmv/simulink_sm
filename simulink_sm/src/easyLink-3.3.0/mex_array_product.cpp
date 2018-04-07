//------------------------------------------------------------------------------
/* This file illustrates how to construct a simple C++ MEX-File with
 * easyLink.
 *
 * Multiplies an input scalar times a 1xN matrix and outputs a 1xN matrix
 *
 * The calling syntax is:
 *
 *     outMatrix = mex_array_product(multiplier, inMatrix)
 *
 * To compile this C++ MEX-File, enter the following command in MATLAB:
 *
 *     >>make mex_array_product.cpp
 *
 */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Specify the number and the sizes of the input ports of the function
//
// The possible types of the inputs are:
//     * 1 for real parameters (scalar or array, use -1 as size to specify
//          a dynamically dimensioned array)
//     * 2 for literal string parameters
//     * 3 for literal identifiers of MATLAB variables
//------------------------------------------------------------------------------
#define INPUT_PORT_NUMBER                  2
enum    inputPortName      	               { MULTIPLIER, MATRIX };
int     inputPortType[INPUT_PORT_NUMBER] = {          1,      1 };
int     inputPortRows[INPUT_PORT_NUMBER] = {          1,      1 };
int     inputPortCols[INPUT_PORT_NUMBER] = {          1,     -1 };

//------------------------------------------------------------------------------
// Specify the number and the sizes of the output ports of the function
//
// Use -1 to specify a dynamically dimensioned output signal
//------------------------------------------------------------------------------
#define OUTPUT_PORT_NUMBER                   1
enum    outputPortName                       { RESULT };
int     outputPortRows[OUTPUT_PORT_NUMBER] = {      1 };
int     outputPortCols[OUTPUT_PORT_NUMBER] = {     -1 };

//------------------------------------------------------------------------------
#include "easylink.h"

class Function : public BaseFunction
{
private:

public:

    static void outputs()
    {
        setOutputPortDimensions(RESULT,1,getInputNCols(MATRIX));
        double multiplier=getInputDouble(MULTIPLIER);
        Array matrix=getInputArray(MATRIX);
        Array result=getOutputArray(RESULT);

        result=matrix*multiplier;
    }

};

//------------------------------------------------------------------------------
#include "mexdefinitions.h"

//------------------------------------------------------------------------------
