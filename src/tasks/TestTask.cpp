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

#include "../../headers/tasks/TestTask.h"

const std::string TestTask::ARG_EXISTS = "-e";
const std::string TestTask::ARG_FOLDER = "-f";
const std::string TestTask::ARG_DIR    = "-d";

const std::string TestTask::OUT_TRUE  = "(True)";
const std::string TestTask::OUT_FALSE = "(False)";

std::string TestTask::getUserDir()
{
    char *hDir;
    std::string homeDir = "";

    hDir = getenv("HOME");
    if (hDir)
    {
        homeDir.append(hDir);
    }
    else
    {
        hDir = getpwuid(getuid())->pw_dir;

        if (hDir)
        {
            homeDir.append(hDir);
        }
    }

    return homeDir;
}

std::string TestTask::getCurrentDir()
{
    unsigned long size = 1024;
    char* cDir = new char[size];

    std::string cDirPath = "";

    while (getcwd(cDir, size) == NULL && errno == ERANGE)
    {
        size *= 2;
        cDir = new char[size];
    }

    if (cDir)
    {
        cDirPath.append(cDir);
        delete[] cDir;
    }

    return cDirPath;
}

std::string TestTask::getCompletePath(std::string relativePath)
{
    if (relativePath.empty() || relativePath[0] == '/')
    {
        return relativePath;
    }
    else if (relativePath[0] == '~')
    {

        std::string hDirPath = getUserDir();

        relativePath.erase(0, 1);

        hDirPath.append(relativePath);

        return hDirPath;
    }
    else
    {
        std::string cDirPath = getCurrentDir();

        if (cDirPath.empty())
        {
            return "";
        }

        unsigned long index = relativePath.find("./");

        if (index == 0)
        {
            relativePath.erase(0, 2);
        }

        while ((index = relativePath.find("../")) != std::string::npos)
        {
            if (index != 0)
            {
                break;
            }

            relativePath.erase(0, 3);


            if ((index = cDirPath.rfind('/')) == std::string::npos || index == 0)
            {
                return "";
            }

            cDirPath = cDirPath.substr(0, index);
        }

        cDirPath.append("/" + relativePath);

        return cDirPath;
    }
}

bool TestTask::isValidArg(std::string arg)
{
    return (arg.compare(this->ARG_EXISTS) == 0 || arg.compare(this->ARG_FOLDER) == 0 || arg.compare(this->ARG_DIR) == 0);
}

TestTask::TestTask(std::vector<std::string> a) : args(a) {}

TestTask::~TestTask() {}

Task::EnumResult TestTask::run(Task::EnumResult r)
{
    if (this->args.at(0).compare("[") == 0)
    {
        if (this->args.size() <= 4 && this->args.at(this->args.size() - 1).compare("]") == 0)
        {
            this->args.pop_back();
        }
        else
        {
            std::cout << "test : malformed test command" << std:: endl;
            return Task::FAIL;
        }
    }

    if (this->args.size() == 1 || this->args.size() > 3)
    {
        std::cout << "test : invalid number of arguments" << std:: endl;
        return Task::FAIL;
    }

    std::string arg = "";
    std::string fullPath = "";

    if (this->args.size() == 2)
    {
        if (this->args.at(1)[0] == '-')
        {
            if (!isValidArg(this->args.at(1)))
            {
                std::cout << "test : invalid argument '" << this->args.at(1) << "'" << std:: endl;
                return Task::FAIL;
            }

            std::cout << "test : path argument not specified" << std:: endl;
            return Task::FAIL;
        }

        arg = this->ARG_EXISTS;
        fullPath = getCompletePath(this->args.at(1));
    }
    else
    {
        if (this->args.at(1)[0] != '-' || !isValidArg(this->args.at(1)))
        {
            std::cout << "test : invalid argument '" << this->args.at(1) << "'" << std:: endl;
            return Task::FAIL;
        }

        arg = this->args.at(1);
        fullPath = getCompletePath(this->args.at(2));
    }

//    std::cout << "DEBUG: " << fullPath << std::endl;

    struct stat sb;

    if (!fullPath.empty() && stat(fullPath.c_str(), &sb) != -1)
    {
        if((arg.compare(this->ARG_EXISTS) == 0 || arg.compare(this->ARG_FOLDER) == 0) && S_ISREG(sb.st_mode))
        {
            std::cout << this->OUT_TRUE << std::endl;
            return Task::PASS;
        }

        if ((arg.compare(this->ARG_EXISTS) == 0 || arg.compare(this->ARG_DIR) == 0) && S_ISDIR(sb.st_mode))
        {
            std::cout << this->OUT_TRUE << std::endl;
            return Task::PASS;
        }
    }

    errno = 0;

    std::cout << this->OUT_FALSE << std::endl;
    return Task::FAIL;
}
