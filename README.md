# Micro transaction service (Socket Programming)
# Environment

---

Ubuntu 20.04.1 LTS ( on Windows 10 WSL )

# How to compile

---

In terminal, under the Server / Client file directory, 

1. Type in ***make*** command
2. If success, the ***server*** / ***client*** binary file should appear in the same directory

# How to execute

---

In terminal, after successful compilation,

1. Type in ***./server***  / ***./client*** command
2. Follow the instructions on the screen
3. After setting up, the application is ready to go

# New Features

---

Secure encrypted connection

- Server-Client, Client-Client connection are now protected under the SSL protocol
- Client private key and self-signed X509 certificate are generated in runtime

Micro-transaction feature

- Client can now trade with each other with a maximum amount of 20,000 per transfer
    - Exceptions : negative amount, non-login payee, non-exist payee, insufficient balance
- Transaction Process ( Suppose A → B) :
    1. A sends request message to both server and B
    2. B receives message and print out
    3. Server receives request and process the transaction, 2 conditions : 
        - Transaction Success : Print "Transaction Success \n Current Balance: ***<Balance of A>*** \n"
        - Transaction Fail: Print "Transaction Failed \n"

# References

---

Dynamically generate key and X509 certificate: [https://stackoverflow.com/questions/256405/programmatically-create-x509-certificate-using-openssl](https://stackoverflow.com/questions/256405/programmatically-create-x509-certificate-using-openssl)

OpenSSL establish SSL connection: 

[http://neokentblog.blogspot.com/2012/10/openssl-ssl.html](http://neokentblog.blogspot.com/2012/10/openssl-ssl.html)
