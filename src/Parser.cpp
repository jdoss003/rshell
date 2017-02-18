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

/*
 * Creates a task when a command and arguments
 * @param input is the string of command and arguments
 *
 * @returns a Task object that will execute the command and arguments
 */
Task* createTask(std::string input)
{
    unsigned long i = 0;
    unsigned long prevPos = 0;
    std::vector<std::string> args; // Vector to hold arguments

    // Separate arguments
    for (; i < input.size(); ++i)
    {
        if (input[i] == '\"')
        {
            if (int(i - 1) >= 0 && input[i - 1] == '\\') // found \"
            {
                input.erase(i - 1, 1); // remove the slash
                --i;
            }
            else // found "
            {
                input.erase(i, 1); // remove the " so it becomes a "

                // need to find the next "
                for (unsigned long j = i; j < input.length(); ++j)
                {
                    if (input[j] == '\"')
                    {
                        if (int(j - 1) >= 0 && input[j - 1] == '\\') // found a \" inside quotes
                        {
                            input.erase(j - 1, 1); // remove the \ so it becomes a "
                            --j;
                        }
                        else // found the next "
                        {
                            input.erase(j, 1); // remove the "
                            i = j - 1;
                            break;
                        }
                    }
                }
            }
        }
        else if (input[i] == ' ') // found a space; signals the end of an arg
        {
            std::string command = input.substr(prevPos, i - prevPos); // copy arg to new string
            //std::cout << "Arg: " << command << std::endl;

            if (args.size() == 0 && command.compare("exit") == 0) // if the first arg is "exit" return exit task
            {
                std::cout << "WARNING: Exit command does not have any arguments!" << std::endl;
                return new ExitTask();
            }

            args.push_back(command); // add arg to vector
            prevPos = i + 1;
        }
    }

    if (i > prevPos) // there is no space at the end to we may need to catch the last arg
    {
        std::string command = input.substr(prevPos, i - prevPos); // copy arg to new string
        //std::cout << "Arg: " << command << std::endl;

        if (command.compare("exit") == 0) // if the first arg is "exit" return exit task
        {
            return new ExitTask();
        }

        args.push_back(command); // add arg to vector
    }

    return new ExternalTask(args); // return task
}

/*
 * Creates a task when a command and arguments
 * @param input is the string of command and arguments
 * @param r is the desired result of the on which the task should be run
 *
 * @returns a conditional Task object that will execute the command and arguments
 */
Task* createCondTask(std::string input, Task::EnumResult r)
{
    return new ConditionalTask(createTask(input), r);
}

/*
 * Converts the input into a task object that can be run
 * @param input is the string of command and arguments
 *
 * @returns a task object that will execute the commands from the input
 */
Task* Parser::parseInput(std::string strInput)
{
    std::string input = strInput; // preserve original string in case it is needed

    // first check for errors in input
    for (unsigned long i = 0; i < input.length(); ++i)
    {
        if (input[i] == '\"') // check that quotes are in pairs
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
        else if (input[i] == '|' || input[i] == '&') // make sure connectors are in pairs and correct
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
        else if (input[i] == '#') // we do not care about anything after comments
        {
            break;
        }
    }

    // pre-process the input input
    for (unsigned long i = 0; i < input.length(); ++i)
    {
        if (input[i] == '\"') // skip over stuff in quotes
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
        else if (input[i] == '#') // delete anything after comment
        {
            input = strInput.substr(0, i);
        }
        else if (isspace(input[i])) // remove extra spaces
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

    // erase unnecessary space at beginning
    if (input.length() > 0 && isspace(input[0]))
    {
        input.erase(0, 1);
    }

    // erase unnecessary space at end
    if (input.length() > 0 && isspace(input[input.length()]))
    {
        input.erase(input.length(), 1);
    }

//    std::cout << "Input after cleanup: " << input << std::endl;

    // Create task list to be filled according to the input
    Task* tList = new TaskList();

    unsigned long i = 0;
    unsigned long prevCond = 0;

    bool condition = false;
    bool orFlg    = false;

    // iterate through input to break it into tasks
    for (; i < input.length(); ++i)
    {
        if (input[i] == '\"') // skip over quotes
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
        else if (input[i] == ';') // found a normal connector
        {
            if (i - prevCond < 1) // nothing between connectors so make it a basic task obj
            {
                tList->addSubtask(new Task());
            }
            else
            {
                std::string cmd = input.substr(prevCond, i - prevCond); // separate command

                if (condition) // make a conditional task or normal task from input
                {
                    tList->addSubtask(createCondTask(cmd, (orFlg ? Task::FAIL : Task::PASS)));
                }
                else
                {
                    tList->addSubtask(createTask(cmd));
                }
            }

            condition = false; // set conditions for next command
            prevCond = i + 1;
        }
        else if (input[i] == '|') // found the on fail connector
        {
            if (i - prevCond < 1) // nothing between connectors so make it a basic task obj
            {
                tList->addSubtask(new Task());
            }
            else
            {
                std::string cmd = input.substr(prevCond, i - prevCond); // separate command

                if (condition) // make a conditional task or normal task from input
                {
                    tList->addSubtask(createCondTask(cmd, (orFlg ? Task::FAIL : Task::PASS)));
                }
                else
                {
                    tList->addSubtask(createTask(cmd));
                }
            }

            condition = true; // set conditions for next command
            orFlg = true;
            prevCond = i + 2;
            ++i;
        }
        else if (input[i] == '&') // found the on pass connector
        {
            if (i - prevCond < 1) // nothing between connectors so make it a basic task obj
            {
                tList->addSubtask(new Task());
            }
            else
            {
                std::string cmd = input.substr(prevCond, i - prevCond); // separate command

                if (condition) // make a conditional task or normal task from input
                {
                    tList->addSubtask(createCondTask(cmd, (orFlg ? Task::FAIL : Task::PASS)));
                }
                else
                {
                    tList->addSubtask(createTask(cmd));
                }
            }

            condition = true; // set conditions for next command
            orFlg = false;
            prevCond = i + 2;
            ++i;
        }
    }

    if (i > prevCond) // no space at the end so we have to catch the last command
    {
        std::string cmd = input.substr(prevCond, i - prevCond); // separate command

        if (condition) // make a conditional task or normal task from input
        {
            tList->addSubtask(createCondTask(cmd, (orFlg ? Task::FAIL : Task::PASS)));
        }
        else
        {
            tList->addSubtask(createTask(cmd));
        }
    }

    return tList; // return the task object
}
