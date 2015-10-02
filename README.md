RazerHydra
==========
A KL extension for the Sixense Razer Hydra controller.

RazerHydra allows you to read the device tracking information as well as getting information about button states etc.

extension status
================

This extension has been built and tested against Fabric Engine 2.0.0. Higher release might work, but this hasn't been validated. Feel free to use this code for reference or training purposes, it's meant to be an example of how to integrate custom libraries into KL.

structure
=========

The RazerHydra extension uses the KL2EDK tool to generate the required C++ headers. Since the number of callbacks is quite small, they are handcrafted and don't use KL2EDK's advanced codegen functionality.

The latest KL2EDK documentation can be found here: http://documentation.fabricengine.com/FabricEngine/latest/HTML/ExtensionAuthoringGuide/kl2edk.html

Additional it uses KL2DFG to generate Canvas presets for each function of the API. These can be found in the stage output folder after building.

supported platforms
===================

RazerHydra is currently only supported on Windows.
For Fabric Engine 2.0.0 the extension has to be built using Visual Studio 2010 (through scons), since the Sixense SDK is built with 2010.

building
========

A scons (http://www.scons.org/) build script is provided. RazerHydra depends on
* The Sixense Core SDK (062612 or higher)
* A static build of boost (1.55.0 or higher)

To inform scons where to find the Fabric Engine includes as well as the thirdparty libraries, you need to set the following environment variables:

* FABRIC_DIR: Should point to Fabric Engine's installation folder.
* SIXENSE_INCLUDE_DIR: Should point to the include folder of the Sixense Core SDK.
* SIXENSE_LIBRARY_DIR: Should point to the library folder of the Sixense Core SDK.
* BOOST_INCLUDE_DIR: Should point to Boost include folder.
* BOOST_LIBRARY_DIR: Should point to Boost library folder.

The temporary files will be built into the *build* folder, while the structured output files will be placed in the *stage* folder. Optionally you can define the FABRIC_EXTS_INSTALL_DIR environment variable, to which the extension will be installed.

To build the extension you need to be in an Fabric Engine enabled shell, define the mentioned environment variables (FABRIC_DIR) will already be defined, and type

    scons

Note: The build-env.sh exists purely for Fabric Engine internal use, but you can use it as a guide for your own build setup.

usage in canvas
==================

To use the extension inside of Canvas, simply cd into the repo's root folder and source deploy.sh

    cd RazerHydra && deploy.sh

You can of course add this to your .bashrc file, for example. Alternatively inspect deploy.sh to see the required environment variables.

samples
=======

The Canvas sample files in the canvas folder can be run with the canvas standalone command line tool.

license
==========

The license used for this extension can be found it in the root folder of the repository, in LICENSE.TXT.
