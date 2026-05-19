# Writing down my understandings and screenshots

## What is a firmware library? 
Considering that firmware code sits close to the hardware, it needs an extensive definition of that hardware like register maps, bit definitions, peripherals that describes the hardware to spec allowing reusabilty of the code. 
It can consist of HAL definitions, functions, special memory architectural scopes, interrupt handlers, etc. 
Just look at the memory for example. Just one piece of the hardware but even in the smallest quantities we can think of like 256 KB, we have so many different segmentations and areas that are necessary to be handled carefully. Firmware libraries can take care of the bulk of these tasks, making the address space very human readable. 

<img width="280" height="253" alt="image" src="https://github.com/user-attachments/assets/a80ba46f-7332-4d82-8c13-f6f97229a090" />

## Why are APIs important? 
APIs provide a developer-friendly layer to the internal workings of the systems on top of which application code can be written. Hardware, projects and other requirements can change but fundamental APIs should stay the same, defining the same physics. 
For eg, UART channels will always communicate following the same standards and byte frames across all devices. 

Mathematical functions, communication interfaces, power supply modes, other hardware features, etc can all be exposed through APIs which allow extremely integral, useful logic to be faithfully hidden behind simple function signatures that can be used in the application logic. 

From the above example of a memory, a function like `memory_write()` or `memory_erase()` can be used easily to do the enormous electrical task of clearing out the electrons from a physical space at a particular address. 

## What was understood from the lab code? 
- The three API functions for the GPIO actions: init, write, read have an intentional structure to them. 
- They share a similar signature where the first parameter is always the subject of the action, the PIN.
- Clear and consise nomenclature. Very important for code that will be used across different projects.

- This was a simple example of how code should ideally be structured for low-level projects, given that GPIO is a general concept and not meant only for one project. This kind of API/function structure is easily usable across other codebases.
- Also this is not a running loop, which is something I have quite often noticed in lowlevel firware code.

## Program Execution screenshot: 

<img width="480" height="253" alt="image" src="https://github.com/palcodes/vsdsquadron-mini-riscv/blob/main/execution.png" />
