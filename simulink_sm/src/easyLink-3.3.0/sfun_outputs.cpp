//------------------------------------------------------------------------------
/* C++ S-function that shows all the different ways to use the output ports.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_outputs.cpp
 *
 * Then open the file "test_outputs.mdl" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfun_outputs

//------------------------------------------------------------------------------
// Specify the number and the sizes of the output ports of the block
//
// Use -1 to specify a dynamically dimensioned output signal
//------------------------------------------------------------------------------
#define OUTPUT_PORT_NUMBER                   3
enum    outputPortName                       { OUT1, OUT2, OUT3 };
int     outputPortRows[OUTPUT_PORT_NUMBER] = {   1,     3,    2 };
int     outputPortCols[OUTPUT_PORT_NUMBER] = {   1,     1,    3 };

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
public:

    void start()
    {
        printf("Starting the s-function for outputs testing...\n");
    }

    void outputs()
    {
        Array  out2(3,1);
        Array  out3(2,3);

        printf("---------- time = %f ----------\n",ssGetT(simStruct));
        printf("output port %i = 7\n",OUT1);
        setOutputDouble(OUT1,7);

        out2.init(1);
        out2[1]=2;
        out2.print();
        setOutputArray(OUT2,out2);

        out3.init(3);
        out3(0,2)=4;
        out3.print();
        setOutputArray(OUT3,out3);

    }

    void terminate()
    {
        printf("Terminating the s-function for outputs testing.\n");
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
