/*
 * This file is part of SPORE.
 * Copyright (c) 2016-2017, Institute for Theoretical Computer Science,
 * Graz University of Technology
 *
 * SPORE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * SPORE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SPORE.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information see: https://github.com/IGITUGraz/spore-nest-module
 *
 * File:   param_utils.h
 * Author: kappel
 *
 * Created on March 31, 2017, 6:10 PM
 */

#ifndef PARAM_UTILS_H
#define	PARAM_UTILS_H

#include <string>
#include <sstream>
#include <exception>

#include "nest.h"
#include "exceptions.h"
#include "dictdatum.h"


namespace spore
{

/**
 * @brief: Definition of parameter conditions.
 */
namespace pc
{
/**
 * @brief Base class to all conditions that can be imposed onto parameters.
 */
template < typename T >
    class Con
{
public:
    virtual void apply( T const &, const std::string &name ) const = 0;
};

/**
 * @brief Minimum condition. Parameter must be larger or equal to given value.
 */
template < typename T >
    class CMin : public Con < T >
{
public:
    CMin( T min_value )
    : min_value_( min_value )
    {
    }

    void apply( T const & val, const std::string &name ) const
    {
        if ( val < min_value_ )
        {
            // Fixme: sending a dynamically allocated string here causes program abort.
            //std::stringstream strm;
            //strm << "Parameter '" << name << "' must not be smaller than " << T(min_value_) << " but is " << T(val);
            //std::string msg = strm.str();
            throw nest::BadProperty("Parameter out of range.");
        }
    }

private:
    T min_value_;
};

/**
 * @brief "Bigger than" condition. Parameter must be larger or equal to given value.
 */
template < typename T >
    class CBigger : public Con < T >
{
public:
    CBigger( T min_value )
    : min_value_( min_value )
    {
    }

    void apply( T const & val, const std::string &name ) const
    {
        if ( val <= min_value_ )
        {
            throw nest::BadProperty("Parameter out of range.");
        }
    }

private:
    T min_value_;
};

typedef CMin< double > MinD;
typedef CMin< long >   MinL;
typedef CMin< int > MinI;
typedef CBigger< double >   BiggerD;
typedef CBigger< long >   BiggerL;
typedef CBigger< int >   BiggerI;

}

/**
 * @brief: Class used to set up the default arguments.
 */
class SetDefault
{
public:
    SetDefault();

    /**
     * @brief Define parameter without constraint.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T default_val )
    {
        val = default_val;
    }

    /**
     * @brief Define parameter with 1 constraint.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1 constraint to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T default_val, const pc::Con< T > &con1 )
    {
        con1.apply(default_val, name);
        val = default_val;
    }

    /**
     * @brief Define parameter with 2 constraints.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1,con2 constraints to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T default_val, const pc::Con< T > &con1, const pc::Con< T > &con2 )
    {
        con1.apply(default_val, name);
        con2.apply(default_val, name);
        val = default_val;
    }

    /**
     * @brief Define parameter with 3 constraints.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1,con2,con3 constraints to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T default_val, const pc::Con< T > &con1, const pc::Con< T > &con2,
                        const pc::Con< T > &con3 )
    {
        con1.apply(default_val, name);
        con2.apply(default_val, name);
        con3.apply(default_val, name);
        val = default_val;
    }
};

/**
 * @brief: Class used to set up the default arguments.
 */
class GetStatus
{
public:
    GetStatus(DictionaryDatum & d);

    /**
     * @brief Define parameter without constraint.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T )
    {
        def< T >(d_, name, val);
    }

    /**
     * @brief Define parameter with 1 constraint.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1 constraint to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T, const pc::Con< T > & )
    {
        def< T >(d_, name, val);
    }

    /**
     * @brief Define parameter with 2 constraints.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1,con2 constraints to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T, const pc::Con< T > &, const pc::Con< T > & )
    {
        def< T >(d_, name, val);
    }

    /**
     * @brief Define parameter with 3 constraints.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1,con2,con3 constraints to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T, const pc::Con< T > &, const pc::Con< T > &,
                  const pc::Con< T > & )
    {
        def< T >(d_, name, val);
    }

private:
    DictionaryDatum & d_;
};

/**
 * @brief: Class used to set the arguments.
 */
class SetStatus
{
public:
    SetStatus(const DictionaryDatum & d);

    /**
     * @brief Define parameter without constraint.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T )
    {
        updateValue< T >(d_, name, val);
    }

    /**
     * @brief Define parameter with 1 constraint.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1 constraint to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T, const pc::Con< T > &con1 )
    {
        T new_val;
        if (updateValue< T >(d_, name, new_val))
        {
            con1.apply(new_val, name);
            val = new_val;
        }
    }

    /**
     * @brief Define parameter with 2 constraints.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1,con2 constraints to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T, const pc::Con< T > &con1, const pc::Con< T > &con2 )
    {
        T new_val;
        if (updateValue< T >(d_, name, new_val))
        {
            con1.apply(new_val, name);
            con2.apply(new_val, name);
            val = new_val;
        }
    }

    /**
     * @brief Define parameter with 3 constraints.
     * @param val Reference to the variable that holds the parameter.
     * @param name The name assigned to the parameter.
     * @param default_val default value of the parameter.
     * @param con1,con2,con3 constraints to the parameter.
     */
    template < typename T >
        void parameter( T & val, const std::string &name, T, const pc::Con< T > &con1, const pc::Con< T > &con2,
                        const pc::Con< T > &con3 )
    {
        T new_val;
        if (updateValue< T >(d_, name, new_val))
        {
            con1.apply(new_val, name);
            con2.apply(new_val, name);
            con3.apply(new_val, name);
            val = new_val;
        }
    }

private:
    const DictionaryDatum & d_;
};

}

#endif
