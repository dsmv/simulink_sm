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
#ifndef EASYLINK_MATLABARRAY_H
#define EASYLINK_MATLABARRAY_H

//------------------------------------------------------------------------------
#include "array.h"

//------------------------------------------------------------------------------
/** \ingroup matlabArray
 * Returns an Array connected to a MATLAB variable in a given workspace.
 * Works only with double array.
 * Values for workspace are: "base", "caller" or "global".
 */
inline Array getMatlabArray(string name,string workspace="base")
{
    const mxArray *mxarray;

    mxarray=mexGetVariablePtr(workspace.c_str(),name.c_str());
    if (!mxarray)
        throw runtime_error("easyLink error: Unable to find a double array named "+name+".");

    return Array(mxarray,name,true);
}

//------------------------------------------------------------------------------
/** \ingroup matlabArray
 * Returns true if a MATLAB variable exists with the given name in a given workspace.
 * Works only with double array.
 * Values for workspace are: "base", "caller" or "global".
 */
inline bool existMatlabArray(string name,string workspace="base")
{
    const mxArray *mxarray;

    mxarray=mexGetVariablePtr(workspace.c_str(),name.c_str());

    return (mxarray!=NULL);
}

//------------------------------------------------------------------------------
/** \ingroup matlabArray
 * Create a new variable in a given MATALB workspace ("base" is the
 * default workspace) and returns an Array connected this variable.
 * Values for workspace are: "base", "caller" or "global".
 */
inline Array newMatlabArray(int nrows,int ncols,string name,string workspace="base")
{
    mxArray *mxarray;

    mxarray=mxCreateDoubleMatrix(nrows,ncols, mxREAL);
    if (!mxarray)
        throw runtime_error("easyLink error: Unable to create a double array named "+name+".");

    if (mexPutVariable(workspace.c_str(),name.c_str(),mxarray))
        throw runtime_error("easyLink error: Unable to put variable "+name+" in workspace "+workspace+".");

    mxDestroyArray((mxArray*)mxarray);

    return getMatlabArray(name,workspace);
}

//------------------------------------------------------------------------------
#endif
