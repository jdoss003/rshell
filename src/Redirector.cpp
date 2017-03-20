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

Redirector::Redirector(int r, int w) : readFD(r), writeFD(w), inputFile(""), outputFile("") {}

Redirector::Redirector(std::string inf, std::string outf) : readFD(STDIN_FILENO), writeFD(STDOUT_FILENO), inputFile(inf), outputFile(outf) {}

bool Redirector::shouldRedirectInput()
{
    return (readFD != STDIN_FILENO || !this->inputFile.empty());
}

bool Redirector::shouldRedirectOutput()
{
    return (writeFD != STDOUT_FILENO || !this->outputFile.empty());
}

void Redirector::doRedirectInput()
{
    if (this->shouldRedirectInput())
    {
        if (!this->inputFile.empty())
        {
            int fd;
            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

            if ((fd = open(this->inputFile.c_str(), O_RDONLY, mode)) == -1)
            {
                std::string error = "Error Opening File - ";
                error.append(this->inputFile);
                perror(error.c_str());
                return;
            }
            this->readFD = fd;
        }
        if (dup2(this->readFD, STDIN_FILENO) == -1)
        {
            perror("dup");
        }
        this->closeWrite();
    }
}

void Redirector::doRedirectOutput()
{
    if (this->shouldRedirectOutput())
    {
        if (!this->outputFile.empty())
        {
            int fd;
            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

            if ((fd = open(this->outputFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, mode)) == -1)
            {
                std::string error = "Error Opening File - ";
                error.append(this->outputFile);
                perror(error.c_str());
                return;
            }
            this->writeFD = fd;
        }
        if (dup2(this->writeFD, STDOUT_FILENO) == -1)
        {
            perror("dup");
        }
        this->closeRead();
    }
}

void Redirector::closeRead()
{
    if (readFD != STDIN_FILENO && close(this->readFD) == -1)
    {
        errno = 0;
    }
}

void Redirector::closeWrite()
{
    if (writeFD != STDOUT_FILENO && close(this->writeFD) == -1)
    {
        errno = 0;
    }
}

void Redirector::writeString(std::string output)
{
    if (write(this->writeFD, output.c_str(), output.length()) != (unsigned)output.length())
    {
        perror("write");
    }
}
