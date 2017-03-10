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

#include "../headers/Redirector.h"

Redirector::Redirector(int r, int w) : readFD(r), writeFD(w) {}

bool Redirector::shouldRedirectInput()
{
    return (readFD != STDIN_FILENO);
}

bool Redirector::shouldRedirectOutput()
{
    return (writeFD != STDOUT_FILENO);
}

void Redirector::doRedirectInput()
{
    if (this->shouldRedirectInput())
    {
        dup2(this->readFD, STDIN_FILENO);
        close(this->writeFD);
    }
}

void Redirector::doRedirectOutput()
{
    if (this->shouldRedirectOutput())
    {
        dup2(this->writeFD, STDOUT_FILENO);
        close(this->readFD);
    }
}

void Redirector::closeRead()
{
    if (this->shouldRedirectInput())
    {
        close(this->readFD);
    }
}

void Redirector::closeWrite()
{
    if (this->shouldRedirectOutput())
    {
        close(this->writeFD);
    }
}
void Redirector::writeString(std::string output)
{
    write(this->writeFD, output.c_str(), output.length());
}
