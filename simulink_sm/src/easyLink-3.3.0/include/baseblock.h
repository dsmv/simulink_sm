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
#ifndef EASYLINK_BASEBLOCK_H
#define EASYLINK_BASEBLOCK_H

//------------------------------------------------------------------------------
#include "array.h"

//------------------------------------------------------------------------------
#ifndef INPUT_PORT_NUMBER
#define INPUT_PORT_NUMBER  0
enum    inputPortName      { DUMMY_INPUT };
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
#ifndef PARAMETER_NUMBER
#define PARAMETER_NUMBER      0
enum    parameterName         { DUMMY_PARAMETER };
int     parameterType[1] =    {               1 };
int     parameterRows[1] =    {               1 };
int     parameterCols[1] =    {               1 };
int     parameterTunable[1] = {               1 };
#endif

//------------------------------------------------------------------------------
/** BaseBlock is the basis class for designing new S-functions.
 *
 * Use the file sfun_offset.cpp as a template to write a new S-function.
 */
class BaseBlock
{
protected:

    /** SimStruct data structure allocated for the S-function.
     * Can be used to call simulink macros. */
    static SimStruct *simStruct;

public:

    static inline void setSimStruct(SimStruct *S)
    {
        simStruct = S;
    }

    /** \ingroup initialization
     * This is the first static method that the Simulink engine calls within
     * mdlInitializeSizes.
     * This method specifies the number of parameters and wheter or not they
     * are tunable. A parameter is tunable if its value
     * can change during simulation.
     *
     * Use -1 to specify dynamically dimensioned parameters.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeParameterPortSizes()
    {
        ssSetNumSFcnParams(simStruct,PARAMETER_NUMBER);
        for (int i=0;i<PARAMETER_NUMBER;i++)
        {
            if (parameterTunable[i]==0)
                ssSetSFcnParamNotTunable(simStruct,i);
        }

        if (ssGetSFcnParamsCount(simStruct) != PARAMETER_NUMBER)
            throw runtime_error("easyLink error: " + toString(PARAMETER_NUMBER) + " parameters are expected.");
    }

    /** \ingroup initialization
     * This is the second static method that the Simulink engine calls within
     * mdlInitializeSizes.
     * This method specifies the number of input ports and their dimensions.
     * All ports are set to direct feedthrough by default.
     *
     * Use -1 to specify dynamically dimensioned input signals.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeInputPortSizes()
    {
        if (!ssSetNumInputPorts(simStruct,INPUT_PORT_NUMBER))
            throw runtime_error("easyLink error: Unable to set input port number to " + toString(INPUT_PORT_NUMBER) + ".");

        for (int i=0;i<INPUT_PORT_NUMBER;i++)
        {
            setInputPortDimensions(i,inputPortRows[i],inputPortCols[i]);
            ssSetInputPortDirectFeedThrough(simStruct,i,1);
            ssSetInputPortRequiredContiguous(simStruct,i,1);
        }
    }

    /** \ingroup initialization
     * This is the third static method that the Simulink engine calls within
     * mdlInitializeSizes.
     * This method specifies the number of output ports and their dimensions.
     *
     * Use -1 to specify dynamically dimensioned output signals.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeOutputPortSizes()
    {
        if (!ssSetNumOutputPorts(simStruct,OUTPUT_PORT_NUMBER))
            throw runtime_error("easyLink error: Unable to set output port number to " + toString(OUTPUT_PORT_NUMBER) + ".");

        for (int i=0;i<OUTPUT_PORT_NUMBER;i++)
            setOutputPortDimensions(i,outputPortRows[i],outputPortCols[i]);
    }

    /** \ingroup initialization
     * This is the fourth static method that the Simulink engine calls within
     * mdlInitializeSizes.
     * This method specifies the number of continuous and discrete states.
     *
     * The default values are zero continuous states and zero discrete states.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeStatePortSizes()
    {
        ssSetNumContStates(simStruct,0);
        ssSetNumDiscStates(simStruct,0);
    }

    /** \ingroup initialization
     * This is the fifth static method that the Simulink engine calls within
     * mdlInitializeSizes.
     * This method specifies the number of sample time (i.e., sample rates) at
     * which the block operates.
     *
     * The default value is 1
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeNumberSampleTimes()
    {
        ssSetNumSampleTimes(simStruct, 1);

		//ssSetSampleTime( simStruct, 0, "[INHERITED_SAMPLE_TIME, 0.0]" );
	}

    /** \ingroup initialization
     * This is the sixth static method that the Simulink engine calls within
     * mdlInitializeSizes.
     * This method specifies the simulation options that this block implements,
     * using ssSetOptions.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeOptions()
    {
    }

    /** \ingroup initialization
     * This static method is called with candidate dimensions for input port.
     * If the proposed dimensions are unacceptable for the port, the method must
     * throw an exception.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlsetinputportdimensioninfo.html */
    static void setInputPortDimensionInfo(int port,const DimsInfo_T *dimsInfo)
    {
        if(!ssSetInputPortDimensionInfo(simStruct, port, dimsInfo)) return;
    }

