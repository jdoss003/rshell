/******************************************************************************
 *  rshell
 *  Authors: Derek A. Sayler, Justin Doss
 *  Copyright (C) 2017
 *
 *  This project is an academic assignment (UCR CS 100) and all code
 *  is original work of the authors above. Anyone who copies this project
 *  and submits it as their own academic work is in violation of academic
 *  integrity.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ***************************************************************************/

#include "../../headers/tasks/Task.h"

Task::Task() {}

Task::~Task()
{
    this->inputRedir.closeWrite();
    this->inputRedir.closeRead();
    this->outputRedir.closeWrite();
    this->outputRedir.closeRead();
}

/*
 * Public function to run the task object
 */
void Task::run()
{
    this->run(PASS);
}

/*
 * Basic run implementation
 * @param r is the EnumResult from the previously run task (not used)
 * @returns EnumResult::SKIP
 */
Task::EnumResult Task::run(Task::EnumResult r)
{
    return r;
}

/*
 * Should only be called on derived objects that override this function
 * Throws an error of called
 */
void Task::addSubtask(Task* t)
{
    throw new std::runtime_error("ERROR: Tried to add a subtask to a non TaskList object!");
}

void Task::setInputRedirect(Redirector r)
{
    if (!this->inputRedir.shouldRedirectInput())
    {
        this->inputRedir = r;
    }
}

void Task::setOutputRedirect(Redirector r)
{
    if (!this->outputRedir.shouldRedirectOutput())
    {
        this->outputRedir = r;
    }
}
