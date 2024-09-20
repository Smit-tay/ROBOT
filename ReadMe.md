# Steampunk Digital CLock
----------

## Purpose
- Using a UArm SwiftPro robotic arm, create a digital clock in the Steampunk style

## Caution
- Only supports SwiftPro.


#### Linux/Windows/MacOS 
- Build 
	```
	mkdir build; && cd build;
	cmake ..
	make
	```
- Run Test (example assumes Linux and device attached via USB detected at /dev/TTYACM0)
    ```
	./clock /dev/TTYACM0
	```
## NOTE
- Currently under heavy development.
