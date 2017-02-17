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

#include "../headers/RShell.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
std::string getUserName()
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

std::string getHostName()
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
std::string getUserName()
{
    char* usr = NULL;
    std::string userName = "";

    usr = std::getenv("USER");
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

std::string getHostName()
{
    char* hst = NULL;
    std::string hostName = "";

    hst = std::getenv("COMPUTERNAME");
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

RShell::RShell() : task(NULL), userName(""), hostName(""), prompt(""), input("") {}

RShell::~RShell()
{
    if (this->task)
    {
        delete this->task;
    }
}

void RShell::runLoop()
{
    this->userName = getUserName();
    this->hostName = getHostName();

    this->prompt = (!this->userName.empty() && !this->hostName.empty() ? this->userName + "@" + this->hostName + " $ " : "$ ");

    while (true)
    {
        std::cout << prompt;

        std::getline(std::cin, this->input);

        this->task = Parser::parseInput(this->input);

        if (this->task)
        {
            this->task->run();
            delete this->task;
        }
        else
        {
            std::cout << "ERROR: Received a NULL Task pointer from the parser!" << std::endl;
        }
    }
}
