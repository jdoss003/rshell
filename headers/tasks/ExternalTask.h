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

#ifndef RSHELL_EXTERNALTASK_H
#define RSHELL_EXTERNALTASK_H

#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "Task.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

#else
#include <sys/wait.h>
#endif

class ExternalTask : public Task
{
    public:
        ExternalTask(char** a);
        ExternalTask(std::vector<std::string> a);
        ~ExternalTask();
        EnumResult run(EnumResult r);

    private:
        char** args;
};

#endif //RSHELL_EXTERNALTASK_H
