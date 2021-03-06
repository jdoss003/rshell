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

#include "../../headers/utils/FileUtils.h"

bool FileUtils::openFileInput(std::string filePath, Redirector &r)
{
    int fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    filePath = EnvUtils::getCompletePath(filePath);

    if ((fd = open(filePath.c_str(), O_RDONLY, mode)) == -1)
    {
        std::string error = "Error Opening File - ";
        error.append(filePath);
        perror(error.c_str());
        return false;
    }
    close(fd);

    r = Redirector(filePath, "");
    return true;
}

bool FileUtils::openFileOutput(std::string filePath, bool append, Redirector &r)
{
    int fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int openMode (append ? O_APPEND : O_TRUNC);
    filePath = EnvUtils::getCompletePath(filePath);

    if ((fd = open(filePath.c_str(), O_WRONLY | O_CREAT | openMode, mode)) == -1)
    {
        std::string error = "Error Opening File - ";
        error.append(filePath);
        perror(error.c_str());
        return false;
    }
    close(fd);

    r = Redirector("", filePath, append);
    return true;
}
