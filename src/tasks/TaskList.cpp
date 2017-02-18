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

#include "../../headers/tasks/TaskList.h"

TaskList::TaskList() {}

TaskList::~TaskList()
{
    while (!this->subtasks.empty())
    {
        if (this->subtasks.back())
        {
            delete this->subtasks.back();
        }

        this->subtasks.pop_back();
    }
}

/*
 * Runs all the sub-tasks in order
 * @param r is the EnumResult from the previously run task which is passed to the first sub-task
 * @returns the EnumResult of the last sub-task that is run
 */
Task::EnumResult TaskList::run(Task::EnumResult r)
{
    for (unsigned int i = 0; i < this->subtasks.size(); ++i)
    {
        r = this->subtasks.at(i)->run(r);
    }

    return r;
}

/*
 * Adds sub-task to the task list
 * @param t is the task to be added
 */
void TaskList::addSubtask(Task* t)
{
    this->subtasks.push_back(t);
}
