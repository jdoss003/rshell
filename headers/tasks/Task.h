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

#ifndef RSHELL_TASK_H
#define RSHELL_TASK_H

#include <stdexcept>

#include "../Redirector.h"

class Task
{
    public:
        Task();
        virtual ~Task();

        enum EnumResult {PASS, FAIL};

        void run();
        virtual EnumResult run(EnumResult);
        virtual void addSubtask(Task* t);

        virtual void setInputRedirect(Redirector r);
        virtual void setOutputRedirect(Redirector r);

    protected:
        Redirector inputRedir;
        Redirector outputRedir;
};

#endif //RSHELL_TASK_H
