/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    StaticCommand.h
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @date    Wed, 13.02.2008
/// @version $Id$
///
// A wrapper for a Command function
/****************************************************************************/
#ifndef StaticCommand_h
#define StaticCommand_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include "Command.h"


// ===========================================================================
// class definition
// ===========================================================================
/**
 * @class StaticCommand
 * @brief A wrapper for a Command function
 *
 * @see Design Patterns, Gamma et al.
 * @see Command
 * @see MSEventControl
 */
template< class T  >
class StaticCommand : public Command {
public:
    /// @brief Type of the function to execute.
    typedef SUMOTime(*Operation)(SUMOTime);


public:
    /**
     * @brief Constructor.
     *
     * @param[in] receiver Pointer to object of type T that will receive a call to one of it's methods.
     * @param[in] operation The objects' method that will be called on execute()
     */
    StaticCommand(Operation operation)
        : myOperation(operation), myAmDescheduledByParent(false) {}


    /// @brief Destructor
    ~StaticCommand() {}


    /** @brief Marks this Command as being descheduled
     *
     * A simple boolean marker ("myAmDescheduledByParent") is set which
     *  prevents this command from being executed.
     */
    void deschedule() {
        myAmDescheduledByParent = true;
    }



    /// @name Derived from Command
    /// @{

    /** @brief Executes the command.
     *
     * If the command is not descheduled, the stored method of the stored instance
     *  is called.
     *
     * @param[in] currentTime The current simulation time
     * @return The time after which the command shall be executed again, 0 if this command shall be descheduled.
     * @exception ProcessError Derived actions may throw this exception
     */
    SUMOTime execute(SUMOTime currentTime) {
        // do not execute if the command was descheduled
        if (myAmDescheduledByParent) {
            return 0;
        }
        // execute if stil valid
        return (*myOperation)(currentTime);
    }
    /// @}


private:
    /// @brief The object's operation to perform.
    Operation myOperation;

    /// @brief Whether this command was descheduled (is invalid) and shall not be executed
    bool myAmDescheduledByParent;


};


#endif

/****************************************************************************/

