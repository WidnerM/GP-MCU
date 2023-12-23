# MCU Controller Extension for Gig Performer

## Overview

An extension for [Gig Performer](https://gigperformer.com) to utilize the MCU-compatible controllers.  Popular controllers using the MCU protocol include the Icon Platform M+, the Behringer X-Touch, Presonus FaderPort, etc.

> _Disclaimer: This is a user-created extension and is not supported by Deskew Technologies. Extensions can cause unexpected behaviour in Gig Performer, and in extreme situations could cause it to crash. Please test it thoroughly before using it in any live performances!_

## Installation

Download the extension (library) files for either MacOS or Windows from the [GP-MCU Extension](https://github.com/WidnerM/GP-MCU/releases) page.  

Mac users will need to move the _libGP-MCU.dylib_ file into this folder and then restart Gig Performer:
```
/Users/Shared/Gig Performer/Extensions
```
Windows users will need to move the _GP-MCU.dll_ file into this folder and then restart Gig Performer:
```
C:\Users\Public\Documents\Gig Performer\Extensions
```
When opening Gig Performer, a prompt will appear asking to confirm whether to install the extension.

## Usage

For the extension to successfully communicate with your MCU controller you must put your controller into MCU compatibility mode.  How you do this depends on the make and model of your controller.

Documentation is available in this repository describing the functionality of the extension and how to configure your Rackspaces, Songs, and Widgets to interact with the MC controller in the manner you would like.

A demonstration Gigfile is also provided in this repository.  You should be able to load this Gigfile, see how it interacts with the controller, and look at the various widgets to see how their configurations translate into what you see on the MCU controller.  This is fully described in the documentation, but may be easier to grasp by playing with the example Gigfile.

## Building the Extension

To customize/build the extension, refer to the [Gig Performer SDK](https://github.com/gigperformer/gp-sdk) GitHub page.