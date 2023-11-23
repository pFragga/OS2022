Operating Systems 2022 - Project
================================
Final Project for the Operating Systems Course at [Athens University of
Economics and Business](https://aueb.gr) during spring of 2022.

I'm uploading this now in 2023, because back then I hadn't learned git/github
and I'm hoping that I'll get some experience in working with a remote
repository.

Goal
----
Implement a booking system for seats at a theatre, using the POSIX threads
(pthreads(7)) C library.

In this system, each customer first chooses the seats that they want, then pays
for them and finally the paid amount gets added to the theatre's bank account.

These types of systems have a large amount of customers that are served from a
limited number of service points, therefore the system must implement mutual
blocking (with mutexes) and synchronization (with condition variables).

You can read more about it in the [assignment paper](os_project.pdf). It's in
Greek though, so good luck!

Usage
-----
After compiling with `make`, do:

	./test-res 100 1000

