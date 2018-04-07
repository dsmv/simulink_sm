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
#ifndef EASYLINK_ARRAY_H
#define EASYLINK_ARRAY_H

//------------------------------------------------------------------------------
#include "utils.h"
#include <string>
#include <stdexcept>
using namespace std;

//------------------------------------------------------------------------------
/** Array is the fundamental class underlying easyLink data manipulation.
 *
 * Array is used to access simulink input, output and parameter ports as well
 * as MATLAB matrix and mxArray.
 *
 * Array provides intuitive functions and operators to perform various
 * operations using a MATLAB like interface.
 */
class Array
{
public:

    /** Default constructor. */
    Array(string name="untitled array")
    {
        nrows=0;
        ncols=0;
        mxarray=NULL;
        data=NULL;
        this->name=name;
        moveable=false;
        releaseData=false;
#ifdef __TEST__
            printf("easyLink test message: constructing empty array called \"%s\".\n",getFullName().c_str());
#endif
    }

    /** Construct an Array and allocate memory for data.
     *
     * Set moveable to true if the Array is used as a return value (and
     * to avoid data copy in this case). */
    Array(int nrows,int ncols=1,string name="untitled array",bool moveable=false)
    {
        this->nrows=nrows;
        this->ncols=ncols;
        this->mxarray=NULL;
        this->data=new double[nrows*ncols];
        this->name=name;
        this->moveable=moveable;
        this->releaseData=true;
        memset((void*)data,0,nrows*ncols*sizeof(double));
#ifdef __TEST__
        allocationNumber++;
        printf("easyLink test message: constructing array called \"%s\".\n",getFullName().c_str());
#endif
    }

    /** Construct an Array using an existing mxArray.
     * The constructor do NOT allocate and NOT copy the data if dataCopy is false.
     *
     * Set moveable to true if the Array is used as a return value (and
     * to avoid data copy in this case). */
    Array(const mxArray *mxarray,string name="untitled mxArray",bool moveable=false,bool dataCopy=false)
    {
        if (mxIsSparse(mxarray)||!mxIsDouble(mxarray))
            throw runtime_error("easyLink error: the array must be a dense array of real values.");

        this->nrows=(int)mxGetM(mxarray);
        this->ncols=(int)mxGetN(mxarray);
        if (dataCopy)
        {
            this->mxarray=NULL;
            this->data=new double[nrows*ncols];
            memcpy((void*)data,(void*)mxGetPr(mxarray), nrows*ncols*sizeof(double) );
            this->releaseData=true;
#ifdef __TEST__
        allocationNumber++;
#endif
        }
        else
        {
            this->mxarray=(mxArray*)mxarray;
            this->data=mxGetPr(mxarray);
            this->releaseData=false;
        }
        this->name=name;
        this->moveable=moveable;
#ifdef __TEST__
        printf("easyLink test message: constructing array (using allocated mxarray) called \"%s\".\n",getFullName().c_str());
#endif
    }

    /** Construct an Array with already allocated data without using mxArray.
     * The constructor do NOT allocate and NOT copy the data.
     *
     * Set moveable to true if the Array is used as a return value (and
     * to avoid data copy in this case).
     *
     * Set releaseData to true if the Array must free the memory during deletion. */
    Array(const double *data,int nrows,int ncols=1,string name="untitled data",bool moveable=false,bool releaseData=false)
    {
        this->nrows=nrows;
        this->ncols=ncols;
        this->mxarray=NULL;
        this->data=(double*)data;
        this->name=name;
        this->moveable=moveable;
        this->releaseData=releaseData;
#ifdef __TEST__
        printf("easyLink test message: constructing array (using allocated double array) \"%s\".\n",getFullName().c_str());
#endif
    }