    /** \ingroup initialization
     * This static method is called with candidate dimensions for output port.
     * If the proposed dimensions are unacceptable for the port, the method must
     * throw an exception.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlsetoutputportdimensioninfo.html */
    static void setOutputPortDimensionInfo(int port,const DimsInfo_T *dimsInfo)
    {
        if(!ssSetOutputPortDimensionInfo(simStruct, port, dimsInfo)) return;
    }


    /** \ingroup initialization
     * This static method is called with candidate parameter values.
     * If the proposed dimensions or types are unacceptable, the method must
     * throw an exception.
     *
     * The possible types of the paramaters are:
     *  - 1 for real parameters (scalar or array, use -1 as size to specify
     *          a dynamically dimensioned array)
     *  - 2 for literal string parameters
     *  - 3 for literal identifiers of MATLAB variables
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlcheckparameters.html */
    static void checkParameters()
    {
        for (int i=0;i<PARAMETER_NUMBER;i++)
        {
            if ((parameterType[i]==2)||(parameterType[i]==3))
            {
                // check if it is really a string
                if (!mxIsChar(ssGetSFcnParam(simStruct,i)))
                    throw runtime_error("easyLink error: Parameter port " + toString(i) + " must be a string.");

                if (parameterType[i]==3)
                {
                    // check if it is really a identifier
                    string str=getParameterString(i);
                    if (!isIdentifier(str))
                        throw runtime_error("easyLink error: Parameter port " + toString(i) + " must be a valid identifier.");
                }
            }
            else
            {
                // check if it is a scalar or a vector
                if (mxIsSparse(ssGetSFcnParam(simStruct,i))||!mxIsDouble(ssGetSFcnParam(simStruct,i)))
                    throw runtime_error("easyLink error: Parameter port " + toString(i) + " must be a scalar or an array of real values.");

                // check the number of rows
                if ( (parameterRows[i]>0) && (mxGetM(ssGetSFcnParam(simStruct,i))!=parameterRows[i]) )
                    throw runtime_error("easyLink error: Parameter port " + toString(i) + " must have " + toString(parameterRows[i]) + " rows.");

                // check the number of cols
                if ( (parameterCols[i]>0) && (mxGetN(ssGetSFcnParam(simStruct,i))!=parameterCols[i]) )
                    throw runtime_error("easyLink error: Parameter port " + toString(i) + " must have " + toString(parameterCols[i]) + " cols.");
            }
        }
    }

