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

#include "../headers/Parser.h"

///////////Creates a task when a command and arguments end with a ";" or '/0'.//////////////////
Task* createTask(std::string input)
{
    unsigned int prevPos = 0;
    unsigned int nextPos = 0;
    std::vector<std::string> args; //Vector to hold arguments

    ///Find command
    for (unsigned int i = 0; i < (input.size() + 1); i++)
    {
        if (input[i] == ' ' && i == 0)
        {
            prevPos = i + 1;
        }
        else if (input[i] == ' ' && isalnum(input[i - 1])) //a space signals the end of a command. Create command string
        {
            std::string command = input.substr(prevPos, (i - prevPos));
            std::cout << "Command: " << command << std::endl; //TODO Delete this
            args.push_back(command);
            prevPos = i;
            break;
        }

        else if (i == prevPos) //If it reaches position of conditional statement
        {
            std::cout << "Exit: " << input << std::endl; //TODO Delete this

            ///Test if exit command is given
            if (!input.compare("exit"))
            {
                return new ExitTask();
            }
        }
    }

    ///Find arguments
    for (unsigned int j = prevPos; j < (input.size() + 1); j++)
    {
        if (input[j] == ' ')
        {
            if (isalnum(input[j + 1]) || input[j + 1] == '-')
            {
                nextPos = j + 1;
            }
            continue;
        }
        else if (isalnum(input[j]) || input[j] == '-')
        {
            if (input[j + 1] == ' ' || input[j + 1] == '\0')
            {
                std::string preArg = input.substr(nextPos, (j - nextPos + 1));
                std::cout << "Arg: " << preArg << std::endl; //TODO Delete this
                args.push_back(preArg);
            }
        }
    }

    return new ExternalTask(args);
}

////////Creates a task when a command and arguments end with either a "||" or "&&"//////////
Task* createCondTask(std::string input, Task::EnumResult r)
{
    return new ConditionalTask(createTask(input), r);
}

/////////////////////*Main Parser Function*////////////////////////
Task* Parser::parseInput(std::string strInput)
{
    //variable used for tracking position of last conditional statement
    unsigned int prevCond = 0;

    //Flags used to see if input is after a conditional statement. only one should be true at a given time
    bool orFlg = false;
    bool andFlg = false;

    std::string input;

    ///Error and comment checking
    if (strInput.size() == 0)
    {
        return new Task();
    }
    for (unsigned int k = 0; k < strInput.length(); k++)
    {
        ///check if any "#"
        if (strInput[k] == '#')//remove all input after "#" if found
        {
            input = strInput.substr(prevCond, (k - prevCond));
            if (input.empty())
            {
                Task* t = new Task();
                return t;
            }
            break;
        }

            ///Check if there is just one "|" or "&"
        else if (strInput[k] == '|' || strInput[k] == '&')
        {
            if (!(strInput[k + 1] == '|' || strInput[k + 1] == '&'))
            {
                Task* t = new Task();
                return t;
            }
            k = k + 1;
        }

        input = strInput;
    }

    //Create Task list to be "filled" according to user input
    Task* tList = new TaskList();

    ///iterate through input to find conditional statements ;, ||, &&, #, or \0
    for (unsigned int i = 0; i < (input.length() + 1); i++)
    {

        ///Finds a ";" or '\0'
        if (input[i] == ';' || input[i] == '\0') //If normal command
        {
            if (orFlg)
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createCondTask(str, Task::FAIL)); //Create conditional task
                prevCond = i + 2;
                orFlg = false; //set both flags to false
                andFlg = false;

            }
            else if (andFlg)
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createCondTask(str, Task::PASS)); //Create conditional task
                prevCond = i + 2;
                orFlg = false; //set both flags to false
                andFlg = false;
            }
            else
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createTask(str)); //Create task
                prevCond = i + 2;
            }
        }

            ///Finds a "&"
        else if (input[i] == '&')
        {
            if (orFlg)
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createCondTask(str, Task::FAIL)); //Create conditional task
                prevCond = i + 2;
                orFlg = false;
                andFlg = true;

            }
            else if (andFlg)
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createCondTask(str, Task::PASS)); //Create conditional task
                prevCond = i + 2;
            }
            else
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createTask(str)); //Create task
                prevCond = i + 2;
                andFlg = true;
            }

            i = i + 1; //Skip second "&"
        }

            ///Finds a "|"
        else if (input[i] == '|')
        {
            if (orFlg)
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createCondTask(str, Task::FAIL)); //Create conditional task
                prevCond = i + 2;

            }
            else if (andFlg)
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createCondTask(str, Task::PASS)); //Create conditional task
                prevCond = i + 2;
                andFlg = false;
                orFlg = true;
            }
            else
            {
                std::string str = input.substr(prevCond, (i - prevCond));
                tList->addSubtask(createTask(str)); //Create conditional task
                prevCond = i + 2;
                orFlg = true;
            }

            i = i + 1; //Skip second "|"
        }
    }

    return tList;
}
