//------------------------------------------------------------------------------
/* C++ S-function for defining a continuous system.
 *
 *    x' = Ax + Bu
 *    y  = Cx + Du
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_state_space.cpp
 *
 * Then open the file "test_state_space.mdl" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfun_state_space

//------------------------------------------------------------------------------
// Specify the number and the sizes of the input ports of the block
//
// Use -1 to specify a dynamically dimensioned input signal
//------------------------------------------------------------------------------
#define INPUT_PORT_NUMBER                  1
enum    inputPortName      	               {  U };
int     inputPortRows[INPUT_PORT_NUMBER] = { -1 };
int     inputPortCols[INPUT_PORT_NUMBER] = {  1 };

//------------------------------------------------------------------------------
// Specify the number and the sizes of the output ports of the block
//
// Use -1 to specify a dynamically dimensioned output signal
//------------------------------------------------------------------------------
#define OUTPUT_PORT_NUMBER                   1
enum    outputPortName                       {  Y };
int     outputPortRows[OUTPUT_PORT_NUMBER] = { -1 };
int     outputPortCols[OUTPUT_PORT_NUMBER] = {  1 };

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
#define PARAMETER_NUMBER                     4
enum    parameterName                        {  A,  B,  C,  D };
int     parameterType[PARAMETER_NUMBER] =    {  1,  1,  1,  1 };
int     parameterRows[PARAMETER_NUMBER] =    { -1, -1, -1, -1 };
int     parameterCols[PARAMETER_NUMBER] =    { -1, -1, -1, -1 };
int     parameterTunable[PARAMETER_NUMBER] = {  0,  0,  0,  0 };

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
private:

    Array a,b,c,d,e;

public:

    static void initializeOutputPortSizes()
    {
        BaseBlock::initializeOutputPortSizes();
        setOutputPortDimensions(Y,getParameterNRows(C),1);
    }

    static void initializeStatePortSizes()
    {
        setContinuousStatesNumber(getParameterNRows(A));
    }

    static void initializeSampleTimes()
    {
        ssSetSampleTime(simStruct, 0, CONTINUOUS_SAMPLE_TIME);
        ssSetOffsetTime(simStruct, 0, 0.0);
    }

    void start()
    {
        a=getParameterArray(A);
        b=getParameterArray(B);
        c=getParameterArray(C);
        d=getParameterArray(D);
        e=Array(2,3);
    }

    void outputs()
    {
        Array u=getInputArray(U);
        Array y(getParameterNRows(C),1,"y");
        Array x=getContinuousStateArray();

        y=c*x+d*u;

        setOutputArray(Y,y);
    }

    void derivatives()
    {
        Array u=getInputArray(U);
        Array x=getContinuousStateArray();
        Array dx(x.getNRows(),1,"dx");
        
        dx=a*x+b*u;
        
        setDerivativeStateArray(dx);
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