    /** \ingroup initialization
     * This static method should specify the sample time and offset time for
     * each sample rate at which this S-function operates.
     *
     * The default values are INHERITED_SAMPLE_TIME and FIXED_IN_MINOR_STEP_OFFSET
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesampletimes.html */
    static void initializeSampleTimes()
    {
        ssSetSampleTime(simStruct, 0, INHERITED_SAMPLE_TIME);
        ssSetOffsetTime(simStruct, 0, 0.0);
        ssSetModelReferenceSampleTimeDefaultInheritance(simStruct);
    }

    /** \ingroup runtime
     * The Simulink engine invokes this method at the beginning of
     * a simulation right after contructing the class.
     * The method should perform initialization activities that this S-function
     * requires only once, such as allocating memory, setting up user data, or
     * initializing states.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlstart.html */
    void start()
    {
    }

    /** \ingroup runtime
     * The Simulink engine invokes this method at each simulation time step.
     * The method should compute the S-function's outputs at the current time
     * step and store the results using outputArray or using writeOutput.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdloutputs.html */
    void outputs()
    {
    }

    /** \ingroup runtime
     * The Simulink engine invokes this optional method at each time step to
     * compute the derivatives of the S-function's continuous states.
     * This method should store the derivatives in the S-function's state
     * derivatives array using derivativesArray.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlderivatives.html */
    void derivatives()
    {
    }

    /** \ingroup runtime
     * An S-function needs to provide this optional method only if it does
     * zero-crossing detection. Implementing zero-crossing detection typically
     * requires using the zero-crossing and mode work vectors to determine when
     * a zero crossing occurs and how the S-function's outputs should respond
     * to this event. The zeroCrossings method should update the S-function's
     * zero-crossing vector, using nonSampledZCsArray.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlzerocrossings.html */
    void zeroCrossings()
    {
    }

    /** \ingroup runtime
     * The Simulink engine invokes this optional method at each major
     * simulation time step. The method should compute the S-function's states
     * at the current time step and store the states in the S-function's state
     * vector. The method can also perform any other tasks that the S-function
     * needs to perform at each major time step.
     *
     * Use this code if your S-function has one or more discrete states or does
     * not have direct feedthrough.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlupdate.html */
    void update()
    {
    }

    /** \ingroup runtime
     * The Simulink engine invokes this method when the simulation is terminated
     * right before deleting the class.
     * This method should perform any terminal actions, such as freeing of
     * memory.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlterminate.html */
    void terminate()
    {
    }

    //--------------------------------------------------------------------------
    /** \ingroup inputPort
     * Returns the scalar value of an input port.
     */
    static inline double getInputDouble(int port)
    {
        return *ssGetInputPortRealSignal(simStruct,port);
    }

    /** \ingroup inputPort
     * Returns the integer value of an input port
     */
    static inline double getInputInt(int port)
    {
        //return round(*(ssGetInputPortRealSignal(simStruct,port)));
		return *(ssGetInputPortRealSignal(simStruct,port));  // FIXME:
    }

    /** \ingroup inputPort
     * Returns the input port Array.
     */
    static inline Array getInputArray(int port)
    {
        return Array(ssGetInputPortRealSignal(simStruct,port),ssGetInputPortDimensionSize(simStruct,port,0),ssGetInputPortDimensionSize(simStruct,port,1),"input port "+toString(port),true);
    }

    /** \ingroup inputPort
     * Returns the input port number of elements.
     * If the input port is a 1-D array with w elements, this function returns w.
     * If the input port is an M-by-N array, this function returns m*n.
     */
    static inline int getInputWidth(int port)
    {
        return ssGetInputPortWidth(simStruct,port);
    }

    /** \ingroup inputPort
     * Returns the input port number of rows.
     */
    static inline int getInputNRows(int port)
    {
        return ssGetInputPortDimensionSize(simStruct,port,0);
    }

    /** \ingroup inputPort
     * Returns the input port number of cols.
     */
    static inline int getInputNCols(int port)
    {
        return ssGetInputPortDimensionSize(simStruct,port,1);
    }

