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

/*
 * Gets the current user's home directory and returns it as a string
 *
 * @returns user's home directory or an empty string if the directory could not be found
 */
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

/*
 * Gets the current working directory and returns it as a string
 *
 * @returns current working directory or an empty string if the directory could not be found
 */
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

/*
 * Transforms a relative path into a complete working path
 *
 * @returns the complete path or an empty string if an error occurred while creating the path
 */
std::string TestTask::getCompletePath(std::string relativePath)
{
    if (relativePath.empty() || relativePath[0] == '/') // nothing to do
    {
        return relativePath;
    }
    else if (relativePath[0] == '~') // add user dir to front
    {

        std::string hDirPath = getUserDir();

        relativePath.erase(0, 1);

        hDirPath.append(relativePath);

        return hDirPath;
    }
    else
    {
        std::string cDirPath = getCurrentDir();

        if (cDirPath.empty()) // could not get the CWD so that's a problem...
        {
            return "";
        }

        unsigned long index = relativePath.find("./"); // do we just add the CWD to the front?

        if (index == 0) // yes we do
        {
            relativePath.erase(0, 2);
        }

        // we need to go up a level (maybe more than once)
        while ((index = relativePath.find("../")) != std::string::npos)
        {
            if (index != 0) // the string was found but its not at the front so we don't care
            {
                break;
            }

            relativePath.erase(0, 3); // erase the ../ on the relative path string


            // could not find a / in the CWD path or it is the last one
            // either way this is a problem so we just exit here
            if ((index = cDirPath.rfind('/')) == std::string::npos || index == 0)
            {
                return "";
            }

            cDirPath = cDirPath.substr(0, index); // remove the last dir in the CWD path
        }

        cDirPath.append("/" + relativePath); // add the relative path to the end of the CWD path

        return cDirPath;
    }
}

/*
 * Helper function to determine if a string is a valid arg for the command
 *
 * @returns true if the string is a valid arg, otherwise false
 */
bool TestTask::isValidArg(std::string arg)
{
    return (arg.compare(this->ARG_EXISTS) == 0 || arg.compare(this->ARG_FOLDER) == 0 || arg.compare(this->ARG_DIR) == 0);
}

TestTask::TestTask(std::vector<std::string> a) : args(a) {}

TestTask::~TestTask() {}

/*
 * Test whether the specified path exists and is a file or dir
 *
 * @returns EnumResult::FAIL if the file/dir does not exist or is not of the specified type,
 *          otherwise returns EnumResult::PASS
 */
Task::EnumResult TestTask::run(Task::EnumResult r)
{
    if (this->args.at(0).compare("[") == 0) // test if it is the bracket form
    {
        if (this->args.size() <= 4 && this->args.at(this->args.size() - 1).compare("]") == 0) // is it valid?
        {
            this->args.pop_back(); // yep! so we can just remove the last arg (]) and everything works like normal
        }
        else // something is not right; throw an error
        {
            std::cout << "test : malformed test command" << std:: endl;
            return Task::FAIL;
        }
    }

    if (this->args.size() == 1 || this->args.size() > 3) // do we have a valid number of args?
    {
        std::cout << "test : invalid number of arguments" << std:: endl;
        return Task::FAIL;
    }

    std::string arg = "";
    std::string fullPath = "";

    if (this->args.size() == 2)
    {
        if (this->args.at(1)[0] == '-') // so there is no path arg :/
        {
            if (!isValidArg(this->args.at(1))) // and the arg given is invalid?
            {
                std::cout << "test : invalid argument '" << this->args.at(1) << "'" << std:: endl;
                return Task::FAIL;
            }

            std::cout << "test : path argument not specified" << std:: endl;
            return Task::FAIL;
        }

        // looks like we are ok; set the default arg and get the complete path
        arg = this->ARG_EXISTS;
        fullPath = getCompletePath(this->args.at(1));
    }
    else
    {
        if (this->args.at(1)[0] != '-' || !isValidArg(this->args.at(1))) // is the first arg valid?
        {
            std::cout << "test : invalid argument '" << this->args.at(1) << "'" << std:: endl;
            return Task::FAIL;
        }

        // looks like we are ok; set the arg and get the complete path
        arg = this->args.at(1);
        fullPath = getCompletePath(this->args.at(2));
    }

//    std::cout << "DEBUG: " << fullPath << std::endl;

    struct stat sb;

    if (!fullPath.empty() && stat(fullPath.c_str(), &sb) != -1) // try to get stat on the file/dir
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

    errno = 0; // clear the error number if stat fails; causes problems later

    std::cout << this->OUT_FALSE << std::endl;
    return Task::FAIL;
}
