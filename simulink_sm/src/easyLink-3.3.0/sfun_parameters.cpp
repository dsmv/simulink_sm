//------------------------------------------------------------------------------
/* C++ S-function that shows all the different ways to use the parameters ports.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_parameters.cpp
 *
 * Then open the file "test_parameters.mdl" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfun_parameters

//------------------------------------------------------------------------------
// Specify the number, the type and the sizes of the parameters of the block
// A parameter is set to tunable if its values can change during the simulation
//
// The possible types of the paramaters are:
//     * 1 for real parameters (scalar or array, use -1 as size to specify
//          a dynamically dimensioned array)
//     * 2 for literal string parameters
//     * 3 for literal identifiers of MATLAB variables
//
//------------------------------------------------------------------------------
#define PARAMETER_NUMBER                     6
enum    parameterName                        { PAR1, PAR2, PAR3, PAR4, PAR5, PAR6 };
int     parameterType[PARAMETER_NUMBER] =    {    1,    1,    1,    1,    2,    3 };
int     parameterRows[PARAMETER_NUMBER] =    {    1,    2,    1,   -1,   -1,   -1 };
int     parameterCols[PARAMETER_NUMBER] =    {    1,    3,   -1,   -1,    1,    1 };
int     parameterTunable[PARAMETER_NUMBER] = {    1,    0,    1,    1,    1,    1 };

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
public:

    void start()
    {
        printf("Starting the s-function for parameters testing....\n");
    }

    void outputs()
    {
        double par1=getParameterDouble(PAR1);
        Array  par2=getParameterArray(PAR2);
        Array  par3=getParameterArray(PAR3);
        Array  par4=getParameterArray(PAR4);
        string par5=getParameterString(PAR5);
        string par6=getParameterString(PAR6);

        printf("---------- time = %f ----------\n",getCurrentTime());
        printf("parameter %i = %f\n",PAR1,par1);
        par2.print();
        par3.print();
        par4.print();
        printf("parameter %i = %s\n",PAR5,par5.c_str());
        printf("parameter %i = %s\n",PAR6,par6.c_str());

    }

    void terminate()
    {
        printf("Terminating the s-function for parameters testing.\n");
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