    /** Copy constructor. The method generally does a copy of the data. If the
     * argument is moveable, the data are stolen to this argument before its
     * deletion (no data copy). */
    Array(const Array &array)
    {
        if (array.moveable)
            stealData(array);
        else
        {
#ifdef __TEST__
        printf("easyLink test message: hard copy of array \"%s\" in copy constructor.\n",array.name.c_str());
#endif
            copyOf(array);
        }
    }

    /** Move constructor. C++ 2011 only.*/
#ifdef __CPP2011__
    Array(const Array && array)
    {
        stealData(array);
    }
#endif

    /** Destructor. Free the memory if not shared. */
    ~Array()
    {
#ifdef __TEST__
            printf("easyLink test message: destroying \"%s\".\n",getFullName().c_str());
#endif
        empty();
    }

    /** Empty the array and free the memory if owned. */
    void empty()
    {
        if (releaseData)
        {
#ifdef __TEST__
            allocationNumber--;
#endif
                delete [] data;
        }
        nrows=0;
        ncols=0;
        mxarray=NULL;
        data=NULL;
        moveable=false;
        releaseData=false;
    }

    /** Array assignment. The method generally does a copy of the data. If the
     * argument is moveable, the data are stolen to this argument before its
     * deletion (no data copy).
     *
     * Throws an exception if the data is shared and sizes don't match. */
    void operator=(const Array &array)
    {
        if (nrows==0 && ncols==0) // empty array
        {
            if (array.moveable)
                stealData(array);
            else
            {
#ifdef __TEST__
                printf("easyLink test message: hard copy in assignment \"%s=%s\".\n",name.c_str(),array.name.c_str());
#endif
                copyOf(array);
            }
        }
        else if (releaseData)
        {
            if (array.moveable)
            {
                empty();
                stealData(array);
            }
            else if (nrows==array.nrows && ncols==array.ncols)
            {
#ifdef __TEST__
                printf("easyLink test message: hard copy in assignment \"%s=%s\".\n",name.c_str(),array.name.c_str());
#endif
                memcpy((void*)data,(void*)array.data, nrows*ncols*sizeof(double) );
            }
            else
            {
#ifdef __TEST__
                printf("easyLink test message: hard copy in assignment \"%s=%s\".\n",name.c_str(),array.name.c_str());
#endif
                empty();
                copyOf(array);
            }
        }
        else // shared data
        {
            if (nrows!=array.nrows || ncols!=array.ncols)
                throw runtime_error("easyLink error: Unable to assign "+array.name+" to shared array "+name+". Array dimensions must agree.");

#ifdef __TEST__
                printf("easyLink test message: hard copy in assignment \"%s=%s\".\n",name.c_str(),array.name.c_str());
#endif
            memcpy((void*)data,(void*)array.data, nrows*ncols*sizeof(double) );
        }
    }

    /** Move assignment. C++ 2011 only. */
#ifdef __CPP2011__
    void operator=(const Array &&array)
    {
        if (nrows==0 && ncols==0) // empty array
        {
            stealData(array);
        }
        else if (releaseData)
        {
            empty();
            stealData(array);
        }
        else // shared data
        {
            if (nrows!=array.nrows || ncols!=array.ncols)
                throw runtime_error("easyLink error: Unable to move assign "+array.name+" to shared array "+name+". Array dimensions must agree.");

#ifdef __TEST__
                printf("easyLink test message: hard copy in assignment \"%s=%s\".\n",name.c_str(),array.name.c_str());
#endif
            memcpy((void*)data,(void*)array.data, nrows*ncols*sizeof(double) );
        }
    }
#endif

    /** Returns the address of the data. */
    double* getData()
    {
        return data;
    }

    /** Returns a copy of the array within a mxArray (data copy).
     *
     * The obtained mxArray must be released using mxDestroyArray. */
    mxArray* getmxArrayCopy() const
    {
        mxArray *copy;
        copy=mxCreateDoubleMatrix(nrows,ncols,mxREAL);
        memcpy((void*)mxGetPr(copy),(void*)data, nrows*ncols*sizeof(double) );
        return copy;
    }

