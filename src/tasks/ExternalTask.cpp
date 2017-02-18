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

/*
 * Constructor for ExternalTask
 * @param is an array of cstrings. The first cstring should be the name of the program
 *        to run and the rest should be the arguments. The last element in the array should be a NULL pointer.
 */
ExternalTask::ExternalTask(char** a) : args(a) {}

/*
 * Constructor for ExternalTask
 * @param a is a vector of strings that will be used to create the child process.
 *        The first string should be the name of the program to run and the rest should be the arguments.
 */
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

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
/*
 * Runs the given command and arguments as a child process
 * @param r is the EnumResult from the previously run task (not used)
 * @returns EnumResult::FAIL if an error is encountered, otherwise returns EnumResult::PASS
 */
Task::EnumResult ExternalTask::run(Task::EnumResult r)
{
    retrun r;
}
#else
/*
 * Runs the given command and arguments as a child process
 * @param r is the EnumResult from the previously run task (not used)
 * @returns EnumResult::FAIL if an error is encountered, otherwise returns EnumResult::PASS
 */
Task::EnumResult ExternalTask::run(Task::EnumResult r)
{
    pid_t childPid = fork(); // create child process

    if (childPid < 0) // failed to create child process
    {
        perror("Error creating child process");
        return Task::FAIL;
    }
    else if (childPid == 0) // process running is child
    {
        if (execvp(this->args[0], this->args) < 0) // if failed to run program, then print error
        {
            std::string error = "Error running command - ";
            error += this->args[0];
            perror(error.c_str());
            return Task::FAIL;
        }
    }
    else // process running is parent
    {
        int status;
        if (waitpid(childPid, &status, 0) != 0) // wait for child process to finish
        {
            return Task::FAIL; // child process returned a failure
        }

//        if (status && WIFEXITED(status))
//        {
//            std::cout << "DEBUG: Process returned " << WEXITSTATUS(status) << std::endl;
//        }
    }

    return Task::PASS; // we made it to this point so we must be good
}
#endif
