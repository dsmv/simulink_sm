//------------------------------------------------------------------------------
/* C++ S-function for multiplying an input by 2
 *
 *   y = 2*u
 *
 * Input and output ports are dynamically sized.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_times_two.cpp
 *
 * Then open the file "test_times_two.mdl" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfun_times_two

//------------------------------------------------------------------------------
// Specify the number and the sizes of the input ports of the block
//
// Use -1 to specify a dynamically dimensioned input signal
//------------------------------------------------------------------------------
#define INPUT_PORT_NUMBER                  1
enum    inputPortName      	               { IN };
int     inputPortRows[INPUT_PORT_NUMBER] = { -1 };
int     inputPortCols[INPUT_PORT_NUMBER] = { 1 };

//------------------------------------------------------------------------------
// Specify the number and the sizes of the output ports of the block
//
// Use -1 to specify a dynamically dimensioned output signal
//------------------------------------------------------------------------------
#define OUTPUT_PORT_NUMBER                   1
enum    outputPortName                       { OUT };
int     outputPortRows[OUTPUT_PORT_NUMBER] = {  -1 };
int     outputPortCols[OUTPUT_PORT_NUMBER] = {  1 };

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
public:

    static void setInputPortDimensionInfo(int port,const DimsInfo_T *dimsInfo)
    {
        BaseBlock::setInputPortDimensionInfo(port,dimsInfo);
        if (port==IN)
            setOutputPortDimensionInfo(OUT,dimsInfo);
    }

    void outputs()
    {
        Array in=getInputArray(IN);
        
        in*=2.0;

        setOutputArray(OUT,in);
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