    /** Returns true is the array does not own the data. */
    bool isShared()
    {
        return !releaseData;
    }
    
    /** Returns the number of elements of the array.
     * For 1-D array with w elements, this method returns w.
     * For M-by-N array, this method returns m*n. */
    int getWidth()
    {
        return nrows*ncols;
    }

    /** Returns the number of columns of the array. */
    int getNCols()
    {
        return ncols;
    }

    /** Returns the number of rows of the array. */
    int getNRows()
    {
        return nrows;
    }

    /** Returns the name of the array. */
    string getName()
    {
        return name;
    }

    /** Read/write access to the element i of the array.
     * i can go from 0 to nrows*ncols-1.
     * Range errors throw an exception. */
    double & operator[](int i)
    {
        if ( (i<0) || (i>=nrows*ncols) )
            throw range_error("easyLink error: index exceeds array dimensions when accessing to "+name+"["+toString(i)+"].");
        return *(data+i);
    }

    /** Read/write access to the element (row,col) of the array.
        Range errors throw an exception. */
    double & operator()(int row,int col)
    {
        if ( (row<0) || (row>=nrows) || (col<0) || (col>=ncols) )
            throw range_error("easyLink error: index exceeds array dimensions when accessing to "+name+"("+toString(row)+","+toString(col)+").");
        return *(data+row+nrows*col);
    }

    /** Initialization of all elements at the same value. */
    void init(double x=0)
    {
        int i=nrows*ncols;
        double* p=data;
        for (; i--; p++)
            *p=x;
    }

    /** Reshaped to a new size with the same elements
     * nrows*ncols must equal the number of elements of the array. */
    void reshape(int nrows,int ncols)
    {
        if (this->nrows*this->ncols!=nrows*ncols)
            throw runtime_error("easyLink error: unable to reshape "+name+".");
        
        this->nrows=nrows;
        this->ncols=ncols;
    }

    /** Print the array in the console. */
    void print()
    {
#ifdef __TEST__
        printf(getFullName().c_str());
#else
        printf(name.c_str());
#endif
        printf(" = \n");
        printf("[");
        for (int row=0; row<nrows; row++)
        {
            for (int col=0; col<ncols; col++)
                printf("  %7g",data[row+nrows*col]);

            if (row!=nrows-1)
                printf("\n ");
        }

        printf("]\n");
    }
    
    /** Call internal MATLAB numeric functions, MATLAB operators,
     * or user-defined functions and applied it to the array. */
    Array callMatlab(string cmd)
    {
        mxArray *input,*output;

        if (mxarray==NULL)
            input=getmxArrayCopy();
        else
            input=mxarray;

        mexCallMATLAB(1,&output,1,&input,cmd.c_str());

        Array result(output,cmd+"("+name+")",true,true);
        mxDestroyArray(output);

        if (mxarray==NULL)
            mxDestroyArray(input);

        return result;
    }

    /** Call internal MATLAB numeric functions, MATLAB operators,
     * or user-defined functions and applied it to the array and to a second
     * operand. */
    Array callMatlab(string cmd,const Array & operand2)
    {
        mxArray *input[2],*output;

        if (mxarray==NULL)
            input[0]=getmxArrayCopy();
        else
            input[0]=mxarray;

        if (operand2.mxarray==NULL)
            input[1]=operand2.getmxArrayCopy();
        else
            input[1]=operand2.mxarray;

        mexCallMATLAB(1,&output,2,input,cmd.c_str());

        Array result(output,cmd+"("+name+","+operand2.name+")",true,true);
        mxDestroyArray(output);

        if (mxarray==NULL)
            mxDestroyArray(input[0]);

        if (operand2.mxarray==NULL)
            mxDestroyArray(input[1]);

        return result;
    }

