Guir
====

This repository provides a GUI library and application. The
application (_Guir_) can load a XML file in which you define your GUI.
The _Guir_ application acts as a server to which you connect.
The values in your application will be automatically updated
when you make changes in _Guir_.

## Prerequisites

### Windows

We have tested this project on Windows 8.1 with Visual Studio 2013 and
Visual Studio 2015. Make sure to install at least:

- Visual Studio 2013 or
- Visual Studio 2015
- Git Bash
- CMake 3.7+

### Mac

We have tested this project on macOS Sierra 10.12.1, i7 with XCode 8.3.
Make sure to install:

- XCode 8.3+
- CMake 3.7+


## Quick Start

The _Guir_ app has been tested on Windows 8.1 and Mac Sierra 10.12.1.
On Mac open a terminal or on Windows use the Git Bash terminal. Then
clone this repository and execute:

    cd build
    ./release.sh

This will start the example application in which we draw a circle
with a radius, color and resolution that you can control with the
_Guir_ application.

Download _Guir_ for [Windows](https://github.com/ApolloInteractiveMedia/Guir/releases/download/v1.0/Guir.exe)
or [Mac](https://github.com/ApolloInteractiveMedia/Guir/releases/download/v1.0/Guir.app.zip).
Once downloaded open it and click on the _Load UI_ button. Open the file `example_guir_visualisation_ui.xml`
that you find in the examples directory.

## Background

At [Apollo](http://www.apollomedia.nl) we develop interactive, real
time visualisations for trade shows, stores, museums etc. Creating the
best experience means that we have to find the right values for
settings that are used by these applications. Being able to add a GUI
and easily tweak these things is important to us.

<img src="https://c2.staticflickr.com/4/3753/32881579024_0799d516ca_o.png" alt="Guir">

The GUI we created can be used in two ways. The first option is to
embed the GUI into your application using our opaque C API and
secondly you can use the remote Guir application.  This remote GUI can
load a XML file in which you define what elements you want to use. We
call this application _Guir_, with the `r` for *r*emote :P.  The
_Guir_ application acts as a server to which your application connects.
Whenever you change a value in this _Guir_ application
the value will be sent to the connected clients. We use TCP to make
sure that no packets will get lost during transmission.

## Using the _Guir_ as remote GUI.

Wether you want to use _Guir_ to control settings for a colorfull
light show, audio synthesis or a terminal application, it's all
possible. The thing you want to control doesn't need to be a graphical
application. Because the GUI is created in a separate application it
also doesn't matter if you're using OpenGL, Vulkan, Metal, DirectX
etc. Also you don't have to bind any mouse and keyboard events from
your application to the GUI. You simply start the _Guir_ application
and load the XML file in which you defined your GUI. In your application
you make use of the API that is defined in the `GuirApi.h` header.

_Guir_ loads a XML file to generate a GUI. In the XML file you
define what elements you want to use that are necessary to tweak
your application. 

Currently we support the following types:

- button
- sliderint
- sliderfloat
- checkbox
- togglegroup
- textinput
- listbox
- colorpicker

Creating the XML for your GUI is simple. Just make sure to wrap all
your elements inside a `<node>` as shown below. The xml will create a
GUI like shown in the image.

<img src="https://c1.staticflickr.com/3/2819/33553844351_a19c893347_o.png">

    <node type="node" direction="column" padding="10" flexgrow="1.0">
      <node type="sliderint" title="Circle resolution" name="circle-segments" min="3" max="36" step="1" />
      <node type="sliderfloat" title="Circle radius" name="circle-radius" min="0.0" max="600" step="0.5" value="120" />
      <node type="colorpicker" title="Circle color" name="circle-color" />
    </node>


## How to use Guir in your application

The `Guir` application acts as a server. In your application you
connect to this server and you'll be notified by changed made by the
user. You include the `GuirApi.h` header into your application and
link with the correct guir library. We provide several libraries for
different compilers on Windows and Mac. We provide Debug and Release
libraries. We provide MultiThreaded DLL and MultiThreaded Statically
linked libraries for Windows, make sure to link with the correct
library.

You allocate and initialize a `GuirClient` context using
`guir_client_alloc()` and `guir_client_init()`. Then set the variables
that need to change whenever the value in the _Guir_ application changes.
You do this by using the `guir_client_add_value_listener_for_*()` functions.
The first argument will always be the `GuirClient` context. The second
argument is the name of the xml node in your xml file that is used to
create the GUI. After initializing and setting the listeners you
have to call `guir_client_update()` in your render loop. By calling
`guir_client_update()` we will process data that we receive from
the _Guir_ application.

For some example code see the `examples` directory. 

## Guir XML types

### button

    <node type="button" title="Trigger particle effect" name="trigger-particle-effect" />

### sliderint

    <node type="sliderint" title="Circle resolution" name="circle-segments" min="3" max="36" step="1" />

### sliderfloat

    <node type="sliderfloat" title="Radius" name="radius" min="0.0" max="600" step="0.5" value="120" />

### checkbox

    <node type="checkbox" title="Animate" name="animate" />

### togglegroup

    <node type="togglegroup" name="Mode">
      <node type="togglegroupitem" title="Fast" flex="1.0" width="40" />
      <node type="togglegroupitem" title="Slow" flex="1.0" width="40" />
      <node type="togglegroupitem" title="Static" flex="1.0" width="40" />
    </node>

### textinput

    <node type="textinput" title="Name" name="name" />

### listbox

    <node type="listbox" title="Select shape..." name="options">
      <node type="listboxitem" title="Circle" />
      <node type="listboxitem" title="Rectangle" />
      <node type="listboxitem" title="Square" />
      <node type="listboxitem" title="Hexagon" />
      <node type="listboxitem" title="Donut" />
    </node>


### colorpicker

    <node type="colorpicker" title="Circle color" name="circle-color" />



