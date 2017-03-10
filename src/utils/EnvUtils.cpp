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

#include "../../headers/utils/EnvUtils.h"

/*
 * Transforms a relative path into a complete working path
 *
 * @returns the complete path or an empty string if an error occurred while creating the path
 */
std::string EnvUtils::getCompletePath(std::string relativePath)
{
    if (relativePath.empty() || relativePath[0] == '/') // nothing to do
    {
        return relativePath;
    }
    else if (relativePath[0] == '~') // add user dir to front
    {

        std::string hDirPath = EnvUtils::getUserDir();

        relativePath.erase(0, 1);

        hDirPath.append(relativePath);

        return hDirPath;
    }
    else
    {
        std::string cDirPath = EnvUtils::getCurrentDir();

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
 * Gets the current working directory and returns it as a string
 *
 * @returns current working directory or an empty string if the directory could not be found
 */
std::string EnvUtils::getCurrentDir()
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

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
/*
 * Returns the name of the current logged in user
 */
std::string EnvUtils::getUserName()
{
    char* usr = NULL;
    std::string userName = "";

    usr = std::getenv("USERNAME");
    if (usr)
    {
        userName.append(usr);
        usr = NULL;
    }
    return userName;
}

/*
 * Returns the name of the current host machine
 */
std::string EnvUtils::getHostName()
{
    char* hst = NULL;
    std::string hostName = "";

    hst = std::getenv("COMPUTERNAME");
    if (hst)
    {
        hostName.append(hst);
    }
    return hostName;
}
#else
/*
 * Returns the name of the current logged in user
 */
std::string EnvUtils::getUserName()
{
    char* usr = NULL;
    std::string userName = "";

    usr = getenv("USER");
    if (usr)
    {
        userName.append(usr);
    }
    else
    {
        usr = new char[_POSIX_LOGIN_NAME_MAX];
        if (getlogin_r(usr, _POSIX_LOGIN_NAME_MAX) == 0)
        {
            userName.append(usr);
        }
        delete[] usr;
    }
    return userName;
}

/*
 * Returns the name of the current host machine
 */
std::string EnvUtils::getHostName()
{
    char* hst = NULL;
    std::string hostName = "";

    hst = getenv("COMPUTERNAME");
    if (hst)
    {
        hostName.append(hst);
    }
    else
    {
        hst = new char[_POSIX_HOST_NAME_MAX];
        if (gethostname(hst, _POSIX_HOST_NAME_MAX) == 0)
        {
            hostName.append(hst);
        }
        delete[] hst;
    }

    if (!hostName.empty())
    {
        unsigned long i = hostName.find('.');

        if (i != std::string::npos)
        {
            hostName = hostName.substr(0, i);
        }
    }

    return hostName;
}
#endif

/*
 * Gets the current user's home directory and returns it as a string
 *
 * @returns user's home directory or an empty string if the directory could not be found
 */
std::string EnvUtils::getUserDir()
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