    /** Call internal MATLAB numeric functions, MATLAB operators,
     * or user-defined functions and applied it to the array and to a second
     * scalar operand. */
    Array callMatlab(string cmd,double operand2)
    {
        mxArray *input[2],*output;

        if (mxarray==NULL)
            input[0]=getmxArrayCopy();
        else
            input[0]=mxarray;

        input[1] = mxCreateDoubleMatrix(1,1,mxREAL);
        *mxGetPr(mxarray)=operand2;
        mexCallMATLAB(1,&output,2,input,cmd.c_str());

        Array result(output,cmd+"("+name+","+toString(operand2)+")",true,true);
        mxDestroyArray(output);

        if (mxarray==NULL)
            mxDestroyArray(input[0]);
        mxDestroyArray(input[1]);

        return result;
     }

    /** In-place element-by-element addition. */
    void operator+=(double x)
    {
        int i=nrows*ncols;
        double* p=data;
        for (;i--;p++)
            *p+=x;
    }

    /** In-place element-by-element substraction. */
    void operator-=(double x)
    {
        int i=nrows*ncols;
        double* p=data;
        for (;i--;p++)
            *p-=x;
    }

    /** In-place element-by-element multiplication. */
    void operator*=(double x)
    {
        int i=nrows*ncols;
        double* p=data;
        for (;i--;p++)
            *p*=x;
    }

    /** In-place element-by-element division. */
    void operator/=(double x)
    {
        int i=nrows*ncols;
        double* p=data;
        for (;i--;p++)
            *p/=x;
    }

    /** Array-double addition. */
    Array operator+(double x)
    {
        Array result(*this);
        result+=x;
        result.moveable=true;
        result.name="("+name+"+"+toString(x)+")";
        return result;
    }

    /** Array-double substraction. */
    Array operator-(double x)
    {
        Array result(*this);
        result-=x;
        result.moveable=true;
        result.name="("+name+"-"+toString(x)+")";
        return result;
    }

    /** Array-double multiplication. */
    Array operator*(double x)
    {
        Array result(*this);
        result*=x;
        result.moveable=true;
        result.name="("+name+"*"+toString(x)+")";
        return result;
    }

    /** Array-double division. */
    Array operator/(double x)
    {
        Array result(*this);
        result/=x;
        result.moveable=true;
        result.name="("+name+"/"+toString(x)+")";
        return result;
    }

    /** In-place element-by-element addition.
     * Arrays must have the same dimensions. */
    void operator+=(const Array & operand)
    {
        if (nrows!=operand.nrows || ncols!=operand.ncols)
            throw runtime_error("easyLink error: Unable to add "+name+" and "+operand.name+". Array dimensions must agree.");

        int   i=nrows*ncols;
        double* p=data;
        double* s=operand.data;
        for (;i--;p++,s++)
            *p+=(*s);
    }

    /** In-place element-by-element substraction.
     * Arrays must have the same dimensions. */
    void operator-=(const Array & operand)
    {
        if (nrows!=operand.nrows || ncols!=operand.ncols)
            throw runtime_error("easyLink error: Unable to substract "+name+" and "+operand.name+". Array dimensions must agree.");

        int   i=nrows*ncols;
        double* p=data;
        double* s=operand.data;
        for (;i--;p++,s++)
            *p-=(*s);
    }

    /** In-place element-by-element multiplication.
     * Arrays must have the same dimensions. */
    void operator*=(const Array & operand)
    {
        if (nrows!=operand.nrows || ncols!=operand.ncols)
            throw runtime_error("easyLink error: Unable to multiply "+name+" and "+operand.name+". Array dimensions must agree.");

        int   i=nrows*ncols;
        double* p=data;
        double* s=operand.data;
        for (;i--;p++,s++)
            *p*=(*s);
    }

    /** In-place element-by-element division.
     * Arrays must have the same dimensions. */
    void operator/=(const Array & operand)
    {
        if (nrows!=operand.nrows || ncols!=operand.ncols)
            throw runtime_error("easyLink error: Unable to divide "+name+" and "+operand.name+". Array dimensions must agree.");

        int   i=nrows*ncols;
        double* p=data;
        double* s=operand.data;
        for (;i--;p++,s++)
            *p/=(*s);
    }

