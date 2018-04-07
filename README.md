# Simulink_sm
Example of work Simulink with FMC128E/FM412x500M via shared memory.

## Overview

This project contain two s-functions for Simulink, two applications for control devices and Simulink model:

* **sm_adc** - S-Function for get data from ADC via shared memory
* **sm_ctrl** - S-Function for control signal generator via shared memory
* **simulink_adc** - Application for get data from ADC with BARDY library
* **simulink_a7dac** - Application for signal generator A7_DAC
* **system_m2.slx** - Simulink model

## Simulink model

![system_m2](/docs/images/system_m2.png)

## Hardware


Name         | Description          | URL
-------------|----------------------|----------------------------------------
FMC128E      | Artix-7  USB 3.0     | http://insys.ru/fmc/fmc128e
FM412x500M   | 4xADC 12-bit 500 MHz | http://insys.ru/mezzanine/fm412x500m
A7_DAC       | 2xDAC 500 MHz        |  


## Doxygen documentation:

* sm_adc 		https://dsmv.github.io/simulink_sm/sm_adc/html/index.html
* sm_ctrl 		https://dsmv.github.io/simulink_sm/sm_ctrl/html/index.html
* simulink_adc 		https://dsmv.github.io/simulink_sm/simulink_adc/html/index.html
* simulink_a7dac 	https://dsmv.github.io/simulink_sm/simulink_a7dac/html/index.html


## easyLink 3.3.0

**sm_adc** and **sm_ctrl** based on easyLink library: http://sourcesup.renater.fr/easylink


## Result in Simulink

![result](/docs/images/result.png)

## Result in Oscilloscope

![osc](/docs/images/osc.png)
