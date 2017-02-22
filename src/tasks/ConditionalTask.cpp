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

#include "../../headers/tasks/ConditionalTask.h"

/*
 * Constructor for ConditionalTask
 * @param t is the child task to run
 * @param r is the desired result on which the child task should be run
 */
ConditionalTask::ConditionalTask(Task* t, Task::EnumResult r) : task(t), result(r) {}

ConditionalTask::~ConditionalTask()
{
    if (this->task)
    {
        delete this->task;
    }
}

/*
 * Runs the child task when given the desired EnumResult
 * @param r is the EnumResult from the previously run task
 *
 * @return when given the desired result, this will return the result
 *         of the child task, otherwise this will return EnumResult::SKIP
 */
Task::EnumResult ConditionalTask::run(Task::EnumResult r)
{
    if (r == this->result) // run child task on desired result
    {
        return this->task->run(r);
    }

    // condition was not true; skip any dependent conditional tasks
    return r;
}
