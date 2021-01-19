# Multi-thread Server readme
# Environment
Ubuntu 20.04.1 LTS ( on Windows 10 WSL )

# How to compile
In terminal, under the file directory, 

1. Type in ***make*** command
2. If success, the ***server*** binary file should appear in the same directory

# How to execute
In terminal, after successful compilation,

1. Type in ***./server*** command
2. Follow the instructions and enter the port number for server to listen
3. System logs should appear
4. Server is ready to serve

# Reply Messages
- Register
    - 100***<space>***OK***<LF>*** : Register Success
    - 210***<space>***NAME_EXIST***<LF>*** : User name existed
- Login
    - ***<accountBalance><LF>***

        ***<number of accounts online><LF>***

        ***<userAccount1>#<userAccount1_IPaddr>#<userAccount1_portNum><LF>***

        ***<userAccount2>#<userAccount2_IPaddr>#<userAccount2_portNum><LF> ...*** : Login Success

    - 220***<space>***AUTH_FAIL***<LF>*** : Account haven't registered
    - 230***<space>***HAVE_LOGGED_SRY***<LF>*** : Account has been logged in
- List
    - ***<accountBalance><LF>***

        ***<number of accounts online><LF>***

        ***<userAccount1>#<userAccount1_IPaddr>#<userAccount1_portNum><LF>***

        ***<userAccount2>#<userAccount2_IPaddr>#<userAccount2_portNum><LF> ...*** : List Format

- Logout
    - Bye***<LF>*** : Logout Success

# Exception Handling
- Aside from the above:
    - The server will auto logout user if the connection terminates unexpectedly

# Class Diagram
![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/15030e4a-8752-4f34-bde8-ad6e035e3047/Untitled.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/15030e4a-8752-4f34-bde8-ad6e035e3047/Untitled.png)

# References
Jacob Sorber Tutorials:

- Multi-thread Server : [https://www.youtube.com/watch?v=Pg_4Jz8ZIH4&ab_channel=JacobSorber](https://www.youtube.com/watch?v=Pg_4Jz8ZIH4&ab_channel=JacobSorber)
- Thread Pool, Mutex: [https://www.youtube.com/watch?v=FMNnusHqjpw&t=156s&ab_channel=JacobSorber](https://www.youtube.com/watch?v=FMNnusHqjpw&t=156s&ab_channel=JacobSorber)
- Conditional Variables: [https://www.youtube.com/watch?v=P6Z5K8zmEmc&t=284s&ab_channel=JacobSorber](https://www.youtube.com/watch?v=P6Z5K8zmEmc&t=284s&ab_channel=JacobSorber)

server.cpp, line 44: [https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c](https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c)

thread_pool.cpp, line 11: [https://stackoverflow.com/questions/1151582/pthread-function-from-a-class](https://stackoverflow.com/questions/1151582/pthread-function-from-a-class)