    /** \ingroup inputPort
     * Sets the input port dimensions.
     * Must be used only in initializeInputPortSizes
     */
    static inline void setInputPortDimensions(int port,int nrows,int ncols)
    {
        if (ncols==1)
            ssSetInputPortWidth(simStruct,port,nrows);
        else
            ssSetInputPortMatrixDimensions(simStruct,port,nrows,ncols);
    }

    //--------------------------------------------------------------------------
    /** \ingroup outputPort
     * Writes a double or int value to an output port.
     */
    static inline void setOutputDouble(int port,double value)
    {
        double *x=ssGetOutputPortRealSignal(simStruct,port);
        x[0]=value;
    }

    /** \ingroup outputPort
     * Writes an Array to an output port. Dimensions must agree.
     */
    static inline void setOutputArray(int port,Array & array)
    {
        if (ssGetOutputPortDimensionSize(simStruct,port,0)!=array.getNRows() || ssGetOutputPortDimensionSize(simStruct,port,1)!=array.getNCols())
            throw runtime_error("easyLink error: Unable to write "+array.getName()+" to output port "+toString(port)+". Array dimensions must agree.");

        memcpy((void*)ssGetOutputPortRealSignal(simStruct,port),(void*)array.getData(), array.getWidth()*sizeof(double) );
#ifdef __TEST__
                printf("easyLink test message: hard copy of array in setOutputArray(\"%s\").\n",array.getName().c_str());
#endif
    }

    /** \ingroup outputPort
     * Returns the output port Array.
     */
    static inline Array getOutputArray(int port)
    {
        return Array(ssGetOutputPortRealSignal(simStruct,port),ssGetOutputPortDimensionSize(simStruct,port,0),ssGetOutputPortDimensionSize(simStruct,port,1),"output port "+toString(port),true);
    }

    /** \ingroup outputPort
     * Returns the output port number of elements.
     * If the output port is a 1-D array with w elements, this function returns w.
     * If the output port is an M-by-N array, this function returns m*n.
     */
    static inline int getOutputWidth(int port)
    {
        return ssGetOutputPortWidth(simStruct,port);
    }

    /** \ingroup outputPort
     * Returns the output port number of rows.
     */
    static inline int getOutputNRows(int port)
    {
        return ssGetOutputPortDimensionSize(simStruct,port,0);
    }

    /** \ingroup outputPort
     * Returns the output port number of cols.
     */
    static inline int getOutputNCols(int port)
    {
        return ssGetOutputPortDimensionSize(simStruct,port,1);
    }

    /** \ingroup outputPort
     * Sets the output port dimensions.
     * Must be used only in initializeOutputPortSizes
     */
    static inline void setOutputPortDimensions(int port,int nrows,int ncols)
    {
        if (ncols==1)
            ssSetOutputPortWidth(simStruct,port,nrows);
        else
            ssSetOutputPortMatrixDimensions(simStruct,port,nrows,ncols);
    }

    //--------------------------------------------------------------------------
    /** \ingroup parameterPort
     * Returns the double value of a parameter port.
     */
    static inline double getParameterDouble(int port)
    {
        return mxGetPr(ssGetSFcnParam(simStruct,port))[0];
    }

    /** \ingroup parameterPort
     * Returns the int value of a parameter port.
     */
    static inline int getParameterInt(int port)
    {
        return _round(mxGetPr(ssGetSFcnParam(simStruct,port))[0]);
    }

    /** \ingroup parameterPort
     * Returns the string value of a parameter port.
     */
    static inline string getParameterString(int port)
    {
        char buffer[256];
        mxGetString(ssGetSFcnParam(simStruct,port), buffer, 256);
        return string(buffer);
    }

    /** \ingroup parameterPort
     * Returns an Array connected to a parameter port.
     */
    static inline Array getParameterArray(int port)
    {
        return Array(ssGetSFcnParam(simStruct,port),"parameter "+toString(port),true);
    }

