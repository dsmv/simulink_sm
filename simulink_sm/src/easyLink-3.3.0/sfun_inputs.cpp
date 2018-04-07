//------------------------------------------------------------------------------
/* C++ S-function that shows all the different ways to use the input ports.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_inputs.cpp
 *
 * Then open the file "test_inputs.mdl" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfun_inputs

//------------------------------------------------------------------------------
// Specify the number and the sizes of the input ports of the block
//
// Use -1 to specify a dynamically dimensioned input signal
//------------------------------------------------------------------------------
#define INPUT_PORT_NUMBER                  5
enum    inputPortName      	               { IN1, IN2, IN3, IN4, IN5 };
int     inputPortRows[INPUT_PORT_NUMBER] = {   1,   3,   4,  -1,  -1 };
int     inputPortCols[INPUT_PORT_NUMBER] = {   1,   1,   2,   1,  -1 };

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
public:

    void start()
    {
        printf("Starting the s-function for inputs testing...\n");
    }

    void outputs()
    {
        double in1=getInputDouble(IN1);
        Array  in2=getInputArray(IN2);
        Array  in3=getInputArray(IN3);
        Array  in4=getInputArray(IN4);
        Array  in5=getInputArray(IN5);

        printf("---------- time = %f ----------\n",ssGetT(simStruct));
        printf("input port %i = %f\n",IN1,in1);
        in2.print();
        in3.print();
        in4.print();
        in5.print();
    }

    void terminate()
    {
        printf("Terminating the s-function for inputs testing.\n");
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
