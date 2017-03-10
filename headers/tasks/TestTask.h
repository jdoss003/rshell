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

#ifndef RSHELL_TESTTASK_H
#define RSHELL_TESTTASK_H

#include <cerrno>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "../utils/EnvUtils.h"
#include "Task.h"

class TestTask : public Task
{
    public:
        TestTask(std::vector<std::string> a);
        ~TestTask();
        EnumResult run(EnumResult r);

    private:
        std::vector<std::string> args;

        static const std::string ARG_EXISTS;
        static const std::string ARG_FOLDER;
        static const std::string ARG_DIR;

        static const std::string OUT_TRUE;
        static const std::string OUT_FALSE;

        bool isValidArg(std::string arg);
};

#endif //RSHELL_TESTTASK_H
