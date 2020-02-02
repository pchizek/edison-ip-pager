# edison-ip-pager
A project to make an internet pager using an Intel Edison that prints messages recieved to an LCD. 

# Overview
The Intel Edison, running an http server, serves a web form to a client asking for a message. The response, in the form of a POST request,
is parsed by the Edison which calls a platform-specific shell program (serial/serialSend) to send that data over a UART connection. An Arduino
Uno recieves the message data, stores it, and displays it on the LCD screen. Additionally, the Uno handles three buttons, two to switch between
the stored messages and one to delete the message.

# Dependencies
[Simple-Web-Server](https://gitlab.com/eidheim/Simple-Web-Server) by Ole Christian Eidheim

# Future improvements
* Landing page for sending a message
* Status pages and rejection notices for empty fields
* Directly driving the lcd1602a with the Intel Edison's GPIO
* Menu support and a counter for new and stored messages
* https support
* Auto startup upon powering on the system

# FAQ
<p>
<b>Q: Is this secure?</b>
<br><b>A:</b> Probably not! The last official Yocto image for the Intel Edison came out in 2016. Also, I'm an embedded guy, not a security guy.
I would only trust this to people I know.
</p>
<p>
<b>Q: Why is there an Arduino in this system?</b>
<br><b>A:</b> I didn't have enough voltage dividers to run the lcd1602a with the Intel Edison. The Edison breakout board I have has 1.8V I/O and
the LCD needs 5V.
</p>
