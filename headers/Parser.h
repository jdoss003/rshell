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

#ifndef RSHELL_PARSER_H
#define RSHELL_PARSER_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdio.h>
#include <string>
#include <vector>

#include "../headers/tasks/ConditionalTask.h"
#include "../headers/tasks/ExitTask.h"
#include "../headers/tasks/ExternalTask.h"
#include "../headers/tasks/Task.h"
#include "../headers/tasks/TaskList.h"
#include "../headers/tasks/TestTask.h"
#include "../headers/utils/FileUtils.h"

namespace Parser
{
    Task* parseInput(std::string strInput);
}

#endif //RSHELL_PARSER_H
