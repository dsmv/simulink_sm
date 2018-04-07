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
#ifndef EASYLINK_BASEFUNCTION_H
#define EASYLINK_BASEFUNCTION_H

//------------------------------------------------------------------------------
#include "array.h"

//------------------------------------------------------------------------------
#ifndef INPUT_PORT_NUMBER
#define INPUT_PORT_NUMBER  0
enum    inputPortName      { DUMMY_INPUT };
int     inputPortType[1] = {           1 };
int     inputPortRows[1] = {           1 };
int     inputPortCols[1] = {           1 };
#endif

//------------------------------------------------------------------------------
#ifndef OUTPUT_PORT_NUMBER
#define OUTPUT_PORT_NUMBER  0
enum    outputPortName      { DUMMY_OUTPUT };
int     outputPortRows[1] = {            1 };
int     outputPortCols[1] = {            1 };
#endif

//------------------------------------------------------------------------------
/** BaseFunction is the basis class for designing new C++ MEX functions.
 *
 *  Use the file mex_array_product.cpp as a template to write a new MEX function.
 */
class BaseFunction
{
protected:

public:

    static int nrhs;
    const static mxArray **prhs;
    static int nlhs;
    static vector<mxArray *> plhs;

    /**
    * This is the first static method that is called within the MEX-Function.
    *
    * This method checks the number of input ports, their types dimensions.
    *
    * The possible types of the inputs are:
    *   - 1 for real inputs (scalar or array, use -1 as size to specify
    *       a dynamically dimensioned array)
    *   - 2 for literal string inputs
    *   - 3 for literal identifiers of MATLAB variables
    */
    static void initializeInputPortSizes()
    {
        /* check for proper number of inputs */
        if (nrhs!=INPUT_PORT_NUMBER)
            throw runtime_error(toString(INPUT_PORT_NUMBER) + " inputs required.");

        /* check for proper type and size of inputs */
        for (int i=0;i<INPUT_PORT_NUMBER;i++)
        {
            if ((inputPortType[i]==2)||(inputPortType[i]==3))
            {
                // check if it is really a string
                if (!mxIsChar(prhs[i]))
                    throw runtime_error("Argument " + toString(i+1) + " must be a string.");

                if (inputPortType[i]==3)
                {
                    // check if it is really a identifier
                    string str=getInputString(i);
                    if (!isIdentifier(str))
                        throw runtime_error("Argument " + toString(i+1) + " must be a valid identifier.");
                }
            }
            else
            {
                // check if it is a scalar or a vector
                if (mxIsSparse(prhs[i])||!mxIsDouble(prhs[i]))
                    throw runtime_error("Argument " + toString(i+1) + " must be a scalar or an array of real values.");

                // check the number of rows
                if ( (inputPortRows[i]>0) && (mxGetM(prhs[i])!=inputPortRows[i]) )
                    throw runtime_error("Argument " + toString(i+1) + " must have " + toString(inputPortRows[i]) + " rows.");

                // check the number of cols
                if ( (inputPortCols[i]>0) && (mxGetN(prhs[i])!=inputPortCols[i]) )
                    throw runtime_error("Argument " + toString(i+1) + " must have " + toString(inputPortCols[i]) + " cols.");
            }
        }

    }

    /**
     * This is the second static method that is called within the MEX-Function.
     *
     * This method specifies the number of output ports and their dimensions.
     *
     * Use -1 to specify dynamically dimensioned output signals.
     */
    static void initializeOutputPortSizes()
    {
        /* check for proper number of outputs */
        if (nlhs!=OUTPUT_PORT_NUMBER)
            throw runtime_error(toString(OUTPUT_PORT_NUMBER) + " outputs required.");

        plhs.resize(nlhs);
        /* set the proper size of outputs */
        for (int i=0;i<nlhs;i++) {
            setOutputPortDimensions(i,outputPortRows[i],outputPortCols[i]);
        }
    }

    /**
     * This is the third static method that is called within the MEX-Function.
     *
     * The method have to compute the function's outputs for given inputs
     * and to store the results using outputArray or using writeOutput.
     */
    static void outputs()
    {
    }

    //--------------------------------------------------------------------------
    /**
     * Returns the scalar value of an input port.
     */
    static inline double getInputDouble(int port)
    {
        return mxGetScalar(prhs[port]);
    }

    /**
     * Returns the integer value of an input port
     */
    static inline double getInputInt(int port)
    {
        return round(getInputDouble(port));
    }

    /**
     * Returns the input port Array.
     */
    static inline Array getInputArray(int port)
    {
        return Array(prhs[port],"input port "+toString(port),true);
    }

    /**
     * Returns the string value of an input port.
     */
    static inline string getInputString(int port)
    {
        char buffer[256];
        mxGetString(prhs[port], buffer, 256);
        return string(buffer);
    }

    /**
     * Returns the input port number of elements.
     * If the input port is a 1-D array with w elements, this function returns w.
     * If the input port is an M-by-N array, this function returns m*n.
     */
    static inline int getInputWidth(int port)
    {
        return getInputNRows(port)*getInputNCols(port);
    }

    /**
     * Returns the input port number of rows.
     */
    static inline int getInputNRows(int port)
    {
        return mxGetM(prhs[port]);
    }

    /**
     * Returns the input port number of cols.
     */
    static inline int getInputNCols(int port)
    {
        return mxGetN(prhs[port]);
    }

    //--------------------------------------------------------------------------
    /**
     * Writes a double or int value to an output port.
     */
    static inline void setOutputDouble(int port,double value)
    {
        double *x=mxGetPr(plhs[port]);
        x[0]=value;
    }

    /**
     * Writes an Array to an output port. Dimensions must agree.
     */
    static inline void setOutputArray(int port,Array & array)
    {
        if (getOutputNRows(port)!=array.getNRows() || getOutputNCols(port)!=array.getNCols())
            throw runtime_error("easyLink error: Unable to write "+array.getName()+" to output port "+toString(port)+". Array dimensions must agree.");

        memcpy((void*)mxGetPr(plhs[port]),(void*)array.getData(), array.getWidth()*sizeof(double) );
#ifdef __TEST__
                printf("easyLink test message: hard copy of array in setOutputArray(\"%s\").\n",array.getName().c_str());
#endif
    }

    /**
     * Returns the output port Array.
     */
    static inline Array getOutputArray(int port)
    {
        return Array(plhs[port],"output port "+toString(port),true);
    }

    /**
     * Returns the output port number of elements.
     * If the output port is a 1-D array with w elements, this function returns w.
     * If the output port is an M-by-N array, this function returns m*n.
     */
    static inline int getOutputWidth(int port)
    {
        return getOutputNRows(port)*getOutputNCols(port);
    }

    /**
     * Returns the output port number of rows.
     */
    static inline int getOutputNRows(int port)
    {
        return mxGetM(plhs[port]);
    }

    /**
     * Returns the output port number of cols.
     */
    static inline int getOutputNCols(int port)
    {
        return mxGetN(plhs[port]);
    }

    /**
     * Sets the output port dimensions.
     * Must be used only in initializeOutputPortSizes
     */
    static inline void setOutputPortDimensions(int port,int nrows,int ncols)
    {
        if (ncols>0 && nrows>0) {
            plhs[port]=mxCreateDoubleMatrix(nrows,ncols, mxREAL);
        }
        else {
            plhs[port]=NULL;
        }
    }

};

int BaseFunction::nrhs=0;
const mxArray** BaseFunction::prhs=NULL;
int BaseFunction::nlhs=0;
vector<mxArray *> BaseFunction::plhs;

//------------------------------------------------------------------------------
#endif
