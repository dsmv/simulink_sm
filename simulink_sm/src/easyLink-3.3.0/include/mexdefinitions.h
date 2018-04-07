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

//------------------------------------------------------------------------------
// Definition of the gateway function for MEX functions
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    try
    {
        Function::nrhs=nrhs;
        Function::prhs=&(prhs[0]);
        Function::nlhs=nlhs;
        Function::initializeInputPortSizes();
        Function::initializeOutputPortSizes();
        Function::outputs();
        for (int i=0;i<nlhs;i++) {
            plhs[i]=Function::plhs[i];
        }
    }
    catch (exception const& e)
    {
        strcpy(ERROR_MSG_BUFFER,e.what());
        mexErrMsgIdAndTxt("easylink:error",ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
