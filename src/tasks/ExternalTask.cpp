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

#include "../../headers/tasks/ExternalTask.h"

ExternalTask::ExternalTask(char** a) : args(a) {}

ExternalTask::ExternalTask(std::vector<std::string> a)
{
    unsigned long size = a.size() + 1;
    this->args = new char*[size];

    for (unsigned long i = 0; i < size - 1; ++i)
    {
        this->args[i] = new char[a.at(i).length() + 1];
        std::strcpy(this->args[i], a.at(i).c_str());
    }

    this->args[size - 1] = NULL;
}

ExternalTask::~ExternalTask()
{
    if (this->args)
    {
        delete[] this->args;
    }
}

Task::EnumResult ExternalTask::run(Task::EnumResult r)
{
    pid_t childPid = fork();

    if (childPid < 0)
    {
        perror("Error creating child process");
        return Task::FAIL;
    }
    else if (childPid == 0)
    {
        if (execvp(this->args[0], this->args) < 0)
        {
            std::string error = "Error running command - ";
            error += this->args[0];
            perror(error.c_str());
            return Task::FAIL;
        }
    }
    else
    {
        int status;
        waitpid(childPid, &status, 0);
        std::cout << "DEBUG: Process returned " << WEXITSTATUS(status) << std::endl;
    }

    return Task::PASS;
}