    /** Array-array addition. */
    Array operator+(const Array & operand)
    {
        return callMatlab("plus",operand);
    }

    /** Array-array substraction. */
    Array operator-(const Array & operand)
    {
        return callMatlab("minus",operand);
    }

    /** Array-array multiplication (matrix product). */
    Array operator*(const Array & operand)
    {
        return callMatlab("mtimes",operand);
    }

    /** Element-by-element comparison.
     * Returns true if the array contains the same values.
     * Use areEqual to compare double values. */
    bool operator==(const Array & operand)
    {
        bool result;
        if (nrows*ncols!=operand.nrows*operand.ncols)
            result=false;
        else
        {
            int i=nrows*ncols;
            double *p=data;
            double *q=operand.data;
            for (;i-- && areEqual(*p,*q);p++,q++)
                ;
            result=(i<0);
        }
        return result;
    }

    /** Returns the maximal value of the array */
    double getMax()
    {
        int i=nrows*ncols-1;
        double *p=data;
        double result=*p;
        p++;
        for (;i--;p++)
            if (*p>result)
                result=*p;

        return result;
    }

    /** Returns the minimal value of the array */
    double getMin()
    {
        int i=nrows*ncols-1;
        double *p=data;
        double result=*p;
        p++;
        for (;i--;p++)
            if (*p<result)
                result=*p;

        return result;
    }

    /** Returns the additive inverse of the array. */
    Array opposite()
    {
        Array result(*this);
        int i=nrows*ncols;
        double* p=result.data;
        for (;i--;p++)
            *p=-(*p);
        result.moveable=true;
        result.name="-"+name;
        return result;
    }

    /** Returns the inverse of the array. */
    Array inverse()
    {
        return callMatlab("inv");
    }

    /** Return the identity matrix. */
    static Array eye(int nrows,int ncols,string name="eye array")
    {
        Array result(nrows,ncols,name,true);

        for(int i=0; i<min(nrows,ncols); i++)
        {	result(i,i) = 1;
        }

        return result;
    }

protected:

    int ncols,nrows;
    mxArray *mxarray;
    double *data;
    string name;
    bool moveable,releaseData;

    inline void stealData(const Array & array)
    {
        nrows=array.nrows;
        ncols=array.ncols;
        mxarray=array.mxarray;
        data=array.data;
        name=array.name;
        moveable=false;
        releaseData=array.releaseData;
        *(bool*)(&array.releaseData)=false; // Trick to write a member of a const argument
    }

    inline void copyOf(const Array & array)
    {
        nrows=array.nrows;
        ncols=array.ncols;
        mxarray=NULL;
        data=new double[nrows*ncols];
        memcpy((void*)data,(void*)array.data, nrows*ncols*sizeof(double) );
        name=string("copy of ")+array.name;
        moveable=false;
        releaseData=true;
#ifdef __TEST__
        allocationNumber++;
#endif
    }

    inline string getFullName()
    {
        string result=name+" ("+toString(nrows)+"x"+toString(ncols)+" ";

        if (moveable)
            result+="moveable ";
        if (releaseData)
            result+="releaseData ";
        if (mxarray!=NULL)
            result+="mex array)";
        else
            result+="double array)";

        return  result;
    }

#ifdef __TEST__    
public:
    static int allocationNumber;
#endif
};

#ifdef __TEST__    
int Array::allocationNumber = 0;
#endif

/** Double-array addition. */
Array operator+(double x,Array & operand)
{
    return operand.operator+(x);
}

/** Double-array substraction. */
Array operator-(double x,Array & operand)
{
    return operand.opposite().operator+(x);
}

/** Double-array multiplication. */
Array operator*(double x,Array & operand)
{
    return operand.operator*(x);
}

//------------------------------------------------------------------------------
#endif