    /** \ingroup parameterPort
     * Returns the parameter port number of elements.
     *
     * If the parameter port is a 1-D array with w elements, this function returns
     * w. If the parameter port is an M-by-N array, this function returns m*n.
     */
    static inline int getParameterWidth(int port)
    {
        return (int)mxGetM(ssGetSFcnParam(simStruct,port))*(int)mxGetN(ssGetSFcnParam(simStruct,port));
    }

    /** \ingroup parameterPort
     * Returns the parameter port number of rows.
     */
    static inline int getParameterNRows(int port)
    {
        return (int)mxGetM(ssGetSFcnParam(simStruct,port));
    }

    /** \ingroup parameterPort
     * Returns the parameter port number of cols.
     */
    static inline int getParameterNCols(int port)
    {
        return (int)mxGetN(ssGetSFcnParam(simStruct,port));
    }

    //--------------------------------------------------------------------------
    /** \ingroup statePort
     * Returns the continuous state Array.
     */
    static inline Array getContinuousStateArray()
    {
        return Array(ssGetContStates(simStruct),ssGetNumContStates(simStruct),1,"continuous state",true);
    }

    /** \ingroup statePort
     * Returns the continuous state number of elements (1-D array).
     */
    static inline int getContinuousStateWidth(int port)
    {
        return ssGetNumContStates(simStruct);
    }

    /** \ingroup statePort
     * Writes the derivative state Array.
     */
    static inline void setDerivativeStateArray(Array & array)
    {
        if (ssGetNumContStates(simStruct)!=array.getNRows() || array.getNCols()!=1 )
            throw runtime_error("easyLink error: Unable to write "+array.getName()+" to derivative of state port. Array dimensions must agree.");

        memcpy((void*)ssGetdX(simStruct),(void*)array.getData(), array.getWidth()*sizeof(double) );
#ifdef __TEST__
                printf("easyLink test message: hard copy of array in setDerivativeStateArray(\"%s\").\n",array.getName().c_str());
#endif
    }

    /** \ingroup statePort
     * Returns the discrete state Array.
     */
    static inline Array getDiscreteStateArray()
    {
        return Array(ssGetDiscStates(simStruct),ssGetNumDiscStates(simStruct),1,"discrete state",true);
    }

    /** \ingroup statePort
     * Returns the discrete state number of elements (1-D array).
     */
    static inline int getDiscreteStateWidth(int port)
    {
        return ssGetNumDiscStates(simStruct);
    }

    /** \ingroup statePort
     * Writes the discrete state Array.
     */
    static inline void setDiscreteStateArray(Array & array)
    {
        if (ssGetNumDiscStates(simStruct)!=array.getNRows() || array.getNCols()!=1 )
            throw runtime_error("easyLink error: Unable to write "+array.getName()+" to discrete state port. Array dimensions must agree.");

        memcpy((void*)ssGetDiscStates(simStruct),(void*)array.getData(), array.getWidth()*sizeof(double) );
#ifdef __TEST__
                printf("easyLink test message: hard copy of array in setDiscreteStateArray(\"%s\").\n",array.getName().c_str());
#endif
    }

    /** \ingroup statePort
     * Sets the number of continuous states. Must be used in lastMinuteSizeChanges.
     */
    static inline void setContinuousStatesNumber(int num)
    {
        ssSetNumContStates(simStruct,num);
    }

    /** \ingroup statePort
     * Sets the number of discrete states. Must be used in lastMinuteSizeChanges.
     */
    static inline void setDiscreteStatesNumber(int num)
    {
        ssSetNumDiscStates(simStruct,num);
    }

    /** Get the current simulation time */
    static inline time_T getCurrentTime()
    {
        return ssGetT(simStruct);
    }

};

SimStruct *BaseBlock::simStruct=NULL;

//------------------------------------------------------------------------------
#endif
