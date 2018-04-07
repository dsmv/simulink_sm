//------------------------------------------------------------------------------
/* C++ S-function that shows all the different ways to use the matlab arrays.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_matlab_arrays.cpp
 *
 * Then open the file "test_matlab_arrays.mdl" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfun_matlab_arrays

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
private:

    Array myArray;

public:

    void start()
    {
        printf("Starting the s-function for matlab array testing...\n");
        Array test=newMatlabArray(2,2,"test");
        test(0,0)=2;
        test(1,1)=4;
        myArray=test;
    }

    void outputs()
    {
        printf("---------- time = %f ----------\n",ssGetT(simStruct));

        Array test=getMatlabArray("test");
        test+=1;
        test.print();

        myArray+=1;
        myArray.print();

        Array A(2,2,"A");
        A(0,0)=-3;
        A(1,1)=4;
        A.print();

        Array B;
        B=A.inverse();
        B.print();

        Array C=A*B;
        C.print();

        if (C==Array::eye(2,2)) printf("%s==eye(2) ;-)\n",C.getName().c_str());

    }

    void terminate()
    {
        printf("Terminating the s-function for matlab array testing.\n");
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
