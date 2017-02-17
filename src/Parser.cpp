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
    unsigned long i = 0;
    unsigned long prevPos = 0;
    std::vector<std::string> args; // Vector to hold arguments

    ///Find command
    for (; i < input.size(); ++i)
    {
        if (input[i] == '\"')
        {
            if (int(i - 1) >= 0 && input[i - 1] == '\\')
            {
                input.erase(i - 1, 1);
                --i;
            }
            else
            {
                input.erase(i, 1);

                for (unsigned long j = i; j < input.length(); ++j)
                {
                    if (input[j] == '\"')
                    {
                        if (int(j - 1) >= 0 && input[j - 1] == '\\')
                        {
                            input.erase(j - 1, 1);
                            --j;
                        }
                        else
                        {
                            input.erase(j, 1);
                            i = j - 1;
                            break;
                        }
                    }
                }
            }
        }
        else if (input[i] == ' ') //a space signals the end of a command. Create command string
        {
            std::string command = input.substr(prevPos, i - prevPos);
            //std::cout << "Arg: " << command << std::endl;

            if (args.size() == 0 && command.compare("exit") == 0)
            {
                std::cout << "WARNING: Exit command does not have any arguments!" << std::endl;
                return new ExitTask();
            }

            args.push_back(command);
            prevPos = i + 1;
        }
    }

    if (i > prevPos)
    {
        std::string command = input.substr(prevPos, i - prevPos);
        //std::cout << "Arg: " << command << std::endl;

        if (command.compare("exit") == 0)
        {
            return new ExitTask();
        }

        args.push_back(command);
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
    std::string input = strInput;

    for (unsigned long i = 0; i < input.length(); ++i)
    {
        if (input[i] == '\"')
        {
            if (int(i - 1) >= 0 && input[i - 1] == '\\')
            {
                continue;
            }
            else
            {
                unsigned long j = i + 1;
                for (; j < input.length(); ++j)
                {
                    if (input[j] == '\"')
                    {
                        if (int(j - 1) < 0 || input[j - 1] != '\\')
                        {
                            i = j;
                            break;
                        }
                    }
                }

                if (j == input.length())
                {
                    std::cout << "Error: Found unmatched quote!" << std::endl;
                    std::cout << input << std::endl;
                    std::cout << std::right << std::setw(int(i) + 1) << '^' << std::endl;
                    return new Task();
                }
            }
        }
        else if (input[i] == '|' || input[i] == '&')
        {
            if (i + 1 < input.length() &&
                    ((input[i] == '|' && input[i + 1] == '|') || (input[i] == '&' && input[i + 1] == '&')))
            {
                ++i;
            }
            else
            {
                std::cout << "Error: Found invalid connector!" << std::endl;
                std::cout << input << std::endl;
                std::cout << std::right << std::setw(int(i) + 1) << '^' << std::endl;
                return new Task();
            }
        }
        else if (input[i] == '#')
        {
            break;
        }
    }

    for (unsigned long i = 0; i < input.length(); ++i)
    {
        if (input[i] == '\"')
        {
            for (unsigned long j = i + 1; j < input.length(); ++j)
            {
                if (input[j] == '\"')
                {
                    if (int(j - 1) < 0 || input[j - 1] != '\\')
                    {
                        i = j;
                        break;
                    }
                }
            }
        }
        else if (input[i] == '#')
        {
            input = strInput.substr(0, i);
        }
        else if (isspace(input[i]))
        {
            input[i] = ' ';

            if (i + 1 < input.length() && isspace(input[i + 1]))
            {
                while (i + 1 < input.length() && isspace(input[i + 1]))
                {
                    input.erase(i + 1, 1);
                }
            }

            if (i - 1 > 0 && (input[i - 1] == '|' || input[i - 1] == '&' || input[i - 1] == ';'))
            {
                input.erase(i, 1);
                --i;
            }
            else if (i + 1 < input.length() && (input[i + 1] == '|' || input[i + 1] == '&' || input[i + 1] == ';'))
            {
                input.erase(i, 1);
                --i;
            }
        }
    }

    if (input.length() > 0 && isspace(input[0]))
    {
        input.erase(0, 1);
    }

    if (input.length() > 0 && isspace(input[input.length()]))
    {
        input.erase(input.length(), 1);
    }

//    std::cout << "Input after cleanup: " << input << std::endl;

    //Create Task list to be "filled" according to user input
    Task* tList = new TaskList();

    unsigned long i = 0;
    unsigned long prevCond = 0;

    bool condition = false;
    bool orFlg    = false;

    ///iterate through input to find conditional statements ;, ||, &&
    for (; i < input.length(); ++i)
    {
        if (input[i] == '\"')
        {
            for (unsigned long j = i + 1; j < input.length(); ++j)
            {
                if (input[j] == '\"')
                {
                    if (int(j - 1) < 0 || input[j - 1] != '\\')
                    {
                        i = j;
                        break;
                    }
                }
            }
        }
        else if (input[i] == ';')
        {
            if (i - prevCond < 1)
            {
                tList->addSubtask(new Task());
            }
            else
            {
                std::string cmd = input.substr(prevCond, i - prevCond);

                if (condition)
                {
                    tList->addSubtask(createCondTask(cmd, (orFlg ? Task::FAIL : Task::PASS)));
                }
                else
                {
                    tList->addSubtask(createTask(cmd));
                }
            }

            condition = false;
            prevCond = i + 1;
        }
        else if (input[i] == '|')
        {
            if (i - prevCond < 1)
            {
                tList->addSubtask(new Task());
            }
            else
            {
                std::string cmd = input.substr(prevCond, i - prevCond);

                if (condition)
                {
                    tList->addSubtask(createCondTask(cmd, (orFlg ? Task::FAIL : Task::PASS)));
                }
                else
                {
                    tList->addSubtask(createTask(cmd));
                }
            }

            condition = true;
            orFlg = true;
            prevCond = i + 2;
            ++i;
        }
        else if (input[i] == '&')
        {
            if (i - prevCond < 1)
            {
                tList->addSubtask(new Task());
            }
            else
            {
                std::string cmd = input.substr(prevCond, i - prevCond);

                if (condition)
                {
                    tList->addSubtask(createCondTask(cmd, (orFlg ? Task::FAIL : Task::PASS)));
                }
                else
                {
                    tList->addSubtask(createTask(cmd));
                }
            }

            condition = true;
            orFlg = false;
            prevCond = i + 2;
            ++i;
        }
    }

    if (i > prevCond)
    {
        std::string cmd = input.substr(prevCond, i - prevCond);
        tList->addSubtask(createTask(cmd));
    }

    return tList;
}
