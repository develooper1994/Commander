# Commander
Commander simplifies process management tasks. 
You can issue commands to the shell as child processes. 
Here are the key features of Commander:
- You can run commands periodically.
- The software waits for the command to finish, retrieves the exit code, and reads the stdout and stderr outputs.

Note: This concept is inspired by the voidrealm Qt6 Core Udemy Tutorial.

- `Commander`: This is the base class that provides essential process functionalities. It is used within the `CommanderManager`. It includes a periodic call mechanism that executes if the specified time interval is greater than 0.
- `CommanderManager`: This class creates instances of `Commander` and executes them concurrently using `QtConcurrent`. With this approach, you don't need to worry about waiting for the processes to complete.

> Example Usage

```cpp
    CommanderManager m_commanderManager;
    QStringList arguments;
    arguments << "-l" << "-a" << "-h";
    m_commanderManager.process(arguments, "ls", "\n", 10'000);
```
