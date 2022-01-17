#Minik Os for mini(k) spaces

https://minik.16x4.com

Minik OS is a binary format embedded operating system. Unlike RTOS(es),
Minik OS is not a framework. It's a compiled operating system, ready
to run on embedded devices.

Curious to see it in action? Here is a video of Minik OS, running 3 different
programs in parallel plus having it's kernel source code reader and status
daemons on. [https://www.youtube.com/watch?v=5vTQgAibZtk](https://www.youtube.com/watch?v=5vTQgAibZtk)

Minik is coded using C++ and Arduino Framework with the power of ESP32 bindings
and FreeRTOS.

Minik is a multi-threaded operating system. Therefore, it can not be assumed
as a "Real Time" operating system because of it's internal task switches.

On ESP32, a single mission critical task can be assigned to the second core.
Or tasks can be shared between cores.

As of now all daemons run on CORE0 and tasks are assigned to Cores in turns.
So each turn will get (almost) equal number of tasks (programs).

Minik OS includes an interpreted programming language called CManager. Syntax
of CManager can be called a "pseudo-assembler". It is a bit ezoteric. Primarily
because it is designed to run on 8bit small chips. Other interpreted languages
like micro-python requires a bit more processing power and ram area for compiling.

You can see some examples of CManager in [scripts director.](https://github.com/sinanislekdemir/minik/tree/main/scripts)

Minik has internal daemons and tasks running within a semaphore. Daemons run before
the tasks.

There are daemons for:

- Serial communications
- Interrupt handlers

    sinan islekdemir
    sinan@islekdemir.com
